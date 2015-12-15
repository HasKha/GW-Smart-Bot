#pragma once

#include <GWCACppClient\GWCAClient.h>

#include "Clustering.h"

class PathPlanner {
public:
	void Update(const World& world);

	void Render(const World& world);

	void Clear();

private:
	Clustering clustering;
};
