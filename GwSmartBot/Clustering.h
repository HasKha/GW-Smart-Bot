#pragma once

#include <vector>
#include <set>
#include <map>

#include "Structures.h"

class Cluster {
public:
	void Insert(long agentid) { agents_.insert(agentid); }
	long Size() { return agents_.size(); }

	std::set<long>::const_iterator begin() const { return agents_.begin(); }
	std::set<long>::const_iterator end() const { return agents_.end(); }

	bool IsInRange(const World& world) const;

private:
	std::set<long> agents_;
};

class Clustering {
	static const int MAX_AGENTS = 300;

public:
	Clustering() : clustered_(MAX_AGENTS, false) {}

	void ComputeClusters(const World& world);

	void RenderClusters(const World& world);

	void ClearClusters();

	const std::vector<Cluster>& clusters() { return clusters_; }

private:
	std::vector<Cluster> clusters_;
	std::vector<bool> clustered_;
};
