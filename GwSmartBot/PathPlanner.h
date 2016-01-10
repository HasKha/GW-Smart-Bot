#pragma once

#include <GWCACppClient\GWCAClient.h>
#include <vector>
#include <functional>

#include "Clustering.h"
#include "Point2f.h"
#include "PathingMap.h"

class PathNode {
protected:
	PathNode() : cluster_(nullptr) {}
	PathNode(const Cluster* cluster) : cluster_(cluster) {}

public:
	virtual Point2f Pos(const World& world) = 0;
	virtual bool valid(const World& world) = 0;

	const Cluster* cluster() { return cluster_; }

private:
	const Cluster* cluster_;
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
		GWCA::GW::GamePos pos = world.player().pos;
		return Point2f(pos.x, pos.y);
	}
	bool valid(const World&) override { return true; }
};

class AgentPathNode : public PathNode {
public:
	AgentPathNode(long id, const Cluster* cluster) : 
		agentid(id), PathNode(cluster) {}
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
	const float TARGET_DISTANCE = 900.0f;
public:
	PathPlanner(PathingMap& pmap) :
		validator_([](AgentPosition) { return true; }),
		destination_(0, 0),
		pmap_(pmap) {}

	void Update(const World& world);

	void Render(const World& world) const;

	void Clear();

	void SetFinalDestination(const Point2f p) { destination_ = p; }
	void SetValidatorFunc(std::function<bool(AgentPosition apos)> f) {
		validator_ = f;
	}

	Point2f GetFinalDestination() const { return destination_; }
	Point2f GetNextDestination(const World& world) const { 
		return path_[1]->Pos(world); }

private:
	PathNode* Nearest(const World& world, std::vector<Cluster*> clusters, 
		const Point2f pos);
	
	void PlanPath(const World& world);
	std::vector<Cluster*> GetFilteredClusters(const World& world);
	void ConstructPath(const World& world);
	void OptimizeAgentInCluster(const World& world);
	void OptimizePosition(const World& world);

	float cross(const Point2f v, const Point2f w) const {
		return v.x() * w.y() - v.y() * w.x();
	}

	PathingMap& pmap_;

	Clustering clustering;

	Point2f destination_;
	std::function<bool(AgentPosition apos)> validator_;

	std::vector<PathNode*> path_;
};
