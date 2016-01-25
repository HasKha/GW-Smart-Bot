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
		if (world.player().GetIsDead()) {
			printf("cannot cast %d, I'm dead\n", slot_);
			failed_ = true;
		} else if (!GWCAClient::Api().IsSkillRecharged(slot_)) {
			printf("cannot cast %d, it's not recharged\n", slot_);
			failed_ = true;
		} else if (world.player().GetEnergy() < cost_) {
			printf("cannot cast %d, no energy for it\n", slot_);
			failed_ = true;
		} else {
			GWCAClient::Api().UseSkill(slot_, target_, false);
			failed_ = false;
		}
	}

	bool Done(const World& world) override {
		if (failed_) return true;
		if (casting_) {
			if (world.player().GetIsDead()) return true;
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