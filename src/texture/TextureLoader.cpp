#include "TextureLoader.h"

#include <format>
#include <stdexcept>

Texture TextureLoader::loadTexture(const char* filePath) {
	SDL_Surface* surface = SDL_LoadPNG(filePath);
	if (!surface)
		throw std::runtime_error(std::format("[ERROR] Could not load '{0}'! SDL error: {1}", filePath, SDL_GetError()));

	SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);

	SDL_DestroySurface(surface);

	if (!convertedSurface)
		throw std::runtime_error(std::format("[ERROR] Could not convert '{0}'! SDL error: {1}", filePath, SDL_GetError()));

	return Texture(convertedSurface);
}
