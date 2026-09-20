#define GLM_ENABLE_EXPERIMENTAL

#include "Rasterizer.h"

#include <algorithm>

#include "glm/packing.hpp"
#include "glm/gtx/exterior_product.hpp"

Rasterizer::Rasterizer(FrameBuffer &frameBuffer) : frameBuffer(frameBuffer) { }

void Rasterizer::clear(uint32_t color) const {
	std::ranges::fill(frameBuffer.data.begin(), frameBuffer.data.end(), color);
}

void Rasterizer::setPixel(int x, int y, glm::vec4 color) const {
	uint8_t r = static_cast<uint8_t>(std::clamp(color.r, 0.0f, 1.0f) * 255);
	uint8_t g = static_cast<uint8_t>(std::clamp(color.g, 0.0f, 1.0f) * 255);
	uint8_t b = static_cast<uint8_t>(std::clamp(color.b, 0.0f, 1.0f) * 255);
	uint8_t a = static_cast<uint8_t>(std::clamp(color.a, 0.0f, 1.0f) * 255);

	frameBuffer.data[y * frameBuffer.width + x] = (r << 24) | (g << 16) | (b << 8) | a;
}

void Rasterizer::drawMesh(Mesh& mesh) const {
	int vertexCount = static_cast<int>(mesh.attributes[0].data.size() / (3 * 2));
	for (int i = 0; i < vertexCount; i++) {
		glm::vec2 v0(mesh.attributes[0].data[i * 6 + 0], mesh.attributes[0].data[i * 6 + 1]);
		glm::vec2 v1(mesh.attributes[0].data[i * 6 + 2], mesh.attributes[0].data[i * 6 + 3]);
		glm::vec2 v2(mesh.attributes[0].data[i * 6 + 4], mesh.attributes[0].data[i * 6 + 5]);

		glm::vec4 color0(mesh.attributes[1].data[i * 12 + 0], mesh.attributes[1].data[i * 12 + 1], mesh.attributes[1].data[i * 12 + 2], mesh.attributes[1].data[i * 12 + 3]);
		glm::vec4 color1(mesh.attributes[1].data[i * 12 + 4], mesh.attributes[1].data[i * 12 + 5], mesh.attributes[1].data[i * 12 + 6], mesh.attributes[1].data[i * 12 + 7]);
		glm::vec4 color2(mesh.attributes[1].data[i * 12 + 8], mesh.attributes[1].data[i * 12 + 9], mesh.attributes[1].data[i * 12 + 10], mesh.attributes[1].data[i * 12 + 11]);

		int minX = std::max(0, static_cast<int>(std::min({v0.x, v1.x, v2.x})));
		int maxX = std::min(frameBuffer.getWidth(), static_cast<int>(std::max({v0.x, v1.x, v2.x})));
		int minY = std::max(0, static_cast<int>(std::min({v0.y, v1.y, v2.y})));
		int maxY = std::min(frameBuffer.getHeight(), static_cast<int>(std::max({v0.y, v1.y, v2.y})));

		float det012 = glm::cross(v1 - v0, v2 - v0);
		bool ccw = det012 < 0;
		if (ccw) {
			std::swap(v1, v2);
			std::swap(color1, color2);
			det012 = -det012;
		}

		for (int y = minY; y < maxY; y++) {
			for (int x = minX; x < maxX; x++) {
				glm::vec2 pixel(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);

				float det01 = glm::cross(v1 - v0, pixel - v0);
				float det12 = glm::cross(v2 - v1, pixel - v1);
				float det20 = glm::cross(v0 - v2, pixel - v2);

				if (det01 >= 0 && det12 >= 0 && det20 >= 0) {
					float l0 = det12 / det012;
					float l1 = det20 / det012;
					float l2 = det01 / det012;

					setPixel(x, y, fragmentShader(color0 * l0 + color1 * l1 + color2 * l2));
				}
			}
		}
	}
}

void Rasterizer::setFragmentShader(std::function<glm::vec4(glm::vec4 color)> fragmentShader) {
	this->fragmentShader = fragmentShader;
}
