#include "Engine.h"

int main(int argc, char** argv)
{
	// Create Engine
	std::unique_ptr<nc::Engine> engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("OpenGL", 800, 600);

	// Create Scene
	std::unique_ptr<nc::Scene> scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../resources");

	// Load Scene
	rapidjson::Document document;
	bool success = nc::json::Load("scenes/main.scn", document);
	scene->Read(document);

	// Create Camera
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 5 };

		{
			auto component = CREATE_ENGINE_OBJECT(CameraComponent);
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}
		{
			auto component = CREATE_ENGINE_OBJECT(FreeCameraController);
			component->speed = 3;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}
		scene->AddActor(std::move(actor));
	}

	// Create Model
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "model";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		auto component = CREATE_ENGINE_OBJECT(ModelComponent);
		component->model = engine->Get<nc::ResourceSystem>()->Get<nc::Model>("models/cube.obj");
		component->material = engine->Get<nc::ResourceSystem>()->Get<nc::Material>("materials/wood.mtl", &engine);

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	// create light
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "light";
		actor->transform.position = glm::vec3{ 4, 1, 4 };

		auto component = CREATE_ENGINE_OBJECT(LightComponent);
		component->ambient = glm::vec3{ 0.2f };
		component->diffuse = glm::vec3{ 1 };
		component->specular = glm::vec3{ 1 };

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	glm::vec3 translate{ 0 };
	float angle = 0;

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();
		engine->Update();
		scene->Update(engine->time.deltaTime);

		// Update Actor
		auto actor = scene->FindActor("model");
		if (actor != nullptr)
		{
			actor->transform.rotation.y += engine->time.deltaTime;
		}

		engine->Get<nc::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<nc::Renderer>()->EndFrame();
	}

	return 0;
}