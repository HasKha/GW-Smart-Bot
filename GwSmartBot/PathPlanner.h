#pragma once

#include <GWCACppClient\GWCAClient.h>
#include <vector>

#include "Clustering.h"
#include "Point2f.h"

class PathNode {
public:
	PathNode() : cluster_(nullptr) {}
	PathNode(Cluster* cluster) : cluster_(cluster) {}

	virtual Point2f Pos(const World& world) = 0;
	virtual bool valid(const World& world) = 0;

	Cluster* cluster() { return cluster_; }

private:
	Cluster* cluster_;
};

class FixedPathNode : public PathNode {
public:
	FixedPathNode(Point2f pos) : pos_(pos) {}
	Point2f Pos(const World&) override { return pos_; }
	bool valid(const World&) override { return true; }
private:
	Point2f pos_;
};

class PlayerPathNode : public PathNode {
public:
	Point2f Pos(const World& world) override {
		GWAPI::GW::GamePos pos = world.player().pos;
		return Point2f(pos.x, pos.y);
	}
	bool valid(const World&) override { return true; }
};

class AgentPathNode : public PathNode {
public:
	AgentPathNode(long id, Cluster* cluster) : agentid(id), PathNode(cluster) {}
	Point2f Pos(const World& world) override {
		AgentPosition pos = world.GetAgentByID(agentid);
		return Point2f(pos.x, pos.y);
	}

	bool valid(const World& world) override { 
		return world.GetAgentByID(agentid).valid();
	}

private:
	long agentid;
};

class PathPlanner {
	static const int NORTH_SOUTH_LINE = -19930;
public:
	void Update(const World& world);

	void Render(const World& world);

	void Clear();

	void SetFinalDestination(const Point2f p) { destination_ = p; }

	Point2f GetNextDestination(const World& world) { 
		return path_[1]->Pos(world); }

private:
	PathNode* Nearest(const World& world, std::vector<Cluster*> clusters, 
		const Point2f pos);
	
	void PlanPath(const World& world);
	std::vector<Cluster*> GetFilteredClusters(const World& world);
	void ConstructPath(const World& world);
	void OptimizeAgentInCluster(const World& world);
	void OptimizePosition(const World& world);

	float cross(const Point2f v, const Point2f w) {
		return v.x() * w.y() - v.y() * w.x();
	}

	Clustering clustering;

	Point2f destination_;
	std::vector<PathNode*> path_;
};
