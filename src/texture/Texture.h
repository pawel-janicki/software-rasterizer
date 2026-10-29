#ifndef SOFTWARE_RASTERIZER_TEXTURE_H
#define SOFTWARE_RASTERIZER_TEXTURE_H

#include <SDL3/SDL_surface.h>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


class Texture {
private:
	SDL_Surface* surface;

public:
	Texture(SDL_Surface* surface);
	~Texture();

	glm::vec4 sample(glm::vec2 uv) const;
};


#endif
