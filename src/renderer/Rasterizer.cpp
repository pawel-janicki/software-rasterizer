#include "Rasterizer.h"

Rasterizer::Rasterizer(FrameBuffer &frameBuffer) : frameBuffer(frameBuffer) { }

void Rasterizer::clear(uint32_t color) const {
	std::ranges::fill(frameBuffer.data.begin(), frameBuffer.data.end(), color);
}
