#pragma once

#include <GWCACppClient\GWCAClient.h>

#include <SDL2\SDL.h>

#include "AgentRenderer.h"
#include "PmapRenderer.h"
#include "RangeRenderer.h"
#include "Viewer.h"
#include "Bot.h"

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

	GWCAClient& gwca() { return gwca_; }

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
	
	GWCAClient& gwca_;

	Bot& bot_;
	
	Viewer& viewer_;
	AgentRenderer& agent_renderer_;
	PmapRenderer& pmap_renderer_;
	RangeRenderer& range_renderer_;
};
