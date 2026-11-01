#include "Texture.h"
#include <glm/glm.hpp>

Texture::Texture(SDL_Surface* surface) {
	this->surface = surface;
}

Texture::~Texture() {
	SDL_DestroySurface(surface);
}

glm::vec4 Texture::sample(glm::vec2 uv) const {
	switch (this->filter) {
		case TextureFilter::Nearest: return sampleNearest(uv);
		case TextureFilter::Bilinear: return sampleBilinear(uv);
		default: return glm::vec4(0.0f);
	}
}

glm::vec4 Texture::sampleNearest(glm::vec2 uv) const {
	glm::ivec2 texelCoords = wrapCoordinates(glm::ivec2(std::floor(uv.x * surface->w), std::floor(uv.y * surface->h)));

	const auto* pixels = static_cast<const std::uint8_t*>(surface->pixels);
	const auto* color = pixels + texelCoords.y * surface->pitch + texelCoords.x * 4;

	return glm::vec4(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f);
}

glm::vec4 Texture::sampleBilinear(glm::vec2 uv) const {
	glm::ivec2 texelCoordsMin = wrapCoordinates(glm::ivec2(std::floor(uv.x * surface->w), std::floor(uv.y * surface->h)));
	glm::ivec2 texelCoordsMax = wrapCoordinates(glm::ivec2(std::floor(uv.x * surface->w) + 1, std::floor(uv.y * surface->h) + 1));

	const auto* pixels = static_cast<const std::uint8_t*>(surface->pixels);

	const auto* colorData00 = pixels + texelCoordsMin.y * surface->pitch + texelCoordsMin.x * 4;
	const auto* colorData10 = pixels + texelCoordsMin.y * surface->pitch + texelCoordsMax.x * 4;
	const auto* colorData01 = pixels + texelCoordsMax.y * surface->pitch + texelCoordsMin.x * 4;
	const auto* colorData11 = pixels + texelCoordsMax.y * surface->pitch + texelCoordsMax.x * 4;

	glm::vec4 color00 = glm::vec4(colorData00[0] / 255.0f, colorData00[1] / 255.0f, colorData00[2] / 255.0f, colorData00[3] / 255.0f);
	glm::vec4 color10 = glm::vec4(colorData10[0] / 255.0f, colorData10[1] / 255.0f, colorData10[2] / 255.0f, colorData10[3] / 255.0f);
	glm::vec4 color01 = glm::vec4(colorData01[0] / 255.0f, colorData01[1] / 255.0f, colorData01[2] / 255.0f, colorData01[3] / 255.0f);
	glm::vec4 color11 = glm::vec4(colorData11[0] / 255.0f, colorData11[1] / 255.0f, colorData11[2] / 255.0f, colorData11[3] / 255.0f);

	glm::vec4 topColor = glm::mix(color00, color10, uv.x * surface->w - std::floor(uv.x * surface->w));
	glm::vec4 bottomColor = glm::mix(color01, color11, uv.x * surface->w - std::floor(uv.x * surface->w));

	return glm::mix(topColor, bottomColor, uv.y * surface->h - std::floor(uv.y * surface->h));
}

glm::ivec2 Texture::wrapCoordinates(glm::ivec2 texelCoords) const {
	switch (this->wrap) {
		case TextureWrap::ClampToEdge: return glm::clamp(texelCoords, glm::ivec2(0.0f), glm::ivec2(surface->w - 1, surface->h - 1));
		case TextureWrap::Repeat: return glm::ivec2((texelCoords.x % surface->w + surface->w) % surface->w, (texelCoords.y % surface->h + surface->h) % surface->h);
		default: return glm::ivec2(0.0f);
	}
}

void Texture::setFilter(TextureFilter filter) {
	this->filter = filter;
}

void Texture::setWrap(TextureWrap wrap) {
	this->wrap = wrap;
}
