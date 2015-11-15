#pragma once

#include <vector>

#include "PathingMap.h"

class PmapRenderer {
public:
	PmapRenderer() {}

	bool Initialize(unsigned int map_hash);

	void Render();

private:
	std::vector<PathingMapTrapezoid> trapezoids_;
};
