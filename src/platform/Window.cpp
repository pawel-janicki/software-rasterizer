#include "Window.h"

#include <format>
#include <stdexcept>

#include "SDL3/SDL_init.h"

Window::Window(const char* title, int width, int height) : frameBuffer(width, height) {
	this->width = width;
	this->height = height;

	if (!SDL_Init(SDL_INIT_VIDEO))
		throw std::runtime_error(std::format("ERROR >> Could not initialize SDL! SDL error: {}", SDL_GetError()));

	window = SDL_CreateWindow(title, width, height, 0);
	if (!window)
		throw std::runtime_error(std::format("ERROR >> Could not create window! SDL error: {}", SDL_GetError()));

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
		throw std::runtime_error(std::format("ERROR >> Could not create window renderer! SDL error: {}", SDL_GetError()));

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!texture)
		throw std::runtime_error(std::format("ERROR >> Could not create window texture! SDL error: {}", SDL_GetError()));
}

Window::~Window() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::resize(int width, int height) {
	if (this->width == width && this->height == height)
		return;

	this->width = width;
	this->height = height;

	frameBuffer.resize(width, height);

	if (width == 0 || height == 0)
		return;

	SDL_DestroyTexture(texture);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!texture)
		throw std::runtime_error(std::format("ERROR >> Could not create window texture! SDL error: {}", SDL_GetError()));
}

void Window::pollEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			shouldClose = true;
		} else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
			resize(event.window.data1, event.window.data2);
		}
	}
}

void Window::render() const {
	if (width == 0 || height == 0)
		return;

	SDL_UpdateTexture(texture, nullptr, frameBuffer.data.data(), width * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

const char* Window::getTitle() const {
	return SDL_GetWindowTitle(window);
}

void Window::setTitle(const char* title) const {
	SDL_SetWindowTitle(window, title);
}

void Window::setWidth(int width) {
	SDL_SetWindowSize(window, width, height);
}

void Window::setHeight(int height) {
	SDL_SetWindowSize(window, width, height);
}

bool Window::isResizable() const {
	return SDL_GetWindowFlags(window) & SDL_WINDOW_RESIZABLE;
}

void Window::setResizable(bool resizable) const {
	SDL_SetWindowResizable(window, resizable);
}
