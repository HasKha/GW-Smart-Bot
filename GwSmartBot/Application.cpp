#include "Application.h"

Application::Application(HWND hWnd) : 
	gwca(GWCAClient()) {

	// directx init
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
}

bool Application::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return false;
}

void Application::Update() {

}

void Application::Render() {
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	d3ddev->BeginScene();

	// render here

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}
