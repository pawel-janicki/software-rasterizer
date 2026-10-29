#ifndef SOFTWARE_RASTERIZER_TEXTURELOADER_H
#define SOFTWARE_RASTERIZER_TEXTURELOADER_H

#include "Texture.h"


class TextureLoader {
public:
	static Texture loadTexture(const char* filePath);
};


#endif
