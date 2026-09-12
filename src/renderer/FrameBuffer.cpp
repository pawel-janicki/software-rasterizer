#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height) : width(width), height(height), data(width * height) { }

void FrameBuffer::resize(int width, int height) {
	this->width = width;
	this->height = height;
	data.resize(width * height);
}
