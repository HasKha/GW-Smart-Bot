#pragma once

#include <vector>

#include "Bot.h"

enum Skill {
	Paradox,
	SF,
	Echo,
	HoS,
	Wastrel,
	Channeling,
	Shroud,
	WayOfPerf
};

class SmartVaettirBot : public Bot {
public:
	SmartVaettirBot(GWCAClient& gwca) : Bot(gwca) {}

	void Update(PseudoAgent& player, 
		std::vector<AgentPosition> agents) override;

private:
	void StayAlive(PseudoAgent& player,
		std::vector<AgentPosition> agents);

	float GetSquaredDistance(float x1, float y1, float x2, float y2) {
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	}

	int proximity_count;
	int spellcast_count;
	int area_count;
	int adjacent_count;
};
