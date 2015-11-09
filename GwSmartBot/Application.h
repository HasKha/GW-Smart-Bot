#pragma once

#include <Windows.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

#include "..\gwcacomm\GWCACppClient\GWCAClient.h"

#include "AgentRenderer.h"
#include "PmapRenderer.h"

class Application {
public:
	Application(HWND hWnd);
	bool Connect();
	bool HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Update();
	void Render();
	void Disconnect();

private:
	std::vector<AgentPosition> agents;
	PseudoAgent player;

	IDirect3D9* directx;
	IDirect3DDevice9* device;
	
	GWCAClient& gwca;
	
	AgentRenderer& agent_renderer_;
	PmapRenderer& pmap_renderer_;
};
