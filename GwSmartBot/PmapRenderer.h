#pragma once

#include <vector>

#include "PathingMap.h"

class PmapRenderer {
public:
	PmapRenderer() {}

	bool Initialize(PathingMap& pmap);

	void Render();

private:
	std::vector<PathingMapTrapezoid> trapezoids_;
};
