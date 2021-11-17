#pragma once
#include "Resource/Resource.h"
#include "Math/MathTypes.h"
#include <SDL.h>

namespace nc
{
	class Renderer;
	class Texture : public Resource
	{
	public:

		glm::vec2 GetSize() const;

		friend class Renderer;

		Texture() {}
		Texture(Renderer* renderer);
		bool Create(SDL_Surface* surface);
		bool Load(const std::string& filename, void* data) override;

	public:
		SDL_Texture* texture{ nullptr };

	private:
		SDL_Renderer* renderer{ nullptr };
	};
}