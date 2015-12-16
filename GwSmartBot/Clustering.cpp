#include "Clustering.h"

#include <SDL2\SDL_opengl.h>

#include "Utils.h"

bool Cluster::IsInRange(const World& world) const {
	for (long id : agents) {
		if (Utils::GetSquaredDistance(world.player(),
			world.GetAgentByID(id)) > GwConstants::SqrRange::Compass) {
			return false;
		}
	}
	return true;
}

void Clustering::ComputeClusters(const World& world) {
	// compute list of unclustered agents
	std::vector<AgentPosition> unclustered;
	for (const AgentPosition& agent : world.agents()) {
		if (agent.id >= MAX_AGENTS) {
			printf("WARNING, agent id %d greater than MAX_AGENTS", agent.id);
			return;
		}

		if (!clustered_[agent.id]) {
			unclustered.push_back(agent);
		}
	}

	// construct cluster proposal
	while (unclustered.size() >= 4) {
		// start with a random one (last)
		AgentPosition agent = unclustered.back();
		unclustered.pop_back();

		Cluster* cluster = new Cluster();
		cluster->agents.insert(agent.id);

		for (int i = 0; i < 3; ++i) {
			size_t closest = 0;
			float closest_distance = Utils::GetSquaredDistance(agent, unclustered[closest]);
			for (size_t j = 1; j < unclustered.size(); ++j) {
				if (closest_distance > Utils::GetSquaredDistance(agent, unclustered[j])) {
					closest_distance = Utils::GetSquaredDistance(agent, unclustered[j]);
					closest = j;
				}
			}
			float max_distance = 750;
			if (closest_distance < max_distance * max_distance) {
				cluster->agents.insert(unclustered[closest].id);
				unclustered.erase(unclustered.begin() + closest);
			}
		}
		if (cluster->agents.size() == 4) {
			clusters_.push_back(cluster);
			printf("created cluster with ");
			for (long id : cluster->agents) {
				printf("%d ", id);
				clustered_[id] = true;
			}
			printf("\n");
		} else {
			delete cluster;
		}
	}

	// remove clusters that are already on top of the player
	for (auto it = clusters_.begin(); it != clusters_.end();) {
		bool remove = false;
		Cluster* cluster = *it;
		for (long agentid : cluster->agents) {
			if (Utils::GetSquaredDistance(world.player(), 
					world.GetAgentByID(agentid)) < GwConstants::SqrRange::Earshot) {
				remove = true;
				printf("in range to remove\n");
				break;
			}
		}
		if (remove) {
			printf("Removing cluster\n");
			delete cluster;
			it = clusters_.erase(it);
		} else {
			++it;
		}
	}
}

void Clustering::ClearClusters() {
	for (Cluster* c : clusters_) {
		delete c;
	}
	clusters_.clear();
	clustered_ = std::vector<bool>(MAX_AGENTS, false);
}


void Clustering::RenderClusters(const World& world) {
	glColor3f(0.8f, 0, 0);
	for (Cluster* cluster : clusters_) {
		glBegin(GL_LINE_LOOP);
		for (long agentid : cluster->agents) {
			const AgentPosition& ap = world.GetAgentByID(agentid);
			if (ap.valid()) {
				glVertex2f(ap.x, ap.y);
			}
		}
		glEnd();
	}
}
