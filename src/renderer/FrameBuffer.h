#ifndef SOFTWARE_RASTERIZER_FRAMEBUFFER_H
#define SOFTWARE_RASTERIZER_FRAMEBUFFER_H

#include <cstdint>
#include <vector>


class FrameBuffer {
private:
	int width;
	int height;

public:
	std::vector<uint32_t> colorAttachment;
	std::vector<float> depthAttachment;

	FrameBuffer(int width, int height);

	void createColorAttachment();
	void createDepthAttachment();

	void resize(int width, int height);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
};


#endif
