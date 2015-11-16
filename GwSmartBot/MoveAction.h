#pragma once

#include "Action.h"

#include "Utils.h"

class MoveAction : public Action {
public:
	MoveAction(GWCAClient& gwca, float x, float y) : Action(gwca), x_(x), y_(y) {}
	
	void Perform(World& world) override {
		printf("moving\n");
		gwca().Move(x_, y_);
	}

	void Update(World& world) override {
		// TODO: check if stuck, if so hos/move again
	}

	bool Done(World& world) override {
		return Utils::GetSquaredDistance(world.player.X, world.player.Y, x_, y_) < (200 * 200);
	}

private:
	float x_;
	float y_;
};
