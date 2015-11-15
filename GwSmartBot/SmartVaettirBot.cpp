#include "SmartVaettirBot.h"

#include <GWCA\GWCA\GwConstants.h>

void SmartVaettirBot::Update(PseudoAgent& player, std::vector<AgentPosition> agents) {
	int recharge = gwca_.GetSkillRecharge(Skill::SF);
	printf("recharge %d\n", recharge);
}

void SmartVaettirBot::StayAlive(PseudoAgent& player, std::vector<AgentPosition> agents) {
	proximity_count = spellcast_count = area_count = adjacent_count = 0;

	for (AgentPosition agent : agents) {
		float distance = GetSquaredDistance(player.X, player.Y, agent.x, agent.y);
		if (distance < 1200 * 1200) {
			++proximity_count;
			if (distance < GwConstants::SqrRange::Spellcast) {
				++spellcast_count;
				if (distance < GwConstants::SqrRange::Area) {
					++area_count;
					if (distance < GwConstants::SqrRange::Adjacent) {
						++adjacent_count;
					}
				}
			}
		}
	}
}
