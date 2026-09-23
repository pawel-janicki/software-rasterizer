#define GLM_ENABLE_EXPERIMENTAL

#include "Rasterizer.h"

#include <algorithm>

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

void Rasterizer::drawMesh(const Mesh& mesh, int triangleCount) const {
	for (int i = 0; i < triangleCount; i++) {
		std::vector<AttributeValue> v0Attributes;
		std::vector<AttributeValue> v1Attributes;
		std::vector<AttributeValue> v2Attributes;

		for (const Attribute& attribute : mesh.attributes) {
			int baseIndex = i * 3 * attribute.dimensions;

			if (attribute.dimensions == 1) {
				v0Attributes.emplace_back(attribute.data[baseIndex]);
				v1Attributes.emplace_back(attribute.data[baseIndex + 1]);
				v2Attributes.emplace_back(attribute.data[baseIndex + 2]);
			} else if (attribute.dimensions == 2) {
				v0Attributes.emplace_back(glm::vec2(attribute.data[baseIndex], attribute.data[baseIndex + 1]));
				v1Attributes.emplace_back(glm::vec2(attribute.data[baseIndex + 2], attribute.data[baseIndex + 3]));
				v2Attributes.emplace_back(glm::vec2(attribute.data[baseIndex + 4], attribute.data[baseIndex + 5]));
			} else if (attribute.dimensions == 3) {
				v0Attributes.emplace_back(glm::vec3(attribute.data[baseIndex], attribute.data[baseIndex + 1], attribute.data[baseIndex + 2]));
				v1Attributes.emplace_back(glm::vec3(attribute.data[baseIndex + 3], attribute.data[baseIndex + 4], attribute.data[baseIndex + 5]));
				v2Attributes.emplace_back(glm::vec3(attribute.data[baseIndex + 6], attribute.data[baseIndex + 7], attribute.data[baseIndex + 8]));
			} else if (attribute.dimensions == 4) {
				v0Attributes.emplace_back(glm::vec4(attribute.data[baseIndex], attribute.data[baseIndex + 1], attribute.data[baseIndex + 2], attribute.data[baseIndex + 3]));
				v1Attributes.emplace_back(glm::vec4(attribute.data[baseIndex + 4], attribute.data[baseIndex + 5], attribute.data[baseIndex + 6], attribute.data[baseIndex + 7]));
				v2Attributes.emplace_back(glm::vec4(attribute.data[baseIndex + 8], attribute.data[baseIndex + 9], attribute.data[baseIndex + 10], attribute.data[baseIndex + 11]));
			}
		}

		VertexShaderOutput v0Output = vertexShader(v0Attributes);
		VertexShaderOutput v1Output = vertexShader(v1Attributes);
		VertexShaderOutput v2Output = vertexShader(v2Attributes);

		glm::vec2 v0 = v0Output.position;
		glm::vec2 v1 = v1Output.position;
		glm::vec2 v2 = v2Output.position;

		int minX = std::max(0, static_cast<int>(std::min({v0.x, v1.x, v2.x})));
		int maxX = std::min(frameBuffer.getWidth(), static_cast<int>(std::max({v0.x, v1.x, v2.x})));
		int minY = std::max(0, static_cast<int>(std::min({v0.y, v1.y, v2.y})));
		int maxY = std::min(frameBuffer.getHeight(), static_cast<int>(std::max({v0.y, v1.y, v2.y})));

		float det012 = glm::cross(v1 - v0, v2 - v0);
		bool ccw = det012 < 0;
		if (ccw) {
			std::swap(v1, v2);
			std::swap(v1Output.attributes, v2Output.attributes);
			det012 = -det012;
		}

		std::vector<AttributeValue> fragmentAttributes(v0Output.attributes.size());

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

					for (int j = 0; j < v0Output.attributes.size(); j++) {
						fragmentAttributes[j] = interpolateAttribute(v0Output.attributes[j], v1Output.attributes[j], v2Output.attributes[j], l0, l1, l2);
					}

					setPixel(x, y, fragmentShader(fragmentAttributes));
				}
			}
		}
	}
}

AttributeValue Rasterizer::interpolateAttribute(const AttributeValue& v0, const AttributeValue& v1, const AttributeValue& v2, float l0, float l1, float l2) const {
	if (v0.index() == 0) {
		return std::get<float>(v0) * l0 + std::get<float>(v1) * l1 + std::get<float>(v2) * l2;
	} else if (v0.index() == 1) {
		return std::get<glm::vec2>(v0) * l0 + std::get<glm::vec2>(v1) * l1 + std::get<glm::vec2>(v2) * l2;
	} else if (v0.index() == 2) {
		return std::get<glm::vec3>(v0) * l0 + std::get<glm::vec3>(v1) * l1 + std::get<glm::vec3>(v2) * l2;
	}

	return std::get<glm::vec4>(v0) * l0 + std::get<glm::vec4>(v1) * l1 + std::get<glm::vec4>(v2) * l2;
}

void Rasterizer::setVertexShader(std::function<VertexShaderOutput(const std::vector<AttributeValue>&)> vertexShader) {
	this->vertexShader = vertexShader;
}

void Rasterizer::setFragmentShader(std::function<glm::vec4(const std::vector<AttributeValue>&)> fragmentShader) {
	this->fragmentShader = fragmentShader;
}
