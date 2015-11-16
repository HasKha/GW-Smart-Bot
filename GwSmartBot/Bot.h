#pragma once

#include <vector>
#include <GWCACppClient\GWCAClient.h>
#include "Structures.h"

class Bot {
public:
	Bot(GWCAClient& gwca) : gwca_(gwca) {}

	virtual void Update(World& world) = 0;

protected:
	GWCAClient& gwca_;
};
