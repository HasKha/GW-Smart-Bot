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

	void Report(const World& w) {
		DWORD time = GWCAClient::Api().GetInstanceTime();
		long seconds = time / 1000;
		long minutes = seconds / 60;
		printf("=== Run Done ===\n");
		printf("it took %d:%d (%d s)\n", minutes, seconds % 60, seconds);

		int count = 0;
		for (const AgentPosition& agent : w.agents()) {
			if (Utils::GetDistance(w.player(), agent) < 900) {
				++count;
			}
		}
		printf("grouped %d agents, missing %d from 60\n", count, 60 - count);
	}

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
