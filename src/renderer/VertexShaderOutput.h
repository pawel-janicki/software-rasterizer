#ifndef SOFTWARE_RASTERIZER_VERTEXSHADEROUTPUT_H
#define SOFTWARE_RASTERIZER_VERTEXSHADEROUTPUT_H

#include <vector>

#include "glm/vec4.hpp"


struct VertexShaderOutput {
	glm::vec4 position;
	std::vector<AttributeValue> attributes;
};


#endif
