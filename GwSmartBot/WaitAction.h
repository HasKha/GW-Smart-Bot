#pragma once

#include <ctime>

#include "Action.h"

class WaitAction : public Action {
public:
	// wait for a set amount of time (in milliseconds)
	WaitAction(long time) : time_(time) {}

	// perform the initial action, will be called once per action
	void Perform(const World& world) override {
		start_ = std::clock();
	}

	// checks if the action is done, should return true when completed
	bool Done(const World& world) override {
		long elapsed = std::clock() - start_;
		return elapsed > time_;
	}

private:
	std::clock_t start_;
	long time_;
};
