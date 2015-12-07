#pragma once

#include <ctime>

#include "Action.h"

// note: this does NOT work with skills without a recharge time
class UseSkillAction : public Action {
public:
	UseSkillAction(const int slot, const int target, const int cost) :
		slot_(slot), target_(target), cost_(cost), failed_(false), casting_(true) {}
	UseSkillAction(const int slot, const int target) :
		UseSkillAction(slot, target, 0) {}

	void Perform(const World& world) override {
		if (world.player.GetIsDead()
			|| !GWCAClient::Api().IsSkillRecharged(slot_)
			|| world.player.GetEnergy() < cost_) {
			failed_ = true;
			printf("failed casting %d\n", slot_);
		} else {
			printf("casting %d\n", slot_);
			GWCAClient::Api().UseSkill(slot_, target_, false);
			printf("send cast command for %d\n", slot_);
		}
	}

	void Update(const World& world) override {
		if (!failed_ && casting_ && world.player.Skill == 0) {
			printf("warning: not casting %d\n", slot_);
		}
	}

	bool Done(const World& world) override {
		if (failed_) return true;
		if (casting_) {
			if (world.player.GetIsDead()) return true;
			if (failed_) return true;
			if (!GWCAClient::Api().IsSkillRecharged(slot_)) {
				casting_ = false;
				aftercast_start_ = std::clock();
			}
			return false;
		} else {
			long elapsed = std::clock() - aftercast_start_;
			return elapsed > 750;
		}
	}

private:
	const int slot_;
	const int target_;
	const int cost_;

	bool failed_;
	bool casting_;
	std::clock_t aftercast_start_;
};