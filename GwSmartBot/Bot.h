#pragma once

#include <vector>
#include <GWCACppClient\GWCAClient.h>
#include "Structures.h"

class Bot {
public:
	Bot() {}

	virtual void Update(const World& world) = 0;

protected:
	GWCAClient& gwca() { return GWCAClient::Api(); }
};
