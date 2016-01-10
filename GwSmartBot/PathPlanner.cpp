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

void PathPlanner::Render(const World& world) const {
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
		glColor3f(0.7f, 0.2f, 1);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
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
	OptimizePosition(world);
}

std::vector<Cluster*> PathPlanner::GetFilteredClusters(const World& world) {
	std::vector<Cluster*> filtered;
	for (Cluster* cluster : clustering.clusters()) {
		//bool north = false;
		bool valid = false;
		for (long id : cluster->agents) {
			AgentPosition apos = world.GetAgentByID(id);
			if (apos.valid() && validator_(apos)) {
				valid = true;
				//
				//if (apos.y > NORTH_SOUTH_LINE) {
				//	north = true;
				//	break;
				//}
			}
		}
		if (valid /*&& !north*/) {
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
	for (int j = 0; j < 3; ++j) {
		for (size_t i = 1; i < path_.size() - 1; ++i) {
			const Cluster* cluster = path_[i]->cluster();
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

void PathPlanner::OptimizePosition(const World& world) {
	for (size_t i = 1; i < path_.size() - 1; ++i) {
		Point2f prev = path_[i - 1]->Pos(world);
		Point2f curr = path_[i    ]->Pos(world);
		Point2f next = path_[i + 1]->Pos(world);

		Point2f to_prev = (prev - curr).normalized();
		Point2f to_next = (next - curr).normalized();
		Point2f middle = ((to_prev + to_next) / 2).normalized();
		Point2f result;

		// http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		Point2f p = curr;
		Point2f q = prev;
		Point2f s = next - prev;
		Point2f r = middle * TARGET_DISTANCE;
		if (cross(r, s) == 0) { // colinear or parallel lines, extremely unlikely
								// but consider to prevent division by 0
			result = curr;
		} else {
			// lines are not colinear, not parallel, they intersect at p + t r = q + u s
			// if t < 1 the intersection is inside earshot
			float t = cross(q - p, s) / cross(r, s);
			if (t < 1) {
				result = p + t * r;
			} else {
				result = p + 1 * r;
			}
		}

		if (pmap_.Contains(result.x(), result.y())) {
			delete path_[i];
			path_[i] = new FixedPathNode(result);
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
