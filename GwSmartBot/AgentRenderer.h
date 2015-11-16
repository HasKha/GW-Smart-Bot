#pragma once

#include <vector>

#include <GWCACppClient\GWCAClient.h>

class AgentRenderer {
public:
	void RenderAgents(std::vector<AgentPosition> agents);
	void RenderPlayer(PseudoAgent& player);
};
