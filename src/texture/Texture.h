#ifndef SOFTWARE_RASTERIZER_TEXTURE_H
#define SOFTWARE_RASTERIZER_TEXTURE_H

#include <SDL3/SDL_surface.h>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


enum class TextureFilter {
	Nearest,
	Bilinear
};

enum class TextureWrap {
	ClampToEdge,
	Repeat
};

class Texture {
private:
	SDL_Surface* surface;

	TextureFilter filter = TextureFilter::Nearest;
	TextureWrap wrap = TextureWrap::ClampToEdge;

	glm::vec4 sampleNearest(glm::vec2 uv) const;
	glm::vec4 sampleBilinear(glm::vec2 uv) const;

	glm::ivec2 wrapCoordinates(glm::ivec2 texelCoords) const;

public:
	Texture(SDL_Surface* surface);
	~Texture();

	glm::vec4 sample(glm::vec2 uv) const;

	void setFilter(TextureFilter filter);
	void setWrap(TextureWrap wrap);
};


#endif
