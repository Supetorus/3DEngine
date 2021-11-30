#include "Engine.h"
#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

// vertices
const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

const GLuint indices[] =
{
	// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
};

int main(int argc, char** argv)
{
	// Create Engine
	nc::Engine engine;
	engine.Startup();
	engine.Get<nc::Renderer>()->Create("OpenGL", 800, 600);

	// Create Scene
	std::unique_ptr<nc::Scene> scene = std::make_unique<nc::Scene>();
	scene->engine = &engine;

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../resources");

	std::shared_ptr<nc::Program> program = engine.Get<nc::ResourceSystem>()->Get<nc::Program>("basic_shader");
	std::shared_ptr<nc::Shader> vshader = engine.Get<nc::ResourceSystem>()->Get<nc::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nc::Shader> fshader = engine.Get<nc::ResourceSystem>()->Get<nc::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	// Vertex Buffers
	std::shared_ptr<nc::VertexIndexBuffer> vertexBuffer = engine.Get<nc::ResourceSystem>()->Get<nc::VertexIndexBuffer>("cube_mesh");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(float), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(float), 3 * sizeof(float));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(float), 6 * sizeof(float));

	// Texture
	auto texture = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("textures/llama.png");
	texture->Bind();

	texture = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("textures/rocks.bmp");
	texture->Bind();

	texture = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("textures/wood.png");
	texture->Bind();

	// Create Camera
	{
		auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Actor");
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 10 };

		{
			auto component = nc::ObjectFactory::Instance().Create<nc::CameraComponent>("CameraComponent");
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}
		{
			auto component = nc::ObjectFactory::Instance().Create<nc::FreeCameraController>("FreeCameraController");
			component->speed = 3;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}
		scene->AddActor(std::move(actor));
	}

	// Create Cube
	{
		auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Actor");
		actor->name = "cube";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		auto component = nc::ObjectFactory::Instance().Create<nc::MeshComponent>("MeshComponent");
		component->program = engine.Get<nc::ResourceSystem>()->Get<nc::Program>("basic_shader");
		component->vertexBuffer = engine.Get<nc::ResourceSystem>()->Get<nc::VertexIndexBuffer>("cube_mesh");

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
		engine.Update();
		scene->Update(engine.time.deltaTime);

		// Update Actor
		auto actor = scene->FindActor("cube");
		if (actor != nullptr)
		{
			actor->transform.rotation.y += engine.time.deltaTime;
		}

		engine.Get<nc::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine.Get<nc::Renderer>()->EndFrame();
	}

	return 0;
}