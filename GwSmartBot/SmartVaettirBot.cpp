#include "SmartVaettirBot.h"

#include <GWCA\GWCA\GwConstants.h>
#include "Utils.h"

SmartVaettirBot::SmartVaettirBot(const World& world) : 
	world_(world),
	current_action_(nullptr),
	current_cast_(nullptr) {

	action_queue_.push(new LogAction("Doing left side"));
	action_queue_.push(new MoveAction(13501, -20925));
	action_queue_.push(new MoveAction(13172, -22137));
	action_queue_.push(new MoveAction(12496, -22600));
	action_queue_.push(new CustomSimpleAction([](const World& world) {
		float closest_dist = GwConstants::SqrRange::Compass;
		DWORD closest_id = Target::Self;
		for (AgentPosition agent : world.agents()) {
			if (closest_dist > Utils::GetSquaredDistance(world.player(), agent)) {
				closest_dist = Utils::GetSquaredDistance(world.player(), agent);
				closest_id = agent.id;
			}
		}
		GWCAClient::Api().ChangeTarget(closest_id);
	}));
	action_queue_.push(new MoveAction(11375, -22761));
	action_queue_.push(new MoveAction(10925, -23466));
	action_queue_.push(new MoveAction(10917, -24311));
	action_queue_.push(new MoveAction(9910, -24599));
	action_queue_.push(new MoveAction(8995, -23177));
	action_queue_.push(new MoveAction(8307, -23187));
	action_queue_.push(new MoveAction(8213, -22829));
	action_queue_.push(new MoveAction(8307, -23187));
	action_queue_.push(new MoveAction(8213, -22829));
	action_queue_.push(new MoveAction(8740, -22475));
	action_queue_.push(new MoveAction(8880, -21384));
	action_queue_.push(new MoveAction(8684, -20833));
	action_queue_.push(new MoveAction(8982, -20576));

	action_queue_.push(new LogAction("Waiting for left ball"));
	action_queue_.push(new WaitAction(12 * 1000));
	action_queue_.push(new UseSkillAction(SkillSlot::HoS, Target::Current));
	action_queue_.push(new WaitAction(6 * 1000));

	action_queue_.push(new LogAction("Doing right side"));
	action_queue_.push(new MoveAction(10196, -20124));
	action_queue_.push(new MoveAction(9976, -18338));
	action_queue_.push(new MoveAction(11316, -18056));
	action_queue_.push(new MoveAction(10392, -17512));
	action_queue_.push(new MoveAction(10114, -16948));
	action_queue_.push(new MoveAction(10729, -16273));
	action_queue_.push(new MoveAction(10810, -15058));
	action_queue_.push(new MoveAction(11120, -15105));
	action_queue_.push(new MoveAction(11670, -15457));
	action_queue_.push(new MoveAction(12604, -15320));
	action_queue_.push(new MoveAction(12476, -16157));

	action_queue_.push(new LogAction("Waiting for right ball"));
	action_queue_.push(new WaitAction(15 * 1000));
	action_queue_.push(new UseSkillAction(SkillSlot::HoS, Target::Current));
	action_queue_.push(new WaitAction(5 * 1000));

	action_queue_.push(new LogAction("Blocking enemies"));
	action_queue_.push(new MoveAction(12920, -17032, 30));
	action_queue_.push(new MoveAction(12847, -17136, 30));
	action_queue_.push(new MoveAction(12720, -17222, 30));
	action_queue_.push(new WaitAction(300));
	action_queue_.push(new MoveAction(12617, -17273, 30));
	action_queue_.push(new WaitAction(300));
	action_queue_.push(new MoveAction(12518, -17305, 20));
	action_queue_.push(new WaitAction(300));
	action_queue_.push(new MoveAction(12445, -17327, 10));

	action_queue_.push(new LogAction("Please take over, and kill them all"));
}

void SmartVaettirBot::Update() {
	if (world_.player().Id == 0) return;
	if (GWCAClient::Api().GetInstancType() != GwConstants::InstanceType::Explorable) return;
	if (GWCAClient::Api().GetMapID() != GwConstants::MapID::Jaga_Moraine) return;

	static const bool doactions = true;
	static bool resume = false;

	if (!cast_queue_.empty()) {
		if (current_cast_ == nullptr) {
			current_cast_ = cast_queue_.front();
			current_cast_->Perform(world_);
		} else {
			current_cast_->Update(world_);
		}

		if (current_cast_->Done(world_)) {
			cast_queue_.pop();
			delete current_cast_;
			current_cast_ = nullptr;
		}
		
		resume = true;
	} else {
		UpdateCounts();

		StayAlive();
		
		if (doactions && !action_queue_.empty()) {
			if (current_action_ == nullptr) {
				current_action_ = action_queue_.front();
				current_action_->Perform(world_);
				resume = false;
			} else if (resume) {
				printf("resume action\n");
				current_action_->Resume(world_);
				resume = false;
			} else {
				current_action_->Update(world_);
			}

			if (current_action_->Done(world_)) {
				action_queue_.pop();
				delete current_action_;
				current_action_ = nullptr;
			}
		}
	}
}

void SmartVaettirBot::StayAlive() {
	if (world_.player().GetIsDead()) return; // Sorry, I failed
	if (CheckUseSF()) {
		// nothing, we already used sf
	} else if (gwca().IsSkillRecharged(SkillSlot::Shroud)) {
		if (gwca().GetEffectTimeRemaining(GwConstants::SkillID::Shroud_of_Distress) == 0
			&& proximity_count_ > 0) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (world_.player().HP < 0.6) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (adjacent_count_ > 20) {
			UseSkillEx(SkillSlot::Shroud);
		}
	} else if (gwca().IsSkillRecharged(SkillSlot::WayOfPerf)) {
		if (world_.player().HP < 0.5) {
			UseSkillEx(SkillSlot::WayOfPerf);
		} else if (adjacent_count_ > 20) {
			UseSkillEx(SkillSlot::WayOfPerf);
		}
	} else if (gwca().IsSkillRecharged(SkillSlot::Channeling)) {
		if (area_count_ > 5 && gwca().GetEffectTimeRemaining(GwConstants::SkillID::Channeling) < 2000) {
			UseSkillEx(SkillSlot::Channeling);
		}
	}
}

bool SmartVaettirBot::CheckUseSF() {
	if (gwca().IsSkillRecharged(SkillSlot::SF) && proximity_count_ > 0) {
		UseSkillEx(SkillSlot::Paradox);
		UseSkillEx(SkillSlot::SF);
		return true;
	} else {
		return false;
	}
}

void SmartVaettirBot::UpdateCounts() {
	proximity_count_ = 0;
	spellcast_count_ = 0;
	area_count_ = 0;
	adjacent_count_ = 0;

	for (AgentPosition agent : world_.agents()) {
		float distance = Utils::GetSquaredDistance(world_.player(), agent);
		if (distance < 1300 * 1300) {
			++proximity_count_;
			if (distance < GwConstants::SqrRange::Spellcast) {
				++spellcast_count_;
				if (distance < GwConstants::SqrRange::Area) {
					++area_count_;
					if (distance < GwConstants::SqrRange::Adjacent) {
						++adjacent_count_;
					}
				}
			}
		}
	}
}

void SmartVaettirBot::UseSkillEx(int slot, int target) {
	cast_queue_.push(new UseSkillAction(slot, target, SkillCost[slot]));
}
