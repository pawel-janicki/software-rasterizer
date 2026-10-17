#include <format>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Rasterizer.h"
#include "platform/Window.h"
#include "renderer/Attribute.h"
#include "renderer/Mesh.h"
#include "renderer/VertexShaderOutput.h"
#include "SDL3/SDL_timer.h"

int main() {
	Window window("Software Rasterizer", 1280, 720);
	window.setResizable(true);

	Rasterizer rasterizer(window.getFrameBuffer());
	rasterizer.enableDepthTest();

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f), window.getWidth() / static_cast<float>(window.getHeight()), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::inverse(glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.0f)));
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -3.0f));

	rasterizer.setVertexShader([&projectionMatrix, &viewMatrix, &transformationMatrix](const std::vector<AttributeValue>& attributes) {
		glm::vec3 vertexPosition = std::get<glm::vec3>(attributes[0]);
		glm::vec3 color = std::get<glm::vec3>(attributes[1]);

		glm::vec4 position = projectionMatrix * viewMatrix * transformationMatrix * glm::vec4(vertexPosition, 1.0f);

		std::vector<AttributeValue> out = {color};
		return VertexShaderOutput(position, out);
	});

	rasterizer.setFragmentShader([](const std::vector<AttributeValue>& attributes) {
		glm::vec3 color = std::get<glm::vec3>(attributes[0]);
		return glm::vec4(color, 1.0f);
	});

	Attribute positions(3, std::vector<float>{-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f});
	Attribute colors(3, std::vector<float>{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f});
	Mesh mesh({positions, colors});
	int triangleCount = positions.data.size() / positions.dimensions / 3;

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

		projectionMatrix = glm::perspective(glm::radians(70.0f), window.getWidth() / static_cast<float>(window.getHeight()), 0.1f, 100.0f);
		transformationMatrix = glm::rotate(transformationMatrix, glm::radians(deltaTime * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		rasterizer.clear(0xFFFFFFFF);
		rasterizer.clearDepth();
		rasterizer.drawMesh(mesh, triangleCount);

		window.render();
	}

	return 0;
}
