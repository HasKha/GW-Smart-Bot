#pragma once

#include <GWCACppClient\GWCAClient.h>

struct World {
	PseudoAgent player;
	std::vector<AgentPosition> agents;
};
