// Copyright 2020 Drew M. Johnson

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "game_list.h"
#include "vertex_helper.h"

#define GLEW_STATIC

const char *MLB_API_URL =
	"http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";

int main(int argc, char *argv[])
{
	// Call API and build list of important details of the day's games
	GameList game_list = GameList(MLB_API_URL);

	size_t active_game_num = 0;

	// Initialize SDL window, modules, and OpenGL context
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	TTF_Init();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window *window =
		SDL_CreateWindow("Preview Carousel",
						 100, 100, 200, 200,
						 SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Initialize GLEW
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
	//float* vertices = vert_help::BuildVertices(0.1f, 0.1f, 1.5f, 13);
	//vert_help::BuildVertices(0.1f, 0.1f, 1.5f, 13);
	float vertices[] = {
		// Background vertices
		-1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		// Game thumbnail vertices
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
		0.85f, -0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		// Headline text vertices
		-0.95f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.85f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.85f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.95f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.80f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.70f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.70f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.80f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.65f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.55f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.55f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.65f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.50f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.40f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.40f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.50f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.35f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.25f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.25f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.35f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.20f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.10f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.10f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.20f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.05f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.05f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.05f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.05f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.10f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.20f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.20f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.10f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.25f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.35f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.35f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.25f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.40f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.50f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.50f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.40f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.55f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.65f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.65f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.55f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.70f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.80f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.80f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.70f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.85f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.95f*1.5f, 0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.95f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.85f*1.5f, 0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		// Detail text vertices
		-0.95f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.85f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.85f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.95f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.80f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.70f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.70f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.80f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.65f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.55f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.55f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.65f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.50f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.40f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.40f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.50f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.35f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.25f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.25f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.35f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.20f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.10f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.10f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.20f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.05f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.05f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.05f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.05f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.10f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.20f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.20f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.10f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.25f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.35f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.35f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.25f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.40f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.50f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.50f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.40f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.55f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.65f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.65f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.55f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.70f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.80f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.80f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.70f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.85f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.95f*1.5f, -0.08f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.95f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.85f*1.5f, -0.1f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		// Focused game thumbnail vertices
		-0.95f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.85f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.85f*1.5f, -0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.95f*1.5f, -0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.80f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.70f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.70f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.80f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.65f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.55f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.55f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.65f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.50f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.40f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.40f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.50f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.35f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.25f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.25f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.35f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.20f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.10f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.10f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.20f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.05f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 0.05f*1.5f,  0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		 0.05f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.05f*1.5f,  -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.10f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.20f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.20f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.10f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.25f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.35f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.35f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.25f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.40f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.50f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.50f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.40f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.55f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.65f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.65f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.55f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.70f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.80f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.80f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.70f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		0.85f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.95f*1.5f, 0.05f*1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.95f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.85f*1.5f, -0.05f*1.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and bind element buffer object
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	// List of which vertices to use to form pairs of triangles
	GLuint elements[(13 * 4 + 1) * vert_help::VERTICES_PER_RECT];
	vert_help::BuildElements(elements, (13 * 4) + 1);
	for (size_t i = 0; i < (13 * 4 + 1) * 6; i++)
	{
		std::cout << elements[i] << std::endl;
	}/*
	elements[0] = 0;
	elements[1] = 1;
	elements[2] = 2;
	elements[3] = 2;
	elements[4] = 3;
	elements[5] = 0;*/
	/*GLuint elements[] = {
		// Background
		 0,  1,  2,
		 2,  3,  0,

		// Game thumbnail
		 4,  5,  6,
		 6,  7,  4,

		 8,  9, 10,
		10, 11,  8,

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
		50, 51, 48,

		52, 53, 54,
		54, 55, 52,

		// Headline text
		 4,  5,  6,
		 6,  7,  4,

		 8,  9, 10,
		10, 11,  8,

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
		50, 51, 48,

		52, 53, 54,
		54, 55, 52,

		// Details text
		 4,  5,  6,
		 6,  7,  4,

		 8,  9, 10,
		10, 11,  8,

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
		50, 51, 48,

		52, 53, 54,
		54, 55, 52
	};*/
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
				 GL_STATIC_DRAW);

	// Setup TTF font
	TTF_Font *font = TTF_OpenFont("res/font/Roboto-Regular.ttf", 16);
	SDL_Color text_color = {0, 0, 0};
#ifdef DEBUG
	if (!font)
	{
		std::cout << TTF_GetError() << std::endl;
	}
#endif

	// Create and bind texture objects 
	size_t num_textures = game_list.GetListSize() * 3 + 1;
	GLuint texture[num_textures];
	glGenTextures(num_textures, texture);

	for (size_t i = 0; i < num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		SDL_Surface *surface;

		if (i >= game_list.GetListSize() * 2 + 1)
		{
			std::cout << "Details!" << std::endl;
			// Render game details text as a surface
			std::string details = game_list.GetDetails((i - (game_list.GetListSize() * 2 + 1)));
			surface = TTF_RenderText_Solid(font, details.data(), text_color);
			
			std::cout << "i: " << (i - (game_list.GetListSize() + 1)) << std::endl;
			//std::cout << "game details(str): " << game_list.GetDetails((i - (game_list.GetListSize() + 1))) << std::endl;
			//std::cout << "game details: " << new std::string(details.data()) << std::endl;
			std::cout << "surface: " << surface->h << " " << surface->w << std::endl;
		}
		else if (i > game_list.GetListSize())
		{
			std::cout << "Headline!" << std::endl;
			// Render game headline text as a surface
			std::string headline = game_list.GetHeadline((i - (game_list.GetListSize() + 1)));
			surface = TTF_RenderText_Solid(font, headline.data(), text_color);
			
			std::cout << "i: " << (i - (game_list.GetListSize() + 1)) << std::endl;
			std::cout << "game headline(str): " << game_list.GetHeadline((i - (game_list.GetListSize() + 1))) << std::endl;
			std::cout << "game headline: " << new std::string(headline.data()) << std::endl;
			std::cout << "surface: " << surface->h << " " << surface->w << std::endl;
		}
		else if (i > 0)
		{
			// Creat a surface for game thumbnails
			game_list.CurlPhoto(i - 1);
			SDL_RWops *rwop = SDL_RWFromMem(game_list.GetPhotoPointer(i - 1), 
											game_list.GetPhotoSize(i - 1));
			surface = IMG_Load_RW(rwop, true);
			game_list.ReleasePhoto(i - 1);
			std::cout << "thumbnails" << std::endl;
		}
		else if (i == 0)
		{
			// Render the background image as a surface
			surface = IMG_Load("res/background.jpg");
			std::cout << "background" << std::endl;
		}

#ifdef DEBUG
		if (!surface)
			std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
#endif //DEBUG

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0,
					 GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

#ifdef DEBUG
	// Get vertex shader compilation errors & warnings
	GLint compile_status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_TRUE)
		std::cout << "Vertex shader successfully compiled!" << std::endl;

	// Additional detailed info
	char err_buffer_vertex[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, err_buffer_vertex);
	std::cout << err_buffer_vertex << std::endl;
#endif //DEBUG

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

#ifdef DEBUG
	// Get fragment shader compilation errors & warnings
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_TRUE)
		std::cout << "Fragment shader successfully compiled!" << std::endl;

	// Additional detailed info
	char err_buffer_fragment[512];
	glGetShaderInfoLog(fragmentShader, 512, NULL, err_buffer_fragment);
	std::cout << err_buffer_fragment << std::endl;
#endif //DEBUG

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
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
						  0);
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

	GLint texUniform = glGetUniformLocation(shaderProgram, "tex");

	// Event handling loop
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
				break;
		}
		
		// Draw triangles
		//for (size_t i = 0; i < 14; i++)
		for (size_t i = 0; i < 1+(13*3); i++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			//TODO: Think there's an issue here with there being more textures
			// than there are elements (mismatch of sizes)
			glUniform1i(texUniform, 0);
			//if (i == active_game_num + 1)
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
						   (GLvoid*)(i * 6 * sizeof(GLuint)));
		}

		// Draw text for active game
		/*for (size_t i = 0; i < 2; i++)
		{
			size_t game_tex_index = active_game_num * i + 1;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[game_tex_index]);
			glUniform1i(texUniform, 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
						   (GLvoid*)(game_tex_index * 6 * sizeof(GLuint)));
		}*/

		SDL_GL_SwapWindow(window);
	}

	// Shutdown procedure
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
	SDL_GL_DeleteContext(context);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
