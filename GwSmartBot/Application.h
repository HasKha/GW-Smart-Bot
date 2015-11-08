#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "..\gwcacomm\GWCACppClient\GWCAClient.h"

#pragma comment (lib, "d3d9.lib")

class Application {
public:
	Application(HWND hWnd);
	bool HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Update();
	void Render();

private:
	IDirect3D9* d3d;
	IDirect3DDevice9* d3ddev;
	const GWCAClient gwca;
};
