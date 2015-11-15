#include "RangeRenderer.h"

#include <SDL2\SDL_opengl.h>
#include <GWCA\GWCA\GwConstants.h>

#define _USE_MATH_DEFINES
#include <math.h> // for M_PI
#include <cmath>  // for cos, sin

void RangeRenderer::Initialize() {
	int n_points = 48;
	float radius;

	radius = GwConstants::Range::Compass;
	for (int i = 0; i < n_points; ++i) {
		float angle = i * (2 * static_cast<float>(M_PI) / n_points);
		float x = radius * std::cos(angle);
		float y = radius * std::sin(angle);
		radar.push_back(Point2f(x, y));
	}

	radius = GwConstants::Range::Spellcast;
	for (int i = 0; i < n_points; ++i) {
		float angle = i * (2 * static_cast<float>(M_PI) / n_points);
		float x = radius * std::cos(angle);
		float y = radius * std::sin(angle);
		aggro.push_back(Point2f(x, y));
	}
}

void RangeRenderer::Render(float x, float y) {
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_LOOP);
	for (Point2f p : radar) {
		glVertex2f(x + p.x(), y + p.y());
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for (Point2f p : aggro) {
		glVertex2f(x + p.x(), y + p.y());
	}
	glEnd();
}
