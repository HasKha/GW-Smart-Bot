#pragma once

#include "VaettirBot.h"
#include "PathPlanner.h"
#include "PathingMap.h"

class SmartBot : public VaettirBot {
public:
	SmartBot(const World& world, PathingMap& pmap);

	void Update(bool do_actions) override;

	void Render() override {
		path_planner_.Render(world_);
	}

	void Clear() override {
		path_planner_.Clear();
	}

private:
	PathPlanner path_planner_;
};
