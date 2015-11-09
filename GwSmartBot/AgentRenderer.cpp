#include "AgentRenderer.h"

#include <cmath>

const float M_PI = 3.14159265358979323846f;

void AgentRenderer::Initialize(IDirect3DDevice9* device) {
	triangles_max = 0x200;
	vertices_max = triangles_max * 3;

	vertices = nullptr;
	device->CreateVertexBuffer(sizeof(Vertex) * vertices_max, 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &buffer_, NULL);
	buffer_->Lock(0, sizeof(Vertex) * vertices_max, (VOID**)&vertices, D3DLOCK_NOOVERWRITE);
}

void AgentRenderer::Begin(IDirect3DDevice9* device) {
	triangle_count = 0;
	device_ = device;
}

void AgentRenderer::RenderAgents(std::vector<AgentPosition> agents) {
	// all agents
	for (size_t i = 0; i < agents.size(); ++i) {
		AgentPosition agent = agents[i];
		QueueQuad(agent.x, agent.y, 75, D3DCOLOR_XRGB(200, 0, 0));
	}
}

void AgentRenderer::RenderPlayer(PseudoAgent* player) {
	//QueueTriangle(player->X, player->Y, player->Rotation, 150, D3DCOLOR_XRGB(0, 200, 0));
	QueueQuad(player->X, player->Y, 100, D3DCOLOR_XRGB(0, 200, 0));
}

void AgentRenderer::QueueTriangle(float x, float y, 
	float rotation, float size, DWORD color) {
	for (int i = 0; i < 3; ++i) {
		vertices[i].z = 1.0f;
		vertices[i].color = color;
	}

	vertices[0].x = x + size * 1.3f * std::cos(rotation);
	vertices[0].y = y + size * 1.3f * std::sin(rotation);

	vertices[1].x = x + size * std::cos(rotation + M_PI * 2 / 3);
	vertices[1].y = y - size * std::sin(rotation + M_PI * 2 / 3);

	vertices[2].x = x - size * std::cos(rotation - M_PI * 2 / 3);
	vertices[2].y = y - size * std::sin(rotation - M_PI * 2 / 3);

	vertices += 3;
	triangle_count += 1;

	CheckFlush();
}

void AgentRenderer::QueueQuad(float x, float y, float size, DWORD color) {

	for (int i = 0; i < 6; ++i) {
		vertices[i].z = 1.0f;
		vertices[i].color = color;
	}

	vertices[0].x = x - size;
	vertices[0].y = y + size;
	vertices[1].x = x + size;
	vertices[1].y = y + size;
	vertices[2].x = x - size;
	vertices[2].y = y - size;
	vertices[3].x = x - size;
	vertices[3].y = y - size;
	vertices[4].x = x + size;
	vertices[4].y = y + size;
	vertices[5].x = x + size;
	vertices[5].y = y - size;

	vertices += 6;
	triangle_count += 2;

	CheckFlush();
}

void AgentRenderer::Flush() {
	buffer_->Unlock();
	device_->SetStreamSource(0, buffer_, 0, sizeof(Vertex));
	device_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangle_count);
	buffer_->Lock(0, sizeof(Vertex) * vertices_max, (VOID**)&vertices, D3DLOCK_NOOVERWRITE);
	triangle_count = 0;
}
