#include "Texture.h"
#include "Renderer.h"
#include <SDL_image.h>
#include <iostream>
#include <cassert>

namespace nc
{
	bool Texture::Load(const std::string& filename, void* data)
	{
		assert(data);
		renderer = static_cast<Renderer*>(data)->renderer;
		// load surface
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return false;
		}
		// create texture
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (!texture)
		{
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return false;
		}
		return true;
	}

	glm::vec2 Texture::GetSize() const
	{
		SDL_Point point;
		SDL_QueryTexture(texture, nullptr, nullptr, &point.x, &point.y);
		return glm::vec2{point.x, point.y};
	}

	Texture::Texture(Renderer* renderer)
	{
		assert(renderer);
		this->renderer = renderer->renderer;
	}

	bool Texture::Create(SDL_Surface* surface)
	{
		assert(surface);
		// create texture
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture == nullptr)
		{
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return false;
		}

		return true;
	}
}

