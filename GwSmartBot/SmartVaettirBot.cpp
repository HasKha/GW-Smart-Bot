#include "SmartVaettirBot.h"

#include <GWCA\GWCA\GwConstants.h>
#include "Utils.h"

SmartVaettirBot::SmartVaettirBot() : player_(PseudoAgent()), current_action_(nullptr) {

	actions_.push(new LogAction("Doing left side"));
	actions_.push(new MoveAction(13501, -20925));
	actions_.push(new MoveAction(13172, -22137));
	actions_.push(new MoveAction(12496, -22600));
	actions_.push(new MoveAction(11375, -22761));
	actions_.push(new MoveAction(10925, -23466));
	actions_.push(new MoveAction(10917, -24311));
	actions_.push(new MoveAction(9910, -24599));
	actions_.push(new MoveAction(8995, -23177));
	actions_.push(new MoveAction(8307, -23187));
	actions_.push(new MoveAction(8213, -22829));
	actions_.push(new MoveAction(8307, -23187));
	actions_.push(new MoveAction(8213, -22829));
	actions_.push(new MoveAction(8740, -22475));
	actions_.push(new MoveAction(8880, -21384));
	actions_.push(new MoveAction(8684, -20833));
	actions_.push(new MoveAction(8982, -20576));

	actions_.push(new LogAction("Waiting for left ball"));
	actions_.push(new WaitAction(12 * 1000));
	actions_.push(new UseSkillAction(SkillSlot::HoS, Target::Self));
	actions_.push(new WaitAction(6 * 1000));

	actions_.push(new LogAction("Doing right side"));
	actions_.push(new MoveAction(10196, -20124));
	actions_.push(new MoveAction(9976, -18338));
	actions_.push(new MoveAction(11316, -18056));
	actions_.push(new MoveAction(10392, -17512));
	actions_.push(new MoveAction(10114, -16948));
	actions_.push(new MoveAction(10729, -16273));
	actions_.push(new MoveAction(10810, -15058));
	actions_.push(new MoveAction(11120, -15105));
	actions_.push(new MoveAction(11670, -15457));
	actions_.push(new MoveAction(12604, -15320));
	actions_.push(new MoveAction(12476, -16157));

	actions_.push(new LogAction("Waiting for right ball"));
	actions_.push(new WaitAction(15 * 1000));
	actions_.push(new UseSkillAction(SkillSlot::HoS, Target::Self));
	actions_.push(new WaitAction(5 * 1000));

	actions_.push(new LogAction("Blocking enemies"));
	actions_.push(new MoveAction(12920, -17032));
	actions_.push(new MoveAction(12847, -17136));
	actions_.push(new MoveAction(12720, -17222));
	actions_.push(new WaitAction(300));
	actions_.push(new MoveAction(12617, -17273));
	actions_.push(new WaitAction(300));
	actions_.push(new MoveAction(12518, -17305));
	actions_.push(new WaitAction(300));
	actions_.push(new MoveAction(12445, -17327));

	actions_.push(new LogAction("Please take over, and kill them all"));
}

void SmartVaettirBot::Update(World& world) {
	if (world.player.Id == 0) return;

	player_ = world.player;
	UpdateCounts(world.agents);

	StayAlive();

	if (!actions_.empty()) {
		if (current_action_ == nullptr) {
			current_action_ = actions_.front();
			current_action_->Perform(world);
		}

		current_action_->Update(world);

		if (current_action_->Done(world)) {
			actions_.pop();
			delete current_action_;
			current_action_ = nullptr;
		}
	}
}

void SmartVaettirBot::StayAlive() {
	if (player_.GetIsDead()) return; // Sorry, I failed
	if (CheckUseSF()) {
		// nothing, we already used sf
	} else if (gwca().IsSkillRecharged(SkillSlot::Shroud)) {
		if (gwca().GetEffectTimeRemaining(GwConstants::SkillID::Shroud_of_Distress) == 0
			&& proximity_count_ > 0) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (player_.HP < 0.6) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (adjacent_count_ > 20) {
			UseSkillEx(SkillSlot::Shroud);
		}
	} else if (gwca().IsSkillRecharged(SkillSlot::WayOfPerf)) {
		if (player_.HP < 0.5) {
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
		return UseSkillEx(SkillSlot::Paradox) && UseSkillEx(SkillSlot::SF);
	} else {
		return false;
	}
}

void SmartVaettirBot::UpdateCounts(std::vector<AgentPosition> agents) {
	proximity_count_ = 0;
	spellcast_count_ = 0;
	area_count_ = 0;
	adjacent_count_ = 0;

	for (AgentPosition agent : agents) {
		float distance = Utils::GetSquaredDistance(player_, agent);
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

bool SmartVaettirBot::UseSkillEx(int slot, int target) {
	printf("using skill %d\n", slot);
	if (player_.GetIsDead()) return false;
	if (!gwca().IsSkillRecharged(slot)) return false;
	if (player_.GetEnergy() < SkillCost[slot]) return false;

	gwca().UseSkill(slot, target, false);
	do {
		Sleep(50); // warning: no deadlock check. Bot could die and we're stuck here.
	} while (gwca().IsSkillRecharged(slot));
	printf("done using skill %d\n", slot);
	return true;
}
