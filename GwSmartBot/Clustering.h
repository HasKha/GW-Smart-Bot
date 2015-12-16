#pragma once

#include <vector>
#include <set>
#include <map>

#include "Structures.h"

struct Cluster {
	bool IsInRange(const World& world) const;

	std::set<long> agents;
};

class Clustering {
	static const int MAX_AGENTS = 300;

public:
	Clustering() : clustered_(MAX_AGENTS, false) {}

	void ComputeClusters(const World& world);

	void RenderClusters(const World& world);

	void ClearClusters();

	const std::vector<Cluster*>& clusters() { return clusters_; }

private:
	std::vector<Cluster*> clusters_;
	std::vector<bool> clustered_;
};
