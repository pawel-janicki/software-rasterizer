#ifndef SOFTWARE_RASTERIZER_ATTRIBUTE_H
#define SOFTWARE_RASTERIZER_ATTRIBUTE_H

#include <variant>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"


using AttributeValue = std::variant<float, glm::vec2, glm::vec3, glm::vec4>;

struct Attribute {
	int dimensions;
	std::vector<float> data;
};


#endif
