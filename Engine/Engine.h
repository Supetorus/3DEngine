#pragma once

#define CREATE_ENGINE_OBJECT(class) nc::ObjectFactory::Instance().Create<nc::class>(#class);
#define REGISTER_CLASS(CLASS)       nc::ObjectFactory::Instance().Register<CLASS>(#CLASS);

// Systems
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

// Core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Json.h"
#include "Core/Serializable.h"

// Framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h"

// Math
#include "Math/MathTypes.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

//Graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/Material.h"
#include "Graphics/Shader.h"
#include "Graphics/Program.h"
#include "Graphics/VertexBuffer.h"

//Resource
#include "Resource/ResourceSystem.h"

// Objects
#include "Object/Actor.h"

// Components
#include "Component/PhysicsComponent.h"
#include "Component/AudioComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/LightComponent.h"
#include "Component/ModelComponent.h"

#include "Component/FreeCameraController.h"

// Math
#include "Math/MathTypes.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace nc
{
	using ObjectFactory = Singleton<Factory<std::string, Object>>;

	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;

	};
	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());// checks if dynamic cast returns something or null
		}
		return nullptr;
	}
}