#include "Engine.h"
#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

// vertices
const float vertices[] =
{
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f
};

const GLuint indices[] =
{
    0, 2, 1,
    0, 3, 2
};

// vertex shader
const char* vertexSource = R"(
    #version 430 core

	layout(location = 0) in vec3 position;
    layout(location = 1)in vec3 color;

	out vec3 fs_color;
	
	uniform float scale;

    void main()
    {
		fs_color = color;
        gl_Position = vec4(position * scale, 1.0);
    }
)";

// fragment
const char* fragmentSource = R"(
    #version 430 core
	in vec3 fs_color;

    out vec4 outColor;
	
	uniform vec3 tint;

    void main()
    {
        outColor = vec4(fs_color, 1.0) * vec4(tint, 1.0f);
    }
)";

int main(int argc, char** argv)
{
	nc::Engine engine;
	engine.Startup();
	engine.Get<nc::Renderer>()->Create("OpenGL", 800, 600);

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../resources");

	// set vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << buffer;
	}

	// set fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		std::cout << buffer;
	}

	// create shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, buffer);
		std::cout << buffer;
	}

	glUseProgram(shaderProgram);

	// Vertex Array
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Vertex Buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo; // element buffer object
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLubyte*)NULL);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Uniform
	GLuint location = glGetUniformLocation(shaderProgram, "scale");
	float time = 0;

	GLuint tintLocation = glGetUniformLocation(shaderProgram, "tint");
	glm::vec3 tint{ 1.0f, 0.15f, 0.15f };

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

		time += 0.001f;

		glUniform1f(location, std::sin(time));
		glUniform3fv(tintLocation, 1, &tint[0]);

		engine.Get<nc::Renderer>()->BeginFrame();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		engine.Get<nc::Renderer>()->EndFrame();
	}

	return 0;
}