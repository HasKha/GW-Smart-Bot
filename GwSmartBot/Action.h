#pragma once

#include <functional>
#include "Structures.h"

class Action {
public:
	// perform the initial action, will be always called once per action
	virtual void Perform(const World& world) = 0;

	// resume the action after a pause, a different action, or a sub-action
	// e.g. resume moving after casting
	virtual void Resume(const World& world) {}

	// check for status, will be called once per loop
	virtual void Update(const World& world) {};

	// checks if the action is done, must return true when completed
	virtual bool Done(const World& world) { return true; };
};

// an action that happens only once, 
// the action is passed as a std::func
class CustomSimpleAction : public Action {
public:
	CustomSimpleAction(std::function<void(const World&)> func) : func_(func) {}
	void Perform(const World& world) { func_(world); }

private:
	std::function<void(const World&)> func_;
};
