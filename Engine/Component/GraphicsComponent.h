#pragma once
#include "Component/Component.h"

namespace nc
{
	class Renderer;

	class GraphicsComponent : public Component
	{
	public:
		virtual void Draw(Renderer* renderer) = 0;
	};
}
