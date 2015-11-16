#pragma once

#include <functional>
#include "Structures.h"

class Action {
public:
	// perform the initial action, will be called once per action
	virtual void Perform(World& world) = 0;

	// check for status, will be called once per loop
	virtual void Update(World& world) = 0;

	// checks if the action is done, should return true when completed
	virtual bool Done(World& world) = 0;
};

// an action that happens only once
class SimpleAction : public Action {
public:
	void Update(World&) final override {}
	bool Done(World&) final override { return true; }
};

class CustomSimpleAction : public SimpleAction {
public:
	CustomSimpleAction(std::function<void(World&)> func) : func_(func) {}
	void Perform(World& world) { func_(world); }

private:
	std::function<void(World&)> func_;
};