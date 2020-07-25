// Copyright 2020 Drew M. Johnson

#include <stdio.h>
#include <iostream>
//#include <vector>
#include <curl/curl.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "rapidjson/document.h"
#include "curl_helper.h"

#define GLEW_STATIC

int main(int argc, char *argv[])
{
	//TODO: Consider caching the API response to speed up future runs of program
	
	// Use curl to get JSON from API
	CURL *curl;
	CURLcode curl_error;
	const char* MLB_API_URL = 
		"http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";
	
	struct MemoryStruct chunk;
	chunk.memory = (char*)malloc(1);
	chunk.size = 0;
	
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, MLB_API_URL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		curl_error = curl_easy_perform(curl);

		//TODO: Do some cleaner error handling here
		if (curl_error != CURLE_OK) {
			fprintf(stderr, 
				"curl_easy_perform() failed: %s\n", 
				curl_easy_strerror(curl_error));
			return -1; // Halt program
		}
		
		curl_easy_cleanup(curl);
	}
	
	// Parse JSON
	rapidjson::Document document;
	document.Parse(chunk.memory);
	//printf("totalItems=%d\n", document["totalItems"].GetInt()); //TODO: temp, remove later

	/* TODO: This will be useful later
	rapidjson::Value& games = document["dates"][0]["games"];
	
	
	for (rapidjson::SizeType i = 0; i < games.Size(); i++) {
		printf("Game #%d Details:\n", i);
		printf("%s vs. %s\n",
			games[i]["teams"]["away"]["team"]["name"].GetString(),
			games[i]["teams"]["home"]["team"]["name"].GetString());
	}*/
	
	// Initialize SDL Video subsystem, an OpenGL context, and GLEW
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	// Create and bind vertex array object
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Create and bind vertex buffer object
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// List of points to draw
	float vertices[] = {
		 0.5f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f,
		-0.5f,  0.5f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create vertex shader
	const char* vertexSource = R"glsl(
		#version 150 core

		in vec2 position;

		void main()
		{
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)glsl";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Get vertex shader compilation errors & warnings //TODO make debug only
	GLint compile_status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_TRUE)
		printf("vertex shader_compiled!\n");

	// Create fragment shader
	const char* fragmentSource = R"glsl(
		#version 150 core

		out vec4 outColor;

		void main()
		{
			outColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	)glsl";
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Get fragment shader compilation errors & warnings //TODO make debug only
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_TRUE)
		printf("fragment shader compiled!\n");

	// Additional shader info //TODO: enable for debug
	// char buffer[512];
	// glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	// std::cout << buffer << std::endl;

	// Attach shaders to program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Direct output to framebuffer
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	// Link and use program
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Link vertex data to the shader program attribute
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glDrawArrays(GL_QUADS, 0, 4);

	// Event handling loop
	SDL_Event windowEvent;
	for (;;)
	{
	    if (SDL_PollEvent(&windowEvent))
	    {
	        if (windowEvent.type == SDL_QUIT) break;
	    }

	    SDL_GL_SwapWindow(window);
	}

	// Shutdown procedure
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
