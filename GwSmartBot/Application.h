#pragma once

#include <GWCACppClient\GWCAClient.h>

#include <SDL2\SDL.h>
#include <thread>
#include <functional>
#include <future>

#include "AgentRenderer.h"
#include "PmapRenderer.h"
#include "RangeRenderer.h"
#include "Viewer.h"
#include "SmartBot.h"
#include "PathPlanner.h"

class Application {
public:
	Application();
	~Application();

	bool Connect();

	bool DebugFunc();

	void HandleInput();

	bool ShouldQuit() { return should_quit_; }
	
	void Update();
	
	void Render();
	
	void Disconnect();

private:
	void HandleMouseDownEvent(SDL_MouseButtonEvent);
	void HandleMouseUpEvent(SDL_MouseButtonEvent);
	void HandleMouseMoveEvent(SDL_MouseMotionEvent);
	void HandleMouseWheelEvent(SDL_MouseWheelEvent);
	void HandleKeyDownEvent(SDL_KeyboardEvent);
	void HandleKeyUpEvent(SDL_KeyboardEvent);
	void HandleWindowEvent(SDL_WindowEvent);

	void RenderFixedPath();

	bool should_quit_;

	World* world_;

	GwConstants::MapID current_map_;
		
	Viewer viewer_;
	AgentRenderer agent_renderer_;
	PmapRenderer pmap_renderer_;
	RangeRenderer range_renderer_;

	bool bot_active_;
	VaettirBot* bot_;

	std::vector<Point2f> fixed_path_;
	std::vector<Point2f> actual_path_;
};
