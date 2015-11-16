#pragma once

#include <string>

#include "Action.h"
#include "MoveAction.h"
#include "WaitAction.h"

class LogAction : public SimpleAction {
public:
	LogAction(std::string message) : message_(message) {}

	void Perform(World& ) override {
		printf("%s\n", message_.c_str());
	}

private:
	const std::string message_;
};

class UseSkillAction : public SimpleAction {
public:
	UseSkillAction(int slot, int target) : slot_(slot), target_(target) {}

	void Perform(World&) override {
		GWCAClient::Api().UseSkill(slot_, target_);
	}

private:
	const int slot_;
	const int target_;
};