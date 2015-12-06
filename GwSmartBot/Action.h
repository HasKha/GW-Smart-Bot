#pragma once

#include <functional>
#include "Structures.h"

class Action {
public:
	// perform the initial action, will be called once per action
	virtual void Perform(const World& world) = 0;

	// check for status, will be called once per loop
	virtual void Update(const World& world) = 0;

	// checks if the action is done, should return true when completed
	virtual bool Done(const World& world) = 0;
};

// an action that happens only once
class SimpleAction : public Action {
public:
	void Update(const World&) final override {}
	bool Done(const World&) final override { return true; }
};

class CustomSimpleAction : public SimpleAction {
public:
	CustomSimpleAction(std::function<void(const World&)> func) : func_(func) {}
	void Perform(const World& world) { func_(world); }

private:
	std::function<void(const World&)> func_;
};