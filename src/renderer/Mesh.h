#ifndef SOFTWARE_RASTERIZER_MESH_H
#define SOFTWARE_RASTERIZER_MESH_H

#include <vector>

#include "Attribute.h"


struct Mesh {
	std::vector<Attribute> attributes;
};


#endif
