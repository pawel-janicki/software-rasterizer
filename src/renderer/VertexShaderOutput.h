#ifndef SOFTWARE_RASTERIZER_VERTEXSHADEROUTPUT_H
#define SOFTWARE_RASTERIZER_VERTEXSHADEROUTPUT_H

#include <vector>

#include "glm/vec3.hpp"


struct VertexShaderOutput {
	glm::vec3 position;
	std::vector<AttributeValue> attributes;
};


#endif
