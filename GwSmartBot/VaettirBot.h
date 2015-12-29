#pragma once

#include <vector>
#include <queue>
#include <GWCACppClient\GWCAClient.h>

#include "Structures.h"
#include "Actions.h"
#include "ActionQueue.h"

class VaettirBot {
public:
	VaettirBot(const World& world) :
		world_(world) {}

	virtual void Update() = 0;

protected:
	GWCAClient& gwca() { return GWCAClient::Api(); }

	void CastIfNeeded();
	void UpdateCounts();
	void StayAlive();
	
	// returns true if it used sf
	bool CheckUseSF();

	void UseSkillEx(int slot, int target = Target::Self);

	int proximity_count_;
	int spellcast_count_;
	int area_count_;
	int adjacent_count_;

	//UseSkillAction* current_cast_;
	//std::queue<UseSkillAction*> cast_queue_;
	ActionQueue cast_queue_;
	const World& world_;
};
