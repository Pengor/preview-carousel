#include <stdbool.h>
#include <stdio.h>
#include <curl/curl.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "util.h"

#define GLEW_STATIC

int main(int argc, char *argv[])
{
	CURL *curl;
	CURLcode response;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, no_print);

		response = curl_easy_perform(curl);

		if (response != CURLE_OK) {
			//fprintf(stderr, "curl_easy_perform() failed!\n");
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(response));
		}

		curl_easy_cleanup(curl);
	}

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	//TEMP
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	printf("%u\n", vertexBuffer);
	//END_TEMP

	SDL_Event windowEvent;
	while (true)
	{
	    if (SDL_PollEvent(&windowEvent))
	    {
	        if (windowEvent.type == SDL_QUIT) break;
	    }

	    SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

