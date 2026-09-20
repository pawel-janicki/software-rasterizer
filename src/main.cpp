#include <format>

#include "renderer/Rasterizer.h"
#include "platform/Window.h"
#include "renderer/Attribute.h"
#include "renderer/Mesh.h"
#include "SDL3/SDL_timer.h"

int main() {
	Window window("Software Rasterizer", 1280, 720);
	window.setResizable(true);

	Rasterizer rasterizer(window.getFrameBuffer());

	rasterizer.setFragmentShader([](glm::vec4 color) {
		return color;
	});

	Attribute positions(std::vector<float>{100, 100, 1180, 100, 100, 620, 100, 620, 1180, 100, 1180, 620});
	Attribute colors(std::vector<float>{1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1});
	Mesh mesh({positions, colors});

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
		rasterizer.drawMesh(mesh);

		window.render();
	}

	return 0;
}
