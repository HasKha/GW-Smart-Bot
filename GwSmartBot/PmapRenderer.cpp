#include "PmapRenderer.h"

#include "PathingMap.h"

#include <vector>

void PmapRenderer::Render(IDirect3DDevice9* device) {
	if (count_ == 0) return;

	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	device->SetStreamSource(0, buffer_, 0, sizeof(Vertex));
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, count_);
}

bool PmapRenderer::Initialize(IDirect3DDevice9* device, unsigned int map_hash) {
	count_ = 0;

	size_t file_hash = 127484;
	TCHAR file_name[MAX_PATH];
	wsprintf(file_name, L"PMAPs\\MAP %010u.pmap", file_hash);
	PathingMap pmap(file_hash);
	bool loaded = pmap.Open(file_name);
	if (loaded) {
		printf("loaded pmap %d!\n", file_hash);
	} else {
		printf("failed to load pmap!\n");
		return false;
	}

	std::vector<PathingMapTrapezoid> trapezoids = pmap.GetPathingData();
	if (trapezoids.size() == 0) {
		printf("empty trapez array!\n");
		return false;
	}
	count_ = trapezoids.size() * 2;
	Vertex* vertices = nullptr;
	if (buffer_) buffer_->Release();
	device->CreateVertexBuffer(sizeof(Vertex) * count_ * 3, 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &buffer_, NULL);
	buffer_->Lock(0, sizeof(Vertex) * count_ * 3,
		(VOID**)&vertices, D3DLOCK_DISCARD);

	DWORD color = D3DCOLOR_ARGB(0xFF, 200, 200, 200);
	for (size_t i = 0; i < trapezoids.size(); ++i) {
		PathingMapTrapezoid trapez = trapezoids[i];

		for (size_t j = 0; j < 6; ++j) {
			vertices[j].z = 1.0f;
			vertices[j].color = color;
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
	buffer_->Unlock();

	return true;
}
