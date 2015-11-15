#pragma once

#include <GWCACppClient\GWCAClient.h>

class Bot {
public:
	Bot(GWCAClient& gwca) : gwca_(gwca) {}

	virtual void Update(PseudoAgent& player, 
		std::vector<AgentPosition> agents) = 0;

protected:
	GWCAClient& gwca_;
};
