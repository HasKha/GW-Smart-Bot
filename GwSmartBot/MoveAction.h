#pragma once

#include <GWCACppClient\GWCAClient.h>

#include "Action.h"

#include "Utils.h"
#include "Constants.h"

class MoveAction : public Action {
public:
	MoveAction(float x, float y) : x_(x), y_(y), blocked_(0) {}
	
	void Perform(const World& world) override {
		GWCAClient::Api().Move(x_, y_);
	}

	void Update(const World& world) override {
		if (world.player.MoveX == 0 && world.player.MoveY == 0) {
			printf("not moving! blocked = %d\n", blocked_);

			++blocked_;

			if (blocked_ < 15) {
				Perform(world);
				Sleep(50);
			} else {
				if (GWCAClient::Api().IsSkillRecharged(SkillSlot::HoS)) {
					GWCAClient::Api().UseSkill(SkillSlot::HoS, Target::Self);
					blocked_ = 0;
				}
			}
		} else {
			blocked_ = 0;
		}
	}

	bool Done(const World& world) override {
		return Utils::GetSquaredDistance(world.player.X, world.player.Y, x_, y_) < (200 * 200);
	}

private:
	int blocked_;
	float x_;
	float y_;
};
