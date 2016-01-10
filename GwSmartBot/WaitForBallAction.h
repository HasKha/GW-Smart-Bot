#pragma once

#include "Action.h"
#include "Utils.h"

class WaitForBallAction : public Action {
public:
	WaitForBallAction() {}

	void Perform(const World& world) override {
	}

	bool Done(const World& world) override {
		for (const AgentPosition& a : world.agents()) {
			float distance = Utils::GetDistance(world.player(), a);
			if (distance < 1400) {
				// if aggroed, it has to be melee
				if (distance > GwConstants::Range::Nearby) {
					return false;
				}
			}
		}
		printf("done waiting\n");
		return true;
	}
};
