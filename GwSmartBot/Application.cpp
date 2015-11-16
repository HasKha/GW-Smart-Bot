#include "Application.h"

#include "SmartVaettirBot.h"

bool Application::DebugFunc() {
	return true;
}

Application::Application() : 
	gwca_(*new GWCAClient()), 
	bot_(*new SmartVaettirBot(gwca_)),
	viewer_(*new Viewer()),
	agent_renderer_(*new AgentRenderer()),
	pmap_renderer_(*new PmapRenderer()),
	range_renderer_(*new RangeRenderer()) {

	viewer_.InitializeWindow();
	range_renderer_.Initialize();
	
	//pmap_renderer_.Initialize(127484); // warrior's isle
	pmap_renderer_.Initialize(290943); // jaga moraine
}
Application::~Application() {
	delete &gwca_;
	delete &viewer_;
	delete &agent_renderer_;
	delete &pmap_renderer_;
}

bool Application::Connect() {
	HWND gw_handle = FindWindow(L"ArenaNet_Dx_Window_Class", NULL);
	DWORD gw_pid;
	GetWindowThreadProcessId(gw_handle, &gw_pid);
	printf("Connecting to PID %d\n", gw_pid);

	return gwca_.ConnectByPID(gw_pid);
}

void Application::Disconnect() {
	gwca_.Disconnect();
}

void Application::HandleInput() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			viewer_.Close();
			should_quit_ = true;
			return; // not break
		case SDL_MOUSEBUTTONDOWN:	HandleMouseDownEvent(e.button); break;
		case SDL_MOUSEBUTTONUP:		HandleMouseUpEvent(e.button);	break;
		case SDL_MOUSEMOTION:		HandleMouseMoveEvent(e.motion); break;
		case SDL_MOUSEWHEEL:		HandleMouseWheelEvent(e.wheel); break;
		case SDL_KEYDOWN:			HandleKeyDownEvent(e.key);		break;
		case SDL_KEYUP:				HandleKeyUpEvent(e.key);		break;
		case SDL_WINDOWEVENT:		HandleWindowEvent(e.window);	break;
		default:
			break;
		}
	}
}

void Application::Update() {
	world_.agents = gwca_.GetAgentsPos();
	world_.player = gwca_.GetPlayer();
	bot_.Update(world_);
}

void Application::Render() {
	viewer_.RenderBegin();
	pmap_renderer_.Render();
	agent_renderer_.RenderAgents(world_.agents);
	agent_renderer_.RenderPlayer(world_.player);
	range_renderer_.Render(world_.player.X, world_.player.Y);
	viewer_.RenderEnd();
}

void Application::HandleMouseDownEvent(SDL_MouseButtonEvent e) {
	viewer_.HandleMouseDownEvent(e);
}

void Application::HandleMouseUpEvent(SDL_MouseButtonEvent e) {
	viewer_.HandleMouseUpEvent(e);
}

void Application::HandleMouseMoveEvent(SDL_MouseMotionEvent e) {
	viewer_.HandleMouseMoveEvent(e);
}

void Application::HandleMouseWheelEvent(SDL_MouseWheelEvent e) {
	viewer_.HandleMouseWheelEvent(e);
}

void Application::HandleKeyDownEvent(SDL_KeyboardEvent e) {
}

void Application::HandleKeyUpEvent(SDL_KeyboardEvent e) {
}

void Application::HandleWindowEvent(SDL_WindowEvent e) {
	if (e.event == SDL_WINDOWEVENT_RESIZED) {
		viewer_.Resize(e.data1, e.data2);
	}
}
