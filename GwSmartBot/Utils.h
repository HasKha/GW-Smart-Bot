#pragma once

#include <cmath>
#include <GWCACppClient\GWCAClient.h>

// a collection of static utility methods
class Utils {
public:
	static float GetSquaredDistance(float x1, float y1, float x2, float y2) {
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	}

	static float GetDistance(float x1, float y1, float x2, float y2) {
		return std::sqrt(GetSquaredDistance(x1, y1, x2, y2));
	}

	static float GetDistance(PseudoAgent& a1, AgentPosition& a2) {
		return GetDistance(a1.X, a1.Y, a2.x, a2.y);
	}

	static float GetSquaredDistance(PseudoAgent& a1, AgentPosition& a2) {
		return GetSquaredDistance(a1.X, a1.Y, a2.x, a2.y);
	}

	static float GetDistance(AgentPosition& a1, AgentPosition& a2) {
		return GetDistance(a1.x, a1.y, a2.x, a2.y);
	}

	static float GetSquaredDistance(AgentPosition& a1, AgentPosition& a2) {
		return GetSquaredDistance(a1.x, a1.y, a2.x, a2.y);
	}
};
