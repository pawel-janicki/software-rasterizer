#define GLM_ENABLE_EXPERIMENTAL

#include "Rasterizer.h"

#include <algorithm>

#include "glm/gtx/exterior_product.hpp"

Rasterizer::Rasterizer(FrameBuffer &frameBuffer) : frameBuffer(frameBuffer) { }

void Rasterizer::clear(uint32_t color) const {
	std::ranges::fill(frameBuffer.data.begin(), frameBuffer.data.end(), color);
}

void Rasterizer::setPixel(int x, int y, uint32_t color) const {
	frameBuffer.data[y * frameBuffer.width + x] = color;
}

void Rasterizer::drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, uint32_t color) const {
	int minX = std::max(0, static_cast<int>(std::min({v1.x, v2.x, v3.x})));
	int maxX = std::min(frameBuffer.getWidth(), static_cast<int>(std::max({v1.x, v2.x, v3.x})));
	int minY = std::max(0, static_cast<int>(std::min({v1.y, v2.y, v3.y})));
	int maxY = std::min(frameBuffer.getHeight(), static_cast<int>(std::max({v1.y, v2.y, v3.y})));

	for (int y = minY; y < maxY; y++) {
		for (int x = minX; x < maxX; x++) {
			glm::vec2 pixel(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);

			float det12 = glm::cross(v2 - v1, pixel - v1);
			float det23 = glm::cross(v3 - v2, pixel - v2);
			float det31 = glm::cross(v1 - v3, pixel - v3);

			if ((det12 >= 0 && det23 >= 0 && det31 >= 0) || (det12 <= 0 && det23 <= 0 && det31 <= 0))
				setPixel(x, y, color);
		}
	}
}
