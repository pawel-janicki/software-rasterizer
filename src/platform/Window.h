#ifndef SOFTWARE_RASTERIZER_WINDOW_H
#define SOFTWARE_RASTERIZER_WINDOW_H

#include "../renderer/FrameBuffer.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"


class Window {
private:
	int width;
	int height;

	SDL_Window* window = nullptr;
	bool shouldClose = false;

	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;

	FrameBuffer frameBuffer;

	void resize(int width, int height);

public:
	Window(const char* title, int width, int height);
	~Window();

	void pollEvents();
	void render() const;

	bool windowShouldClose() const { return shouldClose; }

	const char* getTitle() const;
	void setTitle(const char* title) const;

	int getWidth() const { return width; }
	void setWidth(int width);

	int getHeight() const { return height; }
	void setHeight(int height);

	bool isResizable() const;
	void setResizable(bool resizable) const;

	FrameBuffer& getFrameBuffer() { return frameBuffer; }
};


#endif
