#include "Application.h"

#include <DirectXMath.h>

Application::Application(HWND hWnd) : 
	gwca(*new GWCAClient()), 
	agent_renderer_(*new AgentRenderer()),
	pmap_renderer_(*new PmapRenderer()) {
	
	// directx init
	directx = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	directx->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device);

	
	pmap_renderer_.Initialize(device, 127484);
	agent_renderer_.Initialize(device);
}

bool Application::Connect() {
	HWND gw_handle = FindWindow(L"ArenaNet_Dx_Window_Class", NULL);
	DWORD gw_pid;
	GetWindowThreadProcessId(gw_handle, &gw_pid);

	return gwca.ConnectByPID(gw_pid);
}

void Application::Disconnect() {
	gwca.Disconnect();
}

bool Application::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return false;
}

void Application::Update() {
	agents = gwca.GetAgentsPos();
	player = gwca.GetPlayer();
	printf("x %f, y %f\n", player.X, player.Y);
}

void Application::Render() {
	using namespace DirectX;

	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	device->BeginScene();

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixIdentity());
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&world);

	float aspect_ratio = 1.3f;
	float scale_ = 0.0001f;
	XMMATRIX translate = XMMatrixTranslation(0, -3000, 0);
	XMMATRIX rotate = XMMatrixRotationZ(0);
	XMMATRIX scale = XMMatrixScaling(scale_, scale_ * aspect_ratio, 1);
	XMMATRIX combined = translate * rotate * scale;
	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, combined);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);

	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, XMMatrixOrthographicLH(2, 2, 0.5f, 1.5f));
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection);

	pmap_renderer_.Render(device);

	agent_renderer_.Begin(device);
	agent_renderer_.RenderAgents(agents);
	agent_renderer_.RenderPlayer(&player);
	agent_renderer_.End();

	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}
