#ifndef SOFTWARE_RASTERIZER_VERTEXSHADEROUTPUT_H
#define SOFTWARE_RASTERIZER_VERTEXSHADEROUTPUT_H

#include <vector>

#include "glm/vec2.hpp"


struct VertexShaderOutput {
	glm::vec2 position;
	std::vector<AttributeValue> attributes;
};


#endif
