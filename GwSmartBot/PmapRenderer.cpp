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

bool PmapRenderer::Initialize(unsigned int map_hash) {
	TCHAR file_name[MAX_PATH];
	wsprintf(file_name, L"PMAPs\\MAP %010u.pmap", map_hash);
	PathingMap pmap(map_hash);
	bool loaded = pmap.Open(file_name);
	if (loaded) {
		printf("loaded pmap %d, %d trapezoids\n", map_hash, pmap.GetPathingData().size());
		trapezoids_ = pmap.GetPathingData();
		return true;
	} else {
		printf("failed to load pmap!\n");
		trapezoids_.clear();
		return false;
	}
}
