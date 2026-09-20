#ifndef SOFTWARE_RASTERIZER_RASTERIZER_H
#define SOFTWARE_RASTERIZER_RASTERIZER_H

#include "FrameBuffer.h"
#include "Mesh.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


class Rasterizer {
private:
	FrameBuffer& frameBuffer;

	std::function<glm::vec4(glm::vec4 color)> fragmentShader;

public:
	Rasterizer(FrameBuffer& frameBuffer);

	void clear(uint32_t color) const;
	void setPixel(int x, int y, glm::vec4 color) const;
	void drawMesh(Mesh& mesh) const;

	void setFragmentShader(std::function<glm::vec4(glm::vec4 color)> fragmentShader);
};


#endif
