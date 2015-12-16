#include "PathPlanner.h"

#include <algorithm>
#include <assert.h>

#include <SDL2\SDL_opengl.h>

#include "Utils.h"

void PathPlanner::Update(const World& world) {
	clustering.ComputeClusters(world);

	int count = 0;
	for (Cluster* c : clustering.clusters()) {
		if (c->IsInRange(world)) ++count;
	}
	
	PlanPath(world);
}

void PathPlanner::Render(const World& world) {
	clustering.RenderClusters(world);

	if (!path_.empty()) {
		glColor3f(0, 0, 1);
		glBegin(GL_LINE_STRIP);
		for (PathNode* pn : path_) {
			if (pn->valid(world)) {
				Point2f pos = pn->Pos(world);
				glVertex2f(pos.x(), pos.y());
			}
		}
		glEnd();
	}
}

void PathPlanner::Clear() {
	clustering.ClearClusters();
}

void PathPlanner::PlanPath(const World& world) {
	ConstructPath(world);
	OptimizeAgentInCluster(world);
}

std::vector<Cluster*> PathPlanner::GetFilteredClusters(const World& world) {
	std::vector<Cluster*> filtered;
	for (Cluster* cluster : clustering.clusters()) {
		bool valid = true;
		for (long id : cluster->agents) {
			if (world.GetAgentByID(id).y > NORTH_SOUTH_LINE) {
				valid = false;
				break;
			}
		}
		if (valid) {
			filtered.push_back(cluster);
		}
	}
	return filtered;
}

void PathPlanner::ConstructPath(const World& world) {
	for (PathNode* pn : path_) delete pn;
	path_.clear();

	path_.push_back(new PlayerPathNode());

	std::vector<Cluster*> clusters = GetFilteredClusters(world);

	while (!clusters.empty()) {
		PathNode* nearest = Nearest(world, clusters, path_.back()->Pos(world));
		if (!nearest->valid(world)) break;
		
		path_.push_back(nearest);
		clusters.erase(std::remove(clusters.begin(), clusters.end(), nearest->cluster()));
	}

	path_.push_back(new FixedPathNode(destination_));
}

void PathPlanner::OptimizeAgentInCluster(const World& world) {
	// start from 2nd, 1st is player pos and we can't optimize that
	// end at second last, we can't optimize last since it has no next
	bool changed = false;
	for (int j = 0; j < 3; ++j) {
		for (size_t i = 1; i < path_.size() - 1; ++i) {
			Cluster* cluster = path_[i]->cluster();
			Point2f prev = path_[i - 1]->Pos(world);
			Point2f next = path_[i + 1]->Pos(world);
			float best_dist = -1;
			long best_id = -1;
			for (long id : cluster->agents) {
				AgentPosition agent = world.GetAgentByID(id);
				float before = Utils::GetSquaredDistance(agent, prev);
				float after = Utils::GetSquaredDistance(agent, next);
				float curr_dist = before + after;
				if (best_dist > curr_dist || best_dist < 0) {
					best_dist = curr_dist;
					best_id = id;
				}
			}
			assert(best_id > 0);
			delete path_[i];
			path_[i] = new AgentPathNode(best_id, cluster);
		}
	}
}

PathNode* PathPlanner::Nearest(const World& world, std::vector<Cluster*> clusters, 
	const Point2f pos) {
	long nearest_id = -1;
	Cluster* nearest_cluster = nullptr;
	float min_dist = -1;
	for (Cluster* cluster : clusters) {
		for (long id : cluster->agents) {
			const AgentPosition other = world.GetAgentByID(id);
			float curr_dist = Utils::GetSquaredDistance(other, pos);
			if (min_dist > curr_dist || min_dist < 0) {
				min_dist = curr_dist;
				nearest_id = id;
				nearest_cluster = cluster;
			}
		}
	}
	return new AgentPathNode(nearest_id, nearest_cluster);
}
