#ifndef SOFTWARE_RASTERIZER_RASTERIZER_H
#define SOFTWARE_RASTERIZER_RASTERIZER_H

#include "FrameBuffer.h"
#include "glm/vec2.hpp"


class Rasterizer {
private:
	FrameBuffer& frameBuffer;

public:
	Rasterizer(FrameBuffer& frameBuffer);

	void clear(uint32_t color) const;
	void setPixel(int x, int y, uint32_t color) const;
	void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, uint32_t color) const;
};


#endif
