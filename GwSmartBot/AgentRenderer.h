#pragma once

#include <vector>
#include <d3d9.h>

#include "..\gwcacomm\GWCACppClient\GWCAClient.h"

#include "MyD3DVertex.h"

class AgentRenderer {
public:
	void Initialize(IDirect3DDevice9* device);

	void Begin(IDirect3DDevice9* device);
	void RenderAgents(std::vector<AgentPosition> agents);
	void RenderPlayer(PseudoAgent* player);
	void End() { Flush(); }

private:
	
	void QueueTriangle(float x, float y, float rotation, float size, DWORD color);
	void QueueQuad(float x, float y, float size, DWORD color);

	void CheckFlush() {
		if (triangle_count > triangles_max - 2) Flush();
	}

	void Flush();

	IDirect3DDevice9* device_;
	IDirect3DVertexBuffer9* buffer_; // vertex buffer obect
	Vertex* vertices;			// vertices array
	unsigned int triangle_count;// count of triangles
	unsigned int vertices_max;	// max number of vertices to draw in one call
	unsigned int triangles_max; // max number of triangles to draw in one call
};
