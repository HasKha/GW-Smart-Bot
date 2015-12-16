#pragma once

#include <GWCACppClient\GWCAClient.h>

class World {
public:
	// getters
	const PseudoAgent& player() const { return player_; }

	const std::vector<AgentPosition>& agents() const { return agents_; }

	const AgentPosition GetAgentByID(long id) const {
		if (player_.Id == id) {
			return AgentPosition(player_.Id, player_.X, player_.Y);
		} else {
			return agents_by_id_[id];
		}
	}

	// setters
	void Update() {
		player_ = GWCAClient::Api().GetPlayer();
		agents_ = GWCAClient::Api().GetAgentsPos();
		ComputeAgentsByID();
	}

private:
	void ComputeAgentsByID() {
		long oldsize = agents_by_id_.size();
		long maxid = 0;
		if (!agents_.empty()) maxid = agents_.back().id;
		long newsize = max(oldsize, maxid + 1);

		agents_by_id_.clear();
		agents_by_id_ = std::vector<AgentPosition>(newsize, AgentPosition::Invalid());
		for (const AgentPosition agent : agents_) {
			agents_by_id_[agent.id] = agent;
		}
	}

	PseudoAgent player_;
	std::vector<AgentPosition> agents_;
	std::vector<AgentPosition> agents_by_id_;
};
