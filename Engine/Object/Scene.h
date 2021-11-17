#pragma once
#include "Object.h"
#include "../Math/MathTypes.h"
#include "Core/Serializable.h"
#include <list>
#include <memory>
#include <vector>
#include <string>

namespace nc
{
	class Engine;

	class Actor;

	class Renderer;

	class Scene : public Object, public ISerializable
	{
	public:
		void Update(float dt);
		void Draw(Renderer* renderer);

		void AddActor(std::unique_ptr <Actor> actor);
		void RemoveActor(Actor* actor);
		void RemoveAllActors();
		void RemoveByTag(const std::string& tag);
		glm::vec3 SafeLocation(float radius, float buffer);

		Actor* FindActor(const std::string& name);

		template<typename T>
		T* GetActor();

		template<typename T>
		std::vector<T*> GetActors();

		int ActorCount();

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		Engine* engine{ nullptr };
		unsigned int id = 0;

	private:
		std::vector<std::unique_ptr<Actor>> actors;
		std::vector<std::unique_ptr<Actor>> newActors;

		//Makes the distance between actors calculate larger, so they can get closer before colliding.
		float collisionGive = 2.0f;
	};

	template<typename T>
	inline T* Scene::GetActor()
	{
		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get())) return dynamic_cast<T*>(actor.get());
		}
		return nullptr;
	}

	template<typename T>
	inline std::vector<T*> Scene::GetActors()
	{
		std::vector<T*> result;

		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get())) result.push_back(dynamic_cast<T*>(actor.get()));
		}
		return result;
	}
}