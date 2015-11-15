#include "Viewer.h"

#include <SDL2\SDL_opengl.h>

#include <stdio.h>

Viewer::Viewer() :
	window(nullptr),
	width_(0),
	height_(0),
	mouse_down_(false),
	scale_(0.0001f),
	translate_(Point2f(0.0, 0.0)) {
}

void Viewer::InitializeWindow() {
	int default_width = 800;
	int default_height = 600;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("GwSmartBot",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		default_width, default_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GL_CreateContext(window);

	SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Resize(default_width, default_height);
}

void Viewer::Resize(int width, int height) {
	width_ = width;
	height_ = height;
	printf("resizing to %d %d\n", width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ratio_ = (float)width / height;
	glScalef(1, ratio_, 1);
}

void Viewer::RenderBegin() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, width_, height_);
	glScalef(scale_, scale_, 1);
	glTranslated(translate_.x(), translate_.y(), 0);
}

void Viewer::RenderEnd() {
	SDL_GL_SwapWindow(window);
}

void Viewer::Close() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void Viewer::HandleMouseDownEvent(SDL_MouseButtonEvent e) {
	if (e.button == SDL_BUTTON_LEFT) {
		mouse_down_ = true;
	}
}

void Viewer::HandleMouseUpEvent(SDL_MouseButtonEvent e) {
	if (e.button == SDL_BUTTON_LEFT) {
		mouse_down_ = false;
	}
}

void Viewer::HandleMouseMoveEvent(SDL_MouseMotionEvent e) {
	if (mouse_down_) {
		Point2f diff = Point2f(static_cast<float>(e.xrel), static_cast<float>(-e.yrel));
		diff.x() /= width_; // remap from [0, WIDTH] to [0, 1]
		diff.y() /= height_; // remap from [0, HEIGHT] to [0, 1]
		diff.y() /= ratio_;
		diff *= 2; // remap from [0, 1]^2 to [0, 2]^2 (screen space is [-1, 1] so range has to be 2
		diff /= scale_; // remap for scale

		translate_ += diff;
	}
}

void Viewer::HandleMouseWheelEvent(SDL_MouseWheelEvent e) {
	if (e.y > 0) {
		scale_ *= 1.25f;
	} else {
		scale_ *= 0.8f;
	}
}
