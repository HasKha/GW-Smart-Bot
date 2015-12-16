#pragma once

#include <GWCACppClient\GWCAClient.h>
#include <vector>

#include "Clustering.h"

struct PathNode {
	PathNode() : cluster(nullptr), agentid(-1) {}
	PathNode(Cluster* _cluster, long _agentid) :
		cluster(_cluster), agentid(_agentid) {}
	Cluster* cluster;
	long agentid;

	bool valid() { return agentid != -1 && cluster != nullptr; }
};

class PathPlanner {
public:
	void Update(const World& world);

	void Render(const World& world);

	void Clear();

private:
	PathNode Nearest(const World& world, std::vector<Cluster*> clusters, 
		const AgentPosition agent);
	void PlanPath(const World& world);

	std::vector<PathNode> path_;
	Clustering clustering;
};
