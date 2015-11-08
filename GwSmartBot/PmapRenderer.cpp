#include "PmapRenderer.h"

#include <vector>

void PmapRenderer::Initialize(IDirect3DDevice9* device) {

	PathingMapArray path_map = GWAPI::GWCA::Api().Map().GetPathingMap();

	// get the number of trapezoids, need it to allocate the vertex buffer
	size_t size = 0;
	for (size_t i = 0; i < path_map.size(); ++i) {
		size += path_map[i].trapezoidcount;
	}
	if (size == 0) return;

	count_ = size * 2;
	type_ = D3DPT_TRIANGLELIST;
	Vertex* vertices = nullptr;

	// allocate new vertex buffer
	if (buffer_) buffer_->Release();
	device->CreateVertexBuffer(sizeof(Vertex) * count_ * 3, 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &buffer_, NULL);
	buffer_->Lock(0, sizeof(Vertex) * count_ * 3,
		(VOID**)&vertices, D3DLOCK_DISCARD);

	// populate vertex buffer
	DWORD color = D3DCOLOR_ARGB(0xAA, 200, 200, 200);
	for (size_t i = 0; i < path_map.size(); ++i) {
		GWAPI::GW::PathingMap pmap = path_map[i];
		for (size_t j = 0; j < path_map[i].trapezoidcount; ++j) {
			PathingTrapezoid trapez = path_map[i].trapezoids[j];

			for (size_t k = 0; k < 6; ++k) {
				vertices[k].z = 1.0f;
				vertices[k].color = color;
			}

			// triangle 1
			// topleft
			vertices[0].x = trapez.XTL;
			vertices[0].y = trapez.YT;

			// topright
			vertices[1].x = trapez.XTR;
			vertices[1].y = trapez.YT;

			// botleft
			vertices[2].x = trapez.XBL;
			vertices[2].y = trapez.YB;

			// triangle 2
			// botleft
			vertices[3].x = trapez.XBL;
			vertices[3].y = trapez.YB;

			// topright
			vertices[4].x = trapez.XTR;
			vertices[4].y = trapez.YT;

			// botright
			vertices[5].x = trapez.XBR;
			vertices[5].y = trapez.YB;

			vertices += 6;
		}
	}

	buffer_->Unlock();
}
