#pragma once

#include <vector>
#include "Point2f.h"

class RangeRenderer {
public:
	void Initialize();
	void Render(float x, float y);

private:
	std::vector<Point2f> aggro;
	std::vector<Point2f> radar;
};
