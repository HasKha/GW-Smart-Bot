#include "SmartVaettirBot.h"

#include <GWCA\GWCA\GwConstants.h>
#include "Utils.h"

const int SmartVaettirBot::SkillCost[] = {
	15, 5, 15, 5, 5, 5, 10, 5
};

SmartVaettirBot::SmartVaettirBot(GWCAClient& gwca) :
	Bot(gwca), player_(PseudoAgent()), current_action_(nullptr) {

	actions_.push(new MoveAction(gwca, 13501, -220925));
	actions_.push(new MoveAction(gwca, 13172, -22137));
}

void SmartVaettirBot::Update(World& world) {
	if (world.player.Id == 0) return;

	player_ = world.player;
	UpdateCounts(world.agents);

	StayAlive();

	if (!actions_.empty()) {
		if (current_action_ == nullptr) {
			printf("starting action\n");
			current_action_ = actions_.front();
			current_action_->Perform(world);
		}

		current_action_->Update(world);

		if (current_action_->Done(world)) {
			printf("action done\n");
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
	} else if (gwca_.IsSkillRecharged(SkillSlot::Shroud)) {
		if (gwca_.GetEffectTimeRemaining(GwConstants::SkillID::Shroud_of_Distress) == 0
			&& proximity_count_ > 0) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (player_.HP < 0.6) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (adjacent_count_ > 20) {
			UseSkillEx(SkillSlot::Shroud);
		}
	} else if (gwca_.IsSkillRecharged(SkillSlot::WayOfPerf)) {
		if (player_.HP < 0.5) {
			UseSkillEx(SkillSlot::WayOfPerf);
		} else if (adjacent_count_ > 20) {
			UseSkillEx(SkillSlot::WayOfPerf);
		}
	} else if (gwca_.IsSkillRecharged(SkillSlot::Channeling)) {
		if (area_count_ > 5 && gwca_.GetEffectTimeRemaining(GwConstants::SkillID::Channeling) < 2000) {
			UseSkillEx(SkillSlot::Channeling);
		}
	}
}

bool SmartVaettirBot::CheckUseSF() {
	if (gwca_.IsSkillRecharged(SkillSlot::SF) && proximity_count_ > 0) {
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
	if (player_.GetIsDead()) return false;
	if (!gwca_.IsSkillRecharged(slot)) return false;
	if (player_.GetEnergy() < SkillCost[slot]) return false;

	gwca_.UseSkill(slot, target, false);
	do {
		Sleep(50); // warning: no deadlock check. Bot could die and we're stuck here.
	} while (gwca_.IsSkillRecharged(slot));
	return true;
}
