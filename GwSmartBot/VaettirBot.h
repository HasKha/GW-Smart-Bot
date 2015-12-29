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

	// will be called at every iteration
	virtual void Update(bool do_actions) = 0;

	// render if needed
	virtual void Render() = 0;

	// will be called on map change or reset
	virtual void Clear() = 0;

protected:
	GWCAClient& gwca() { return GWCAClient::Api(); }

	void UpdateCounts();
	void StayAlive();
	
	// returns true if it used sf
	bool CheckUseSF();

	void UseSkillEx(int slot, int target = Target::Self);

	int proximity_count_;
	int spellcast_count_;
	int area_count_;
	int adjacent_count_;

	ActionQueue cast_queue_;
	ActionQueue action_queue_;

	const World& world_;
};
