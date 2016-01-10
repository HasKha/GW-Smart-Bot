#include "PmapRenderer.h"

#include <SDL2\SDL_opengl.h>

#include "PathingMap.h"

#include <vector>

void PmapRenderer::Render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	for (size_t i = 0; i < trapezoids_.size(); ++i) {
		glVertex2f(trapezoids_[i].XTL, trapezoids_[i].YT);
		glVertex2f(trapezoids_[i].XTR, trapezoids_[i].YT);
		glVertex2f(trapezoids_[i].XBR, trapezoids_[i].YB);
		glVertex2f(trapezoids_[i].XBL, trapezoids_[i].YB);
	}
	glEnd();
}

bool PmapRenderer::Initialize(PathingMap& pmap) {
	trapezoids_ = pmap.GetPathingData();
	return true;
}
