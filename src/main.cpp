#include <format>

#include "renderer/Rasterizer.h"
#include "platform/Window.h"
#include "SDL3/SDL_timer.h"

int main() {
	Window window("Software Rasterizer", 1280, 720);
	window.setResizable(true);

	Rasterizer rasterizer(window.getFrameBuffer());

	glm::vec2 v1(100, 100);
	glm::vec2 v2(400, 100);
	glm::vec2 v3(100, 400);

	uint64_t lastTime = SDL_GetTicksNS();
	int frames = 0;
	float counter = 0;

	while (!window.windowShouldClose()) {
		uint64_t currentTime = SDL_GetTicksNS();
		uint64_t elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		float deltaTime = static_cast<float>(elapsedTime) * 1E-9f;

		frames++;
		counter += deltaTime;
		if (counter >= 0.5) {
			window.setTitle(std::format("Software Rasterizer | FPS: {0}", frames / counter).c_str());
			counter = 0;
			frames = 0;
		}

		window.pollEvents();

		rasterizer.clear(0xFFFFFFFF);
		rasterizer.drawTriangle(v1, v2, v3, 0x00FFFFFF);

		window.render();
	}

	return 0;
}
