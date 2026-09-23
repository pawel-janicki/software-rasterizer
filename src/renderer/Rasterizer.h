#ifndef SOFTWARE_RASTERIZER_RASTERIZER_H
#define SOFTWARE_RASTERIZER_RASTERIZER_H

#include "FrameBuffer.h"
#include "Mesh.h"
#include "VertexShaderOutput.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"


class Rasterizer {
private:
	FrameBuffer& frameBuffer;

	std::function<VertexShaderOutput(std::vector<AttributeValue>&)> vertexShader;
	std::function<glm::vec4(std::vector<AttributeValue>&)> fragmentShader;

	AttributeValue interpolateAttribute(const AttributeValue& v0, const AttributeValue& v1, const AttributeValue& v2, float l0, float l1, float l2) const;

public:
	Rasterizer(FrameBuffer& frameBuffer);

	void clear(uint32_t color) const;
	void setPixel(int x, int y, glm::vec4 color) const;
	void drawMesh(const Mesh& mesh, int triangleCount) const;

	void setVertexShader(std::function<VertexShaderOutput(const std::vector<AttributeValue>&)> vertexShader);
	void setFragmentShader(std::function<glm::vec4(const std::vector<AttributeValue>&)> fragmentShader);
};


#endif
