#include "VaettirBot.h"

#include <GWCA\GWCA\GwConstants.h>
#include "Constants.h"
#include "Utils.h"

void VaettirBot::StayAlive() {
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

bool VaettirBot::CheckUseSF() {
	if (gwca().IsSkillRecharged(SkillSlot::SF) && proximity_count_ > 0) {
		UseSkillEx(SkillSlot::Paradox);
		UseSkillEx(SkillSlot::SF);
		return true;
	} else {
		return false;
	}
}

void VaettirBot::UpdateCounts() {
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

void VaettirBot::UseSkillEx(int slot, int target) {
	cast_queue_.Append(new UseSkillAction(slot, target, SkillCost[slot]));
}
