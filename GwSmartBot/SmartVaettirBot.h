#pragma once

#include <vector>
#include <queue>

#include "Bot.h"
#include "Actions.h"

class SmartVaettirBot : public Bot {
public:
	SmartVaettirBot();

	void Update(World& world) override;

private:
	void StayAlive();
	void UpdateCounts(std::vector<AgentPosition> agents);

	// returns true if it used sf
	bool CheckUseSF();

	// uses skill, waits until used, returns true if successful
	bool UseSkillEx(int slot, int target = Target::Self);

	int proximity_count_;
	int spellcast_count_;
	int area_count_;
	int adjacent_count_;

	PseudoAgent& player_;
	std::queue<Action*> actions_;
	Action* current_action_;
};
