#include "Texture.h"
#include <glm/glm.hpp>

Texture::Texture(SDL_Surface* surface) {
	this->surface = surface;
}

Texture::~Texture() {
	SDL_DestroySurface(surface);
}

glm::vec4 Texture::sample(glm::vec2 uv) const {
	uv = glm::clamp(uv, glm::vec2(0.0f), glm::vec2(1.0f));

	int x = static_cast<int>(uv.x * surface->w);
	int y = static_cast<int>(uv.y * surface->h);

	const auto* pixels = static_cast<const std::uint8_t*>(surface->pixels);
	const auto* color = pixels + y * surface->pitch + x * 4;

	return glm::vec4(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f);
}
