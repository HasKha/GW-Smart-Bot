#include "PathPlanner.h"

void PathPlanner::Update(const World& world) {
	clustering.ComputeClusters(world);

	int count = 0;
	for (const Cluster& c : clustering.clusters()) {
		if (c.IsInRange(world)) ++count;
	}
	printf("clusters: %d\n", count);
}

void PathPlanner::Render(const World& world) {
	clustering.RenderClusters(world);
}

void PathPlanner::Clear() {
	clustering.ClearClusters();
}
