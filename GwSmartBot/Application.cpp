#include "Application.h"

#include <SDL2\SDL_opengl.h>

bool Application::DebugFunc() {
	return true;
}

Application::Application() :
	should_quit_(false),
	bot_active_(false),
	viewer_(Viewer()),
	world_(new World()),
	agent_renderer_(AgentRenderer()),
	pmap_renderer_(PmapRenderer()),
	range_renderer_(RangeRenderer()),
	bot_(new FixedPathBot(*world_)) {

	GWCAClient::Initialize();

	viewer_.InitializeWindow();
	range_renderer_.Initialize();
	
	//pmap_renderer_.Initialize(127484); // warrior's isle
	pmap_renderer_.Initialize(290943); // jaga moraine

	fixed_path_.push_back(Point2f(13501, -20925));
	fixed_path_.push_back(Point2f(13172, -22137));
	fixed_path_.push_back(Point2f(12496, -22600));
	fixed_path_.push_back(Point2f(11375, -22761));
	fixed_path_.push_back(Point2f(10925, -23466));
	fixed_path_.push_back(Point2f(10917, -24311));
	fixed_path_.push_back(Point2f(9910, -24599));
	fixed_path_.push_back(Point2f(8995, -23177));
	fixed_path_.push_back(Point2f(8307, -23187));
	fixed_path_.push_back(Point2f(8213, -22829));
	fixed_path_.push_back(Point2f(8307, -23187));
	fixed_path_.push_back(Point2f(8213, -22829));
	fixed_path_.push_back(Point2f(8740, -22475));
	fixed_path_.push_back(Point2f(8880, -21384));
	fixed_path_.push_back(Point2f(8684, -20833));
	fixed_path_.push_back(Point2f(8982, -20576));
	fixed_path_.push_back(Point2f(10196, -20124));
	fixed_path_.push_back(Point2f(9976, -18338));
	fixed_path_.push_back(Point2f(11316, -18056));
	fixed_path_.push_back(Point2f(10392, -17512));
	fixed_path_.push_back(Point2f(10114, -16948));
	fixed_path_.push_back(Point2f(10729, -16273));
	fixed_path_.push_back(Point2f(10810, -15058));
	fixed_path_.push_back(Point2f(11120, -15105));
	fixed_path_.push_back(Point2f(11670, -15457));
	fixed_path_.push_back(Point2f(12604, -15320));
	fixed_path_.push_back(Point2f(12476, -16157));
	fixed_path_.push_back(Point2f(12920, -17032));
	fixed_path_.push_back(Point2f(12847, -17136));
	fixed_path_.push_back(Point2f(12720, -17222));
	fixed_path_.push_back(Point2f(12617, -17273));
	fixed_path_.push_back(Point2f(12518, -17305));
	fixed_path_.push_back(Point2f(12445, -17327));

	path_planner_.SetFinalDestination(Point2f(8982, -20576));
}

Application::~Application() {
	delete world_;
	delete bot_;
	GWCAClient::Destroy();
}

bool Application::Connect() {
	HWND gw_handle = FindWindow(L"ArenaNet_Dx_Window_Class", NULL);
	DWORD gw_pid;
	GetWindowThreadProcessId(gw_handle, &gw_pid);
	printf("Connecting to PID %d\n", gw_pid);

	return GWCAClient::Api().ConnectByPID(gw_pid);
}

void Application::Disconnect() {
	GWCAClient::Api().Disconnect();
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
	if (GWCAClient::Api().GetInstancType() == GwConstants::InstanceType::Loading) return;

	if (current_map_ != GWCAClient::Api().GetMapID()) {
		current_map_ = GWCAClient::Api().GetMapID();
		path_planner_.Clear();
		actual_path_.clear();
		printf("Zoned to %d, cleaned up things\n", current_map_);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}

	world_->Update();

	static int tau = 1;
	tau = (tau + 1) % 10;
	if (tau == 0) {
		Point2f pos = Point2f(world_->player().X, world_->player().Y);
		if (pos.x() != 0 || pos.y() != 0) {
			if (actual_path_.empty() || !(actual_path_.back() == pos)) {
				actual_path_.push_back(pos);
				//printf("x %f y %f\n", pos.x(), pos.y());
			}
		}
	}

	if (bot_active_) {
		bot_->Update();
	}

	path_planner_.Update(*world_);
}

void Application::Render() {
	glLineWidth(2.0f);
	viewer_.RenderBegin();
	pmap_renderer_.Render();
	
	//glColor3f(1, 0, 0);
	//glBegin(GL_LINE_STRIP);
	//for (Point2f p : fixed_path_) {
	//	glVertex2f(p.x(), p.y());
	//}
	//glEnd();
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (Point2f p : actual_path_) {
		glVertex2f(p.x(), p.y());
	}
	glEnd();

	path_planner_.Render(*world_);

	agent_renderer_.RenderAgents(world_->agents());
	agent_renderer_.RenderPlayer(world_->player());
	range_renderer_.Render(world_->player().X, world_->player().Y);
	viewer_.RenderEnd();
}

void Application::RenderFixedPath() {
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (Point2f p : fixed_path_) {
		glVertex2f(p.x(), p.y());
	}
	glEnd();
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
	if (e.keysym.sym == SDLK_SPACE) {
		bot_active_ = !bot_active_;
		printf("bot active %d\n", bot_active_);
	}
}

void Application::HandleWindowEvent(SDL_WindowEvent e) {
	if (e.event == SDL_WINDOWEVENT_RESIZED) {
		viewer_.Resize(e.data1, e.data2);
	}
}
