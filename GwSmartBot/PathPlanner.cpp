#include "PathPlanner.h"

#include <SDL2\SDL_opengl.h>
#include <algorithm>

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
		for (PathNode pn : path_) {
			const AgentPosition ap = world.GetAgentByID(pn.agentid);
			if (ap.valid()) {
				glVertex2f(ap.x, ap.y);
			}
		}
		glEnd();
	}
}

void PathPlanner::Clear() {
	clustering.ClearClusters();
}

void PathPlanner::PlanPath(const World& world) {
	path_.clear();

	path_.push_back(PathNode(nullptr, world.player().Id));

	std::vector<Cluster*> clusters = clustering.clusters();

	while (!clusters.empty()) {
		PathNode nearest = Nearest(world, clusters, world.GetAgentByID(path_.back().agentid));
		if (!nearest.valid()) break;
		
		path_.push_back(nearest);
		clusters.erase(std::remove(clusters.begin(), clusters.end(), nearest.cluster));
	}
}

PathNode PathPlanner::Nearest(const World& world, std::vector<Cluster*> clusters, 
	const AgentPosition agent) {
	PathNode node;
	float min_dist = -1;
	for (Cluster* cluster : clusters) {
		for (long id : cluster->agents) {
			const AgentPosition other = world.GetAgentByID(id);
			float curr_dist = Utils::GetSquaredDistance(agent, other);
			if (min_dist > curr_dist || min_dist < 0) {
				min_dist = curr_dist;
				node.agentid = id;
				node.cluster = cluster;
			}
		}
	}
	return node;
}
