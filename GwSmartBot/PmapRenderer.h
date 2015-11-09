#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "MyD3DVertex.h"
#include "Renderer.h"

class PmapRenderer {
public:
	PmapRenderer() : count_(0), buffer_(nullptr) {}

	bool Initialize(IDirect3DDevice9* device, unsigned int map_hash);

	void Render(IDirect3DDevice9* device);

private:
	IDirect3DVertexBuffer9* buffer_; // vertex buffer obect
	unsigned int count_;	// number of triangles to render
};
