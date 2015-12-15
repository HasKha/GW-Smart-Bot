#pragma once

#include <vector>

#include <GWCACppClient\GWCAClient.h>

class AgentRenderer {
public:
	void RenderAgents(const std::vector<AgentPosition>& agents);
	void RenderPlayer(const PseudoAgent& player);
};
