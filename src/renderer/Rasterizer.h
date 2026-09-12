#ifndef SOFTWARE_RASTERIZER_RASTERIZER_H
#define SOFTWARE_RASTERIZER_RASTERIZER_H

#include "FrameBuffer.h"


class Rasterizer {
private:
	FrameBuffer& frameBuffer;

public:
	Rasterizer(FrameBuffer& frameBuffer);

	void clear(uint32_t color) const;
};


#endif
