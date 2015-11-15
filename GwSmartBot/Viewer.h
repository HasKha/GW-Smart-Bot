#pragma once

#include <SDL2\SDL.h>

#include "Point2f.h"

// handles SDL window creation, opengl transformations
class Viewer {
public:
	Viewer();

	void InitializeWindow();

	void Resize(int width, int height);

	void RenderBegin();
	void RenderEnd();

	void Close();

	void HandleMouseDownEvent(SDL_MouseButtonEvent);
	void HandleMouseUpEvent(SDL_MouseButtonEvent);
	void HandleMouseMoveEvent(SDL_MouseMotionEvent);
	void HandleMouseWheelEvent(SDL_MouseWheelEvent);

private:
	SDL_Window* window;

	int width_;
	int height_;

	bool mouse_down_;
	float scale_;
	Point2f translate_;
	float ratio_;
};
