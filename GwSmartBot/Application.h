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
#include "SmartVaettirBot.h"

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

	const World world() { return world_; }

private:
	void HandleMouseDownEvent(SDL_MouseButtonEvent);
	void HandleMouseUpEvent(SDL_MouseButtonEvent);
	void HandleMouseMoveEvent(SDL_MouseMotionEvent);
	void HandleMouseWheelEvent(SDL_MouseWheelEvent);
	void HandleKeyDownEvent(SDL_KeyboardEvent);
	void HandleKeyUpEvent(SDL_KeyboardEvent);
	void HandleWindowEvent(SDL_WindowEvent);

	bool should_quit_;

	World world_;
		
	Viewer viewer_;
	AgentRenderer agent_renderer_;
	PmapRenderer pmap_renderer_;
	RangeRenderer range_renderer_;
};
