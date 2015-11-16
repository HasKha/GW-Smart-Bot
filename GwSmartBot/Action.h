#pragma once

#include <GWCACppClient\GWCAClient.h>

#include "Structures.h"

class Action {
public:
	Action(GWCAClient& gwca) : gwca_(gwca) {}

	// perform the initial action, will be called once per action
	virtual void Perform(World& world) = 0;

	// check for status, will be called once per loop
	virtual void Update(World& world) = 0;

	// checks if the action is done, should return true when completed
	virtual bool Done(World& world) = 0;

protected:
	GWCAClient& gwca() { return gwca_; }

private:
	GWCAClient& gwca_;
};
