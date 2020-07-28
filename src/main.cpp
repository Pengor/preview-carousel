// Copyright 2020 Drew M. Johnson

#include <stdio.h>
#include <iostream>
//#include <vector>
//#include <curl/curl.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include "rapidjson/document.h"
//#include "curl_helper.h"
#include "game_list.h"

#define GLEW_STATIC

const char *MLB_API_URL =
	"http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";

int main(int argc, char *argv[])
{
	GameList game_list = GameList(MLB_API_URL);

	//TODO: Consider caching the API response to speed up future runs of program

	// Use curl to get JSON from API
	/*CURL *curl;
	CURLcode curl_error;
	
	
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
	free(chunk.memory); //TODO: confirm this works
	//printf("totalItems=%d\n", document["totalItems"].GetInt()); //TODO: temp, remove later

	// TODO: This will be useful later
	// rapidjson::Value& games = document["dates"][0]["games"];
	
	// for (rapidjson::SizeType i = 0; i < games.Size(); i++) {
	// 	printf("Game #%d Details:\n", i);
	// 	printf("%s vs. %s\n",
	// 		games[i]["teams"]["away"]["team"]["name"].GetString(),
	// 		games[i]["teams"]["home"]["team"]["name"].GetString());
	// }
	document.SetObject(); // Clear and minimize the JSON document
	*/

	// Initialize SDL Video subsystem, an OpenGL context, and GLEW
	SDL_Init(SDL_INIT_VIDEO);

	IMG_Init(IMG_INIT_JPG);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window *window = SDL_CreateWindow("Preview Carousel",
										  100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);

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
		-0.95f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.85f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.85f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.95f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.80f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.70f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.70f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.80f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.65f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.55f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.55f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.65f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.50f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.40f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.40f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.50f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.35f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.25f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.25f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.35f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.20f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.10f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.10f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.20f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.05f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.05f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.05f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.05f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.10f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.20f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.20f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.10f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.25f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.35f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.35f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.25f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.40f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.50f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.50f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.40f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.55f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.65f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.65f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.55f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.70f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.80f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.80f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.70f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.85f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.95f, 0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.95f, -0.05f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.85f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and bind element buffer object
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	// List of which vertices to use to form triangles
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,

		24, 25, 26,
		26, 27, 24,

		28, 29, 30,
		30, 31, 28,

		32, 33, 34,
		34, 35, 32,

		36, 37, 38,
		38, 39, 36,

		40, 41, 42,
		42, 43, 40,

		44, 45, 46,
		46, 47, 44,

		48, 49, 50,
		50, 51, 48};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
				 GL_STATIC_DRAW);

	// Create and bind texture objects
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//glGenerateMipmap(GL_TEXTURE_2D); // TODO: Does this belong here?

	SDL_Surface *surface = IMG_Load(/*TODO curled filepath here*/ "release/cut.jpg");

	if (!surface)
	{
		printf("IMG_Load: %s\n", IMG_GetError());
	}

	// TODO: Improvement: dynamically get the closest resolution image URL
	// from the JSON data to lessen the amount of scaling/filtering needed
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, surface->pixels);

	// Create vertex shader
	const char *vertexSource = R"glsl(
		#version 150 core

		in vec2 position;
		in vec3 color;
		in vec2 texcoord;

		out vec3 Color;
		out vec2 Texcoord;

		void main()
		{
			Color = color;
			Texcoord = texcoord;
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
	const char *fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D tex;
    void main()
    {
        outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
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
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	// Link color data to the shader program attribute
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
						  (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(colorAttrib);

	// Link texture data to te shader program attribute
	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
						  (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(texAttrib);

	// Event handling loop
	SDL_Event windowEvent;
	for (;;)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
				break;
		}

		//glUniform3f(uniColor, 0.3f, 0.4f, 0.1f);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
	}

	// Shutdown procedure //TODO: Move to separate function
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
	SDL_GL_DeleteContext(context);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
