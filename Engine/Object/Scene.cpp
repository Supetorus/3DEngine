#include "Scene.h"
#include "Math/Random.h"
#include "Engine.h"
#include <algorithm>

namespace nc
{
	void Scene::Update(float dt)
	{
		// add new actors
		actors.insert(actors.end(), std::make_move_iterator(newActors.begin()), std::make_move_iterator(newActors.end()));
		newActors.clear();

		// update actors
		std::for_each(actors.begin(), actors.end(), [dt](auto& actor) { actor->Update(dt); });
		
		// destroy actors
		auto iter = actors.begin();
		while (iter != actors.end())
		{
			if ((*iter)->destroy)
			{
				iter = actors.erase(iter);
			}
			else {
				iter++;
			}
		}
	}

	bool Scene::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool Scene::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("actors") && value["actors"].IsArray())
		{
			for (auto& actorValue : value["actors"].GetArray())
			{
				std::string type;
				JSON_READ(actorValue, type);

				bool prototype = false;
				JSON_READ(actorValue, prototype);

				auto actor = ObjectFactory::Instance().Create<Actor>(type);
				if (actor)
				{
					actor->scene = this;
					actor->Read(actorValue);

					if (prototype)
					{
						std::string name = actor->name;
						ObjectFactory::Instance().RegisterPrototype<Actor>(name, std::move(actor));
					}
					else
					{
						AddActor(std::move(actor));
					}
				}
			}
		}

		return true;
	}

	void Scene::Draw(Renderer* renderer)
	{
		std::for_each(actors.begin(), actors.end(), [renderer](auto& actor) { actor->Draw(renderer); });
	}

	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor->scene = this;
		actor->Intitialize();

		id++;
		actor->id = id;
		newActors.push_back(std::move(actor));
	}

	void Scene::RemoveActor(Actor* actor)
	{
	}

	void Scene::RemoveAllActors()
	{
		actors.clear();
	}

	void Scene::RemoveByTag(const std::string& tag)
	{
		auto iter = actors.begin();
		while (iter != actors.end())
		{
			if ((*iter)->hasTag(tag))
			{
				(*iter)->destroy = true;
			}
			iter++;
		}
	}

	//glm::vec3 Scene::SafeLocation(float radius, float buffer)
	//{
	//	nc::glm::vec3 location;
	//	bool invalidLocation = true;
	//	while (invalidLocation)
	//	{
	//		location =
	//		{
	//			nc::RandomRange(0.0f, engine->Get<nc::Renderer>()->GetWidth() * 1.0f),
	//			nc::RandomRange(0.0f, engine->Get<nc::Renderer>()->GetHeight() * 1.0f)
	//		};
	//		for (std::unique_ptr<Actor>& actor : actors)
	//		{
	//			if (glm::vec3::Distance(location, actor->transform.position) < radius + buffer + actor->GetRadius()) break;
	//			invalidLocation = false;
	//		}
	//	}
	//	return location;
	//}

	Actor* Scene::FindActor(const std::string& name)
	{
		for (auto& actor : actors)
		{
			if (actor->name == name) return actor.get();
		}
		return nullptr;
	}

	int Scene::ActorCount()
	{
		return actors.size();
	}
}