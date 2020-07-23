// Copyright 2020 Drew M. Johnson

#include <stdio.h>
#include <vector>
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
	const char* MLB_API_URL = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";
	
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
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_error));
			return -1; // Halt program
		}
		
		curl_easy_cleanup(curl);
	}
	
	// Parse JSON
	rapidjson::Document document;
	document.Parse(chunk.memory);
	printf("totalItems=%d\n", document["totalItems"].GetInt()); //TODO: temp, remove later
	
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

	//TEMP_START
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	printf("%u\n", vertexBuffer);
	//TEMP_END

	// Event handling loop
	SDL_Event windowEvent;
	//while (true)
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

