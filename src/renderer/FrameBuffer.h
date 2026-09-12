#ifndef SOFTWARE_RASTERIZER_FRAMEBUFFER_H
#define SOFTWARE_RASTERIZER_FRAMEBUFFER_H

#include <cstdint>
#include <vector>


class FrameBuffer {
public:
	int width;
	int height;
	std::vector<uint32_t> data;

	FrameBuffer(int width, int height);

	void resize(int width, int height);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
};


#endif
