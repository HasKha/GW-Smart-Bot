#include "SmartVaettirBot.h"

#include <GWCA\GWCA\GwConstants.h>

const int SmartVaettirBot::SkillCost[] = {
	15, 5, 15, 5, 5, 5, 10, 5
};

void SmartVaettirBot::Update(PseudoAgent* player, std::vector<AgentPosition> agents) {
	if (player == nullptr) return;

	player_ = player;
	UpdateCounts(agents);

	StayAlive();
}

void SmartVaettirBot::StayAlive() {
	CheckUseSF();

	if (gwca_.IsSkillRecharged(SkillSlot::Shroud)) {
		if (gwca_.GetEffectTimeRemaining(GwConstants::SkillID::Shroud_of_Distress) == 0) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (player_->HP < 0.6) {
			UseSkillEx(SkillSlot::Shroud);
		} else if (adjacent_count_ > 20) {
			UseSkillEx(SkillSlot::Shroud);
		}
	}

	CheckUseSF();

	if (gwca_.IsSkillRecharged(SkillSlot::WayOfPerf)) {
		if (player_->HP < 0.5) {
			UseSkillEx(SkillSlot::WayOfPerf);
		} else if (adjacent_count_ > 20) {
			UseSkillEx(SkillSlot::WayOfPerf);
		}
	}

	CheckUseSF();

	if (gwca_.IsSkillRecharged(SkillSlot::Channeling)) {
		if (area_count_ > 5 && gwca_.GetEffectTimeRemaining(GwConstants::SkillID::Channeling) < 2000) {
			UseSkillEx(SkillSlot::Channeling);
		}
	}

	CheckUseSF();
}

void SmartVaettirBot::CheckUseSF() {
	if (gwca_.IsSkillRecharged(SkillSlot::SF) && proximity_count_ > 0) {
		UseSkillEx(SkillSlot::Paradox);
		UseSkillEx(SkillSlot::SF);
	}
}

void SmartVaettirBot::UpdateCounts(std::vector<AgentPosition> agents) {
	proximity_count_ = 0;
	spellcast_count_ = 0;
	area_count_ = 0;
	adjacent_count_ = 0;

	for (AgentPosition agent : agents) {
		float distance = GetSquaredDistance(player_->X, player_->Y, agent.x, agent.y);
		if (distance < 1200 * 1200) {
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
	if (player_->GetIsDead()) return;
	if (!gwca_.IsSkillRecharged(slot)) return;
	if (player_->GetEnergy() < SkillCost[slot]) return;

	gwca_.UseSkill(slot, target, false);
	do {
		Sleep(50); // warning: no deadlock check. Bot could die and we're stuck here.
	} while (gwca_.IsSkillRecharged(slot));
}
