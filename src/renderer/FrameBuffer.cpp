#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height) : width(width), height(height) { }

void FrameBuffer::createColorAttachment() {
	colorAttachment.assign(width * height, 0);
}

void FrameBuffer::createDepthAttachment() {
	depthAttachment.assign(width * height, 1);
}

void FrameBuffer::resize(int width, int height) {
	this->width = width;
	this->height = height;

	if (!colorAttachment.empty())
		colorAttachment.resize(width * height);

	if (!depthAttachment.empty())
		depthAttachment.resize(width * height);
}
