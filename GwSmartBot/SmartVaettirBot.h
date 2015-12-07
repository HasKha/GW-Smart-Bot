#pragma once

#include <vector>
#include <queue>

#include "Actions.h"

class SmartVaettirBot {
public:
	SmartVaettirBot(const World& world);
	
	void Update();

private:
	GWCAClient gwca() { return GWCAClient::Api(); }

	void StayAlive();
	void UpdateCounts();

	// returns true if it used sf
	bool CheckUseSF();

	// uses skill, waits until used, returns true if successful
	void UseSkillEx(int slot, int target = Target::Self);

	int proximity_count_;
	int spellcast_count_;
	int area_count_;
	int adjacent_count_;

	const World& world_;

	Action* current_action_;
	std::queue<Action*> action_queue_;
	UseSkillAction* current_cast_;
	std::queue<UseSkillAction*> cast_queue_;
	
};
