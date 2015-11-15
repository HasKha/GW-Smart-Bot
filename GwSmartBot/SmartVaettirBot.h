#pragma once

#include <vector>

#include "Bot.h"

class SmartVaettirBot : public Bot {
	enum SkillSlot {
		Paradox,
		SF,
		Echo,
		HoS,
		Wastrel,
		Channeling,
		Shroud,
		WayOfPerf
	};
	static const int SkillCost[];

public:
	SmartVaettirBot(GWCAClient& gwca) : Bot(gwca) {}

	void Update(PseudoAgent* player, 
		std::vector<AgentPosition> agents) override;

private:
	void StayAlive();
	void UpdateCounts(std::vector<AgentPosition> agents);

	void CheckUseSF();

	void UseSkillEx(int slot, int target = Target::Self);

	float GetSquaredDistance(float x1, float y1, float x2, float y2) {
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	}

	int proximity_count_;
	int spellcast_count_;
	int area_count_;
	int adjacent_count_;

	PseudoAgent* player_;
};
