// Copyright 2020 Drew M. Johnson

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "game_list.h"
#include "element_helper.h"
#include "vertices.h"
#include "shaders.h"

#define GLEW_STATIC

const char *MLB_API_URL =
	"http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";

int main(int argc, char *argv[])
{
	// Call API and build list of important details of the day's games
	GameList game_list = GameList(MLB_API_URL);

	const size_t NUM_THUMBNAILS = 13;
	size_t active_game = 0;
	size_t leftmost_game = 0;
	size_t rightmost_game = NUM_THUMBNAILS - 1;

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

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	// Create and bind vertex array object
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Create and bind vertex buffer object
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// Load list of vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices::array), vertices::array, 
				 GL_STATIC_DRAW);

	// Create and bind element buffer object
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	// List of which vertices to use to form pairs of triangles
	GLuint elements[(NUM_THUMBNAILS * 4 + 1) * elem_help::VERTICES_PER_RECT];
	elem_help::BuildElements(elements, (NUM_THUMBNAILS * 4) + 1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
				 GL_STATIC_DRAW);

	// Setup TTF font
	TTF_Font *font = TTF_OpenFont("res/font/Roboto-Regular.ttf", 16);
	SDL_Color text_color = {255, 255, 255};
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

	GLint color_format = GL_RGB;

	for (size_t i = 0; i < num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		SDL_Surface *surface;

		if (i >= game_list.GetListSize() * 2 + 1)
		{
			// Render game details text as a surface
			std::string details = game_list.GetDetails(
				i - (game_list.GetListSize() * 2 + 1));
			surface = TTF_RenderText_Blended_Wrapped(font, details.data(), 
													 text_color, 128);
			color_format = GL_RGBA;
		}
		else if (i > game_list.GetListSize())
		{
			// Render game headline text as a surface
			std::string headline = game_list.GetHeadline(
				i - (game_list.GetListSize() + 1));
			surface = TTF_RenderText_Blended_Wrapped(font, headline.data(), 
													 text_color, 128);
			color_format = GL_RGBA;
		}
		else if (i > 0)
		{
			// Creat a surface for game thumbnails
			game_list.CurlPhoto(i - 1);
			SDL_RWops *rwop = SDL_RWFromMem(game_list.GetPhotoPointer(i - 1), 
											game_list.GetPhotoSize(i - 1));
			surface = IMG_Load_RW(rwop, true);
			game_list.ReleasePhoto(i - 1);
		}
		else if (i == 0)
		{
			// Render the background image as a surface
			surface = IMG_Load("res/background.jpg");
		}

#ifdef DEBUG
		if (!surface)
			std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
#endif //DEBUG
		glTexImage2D(GL_TEXTURE_2D, 0, color_format, surface->w, surface->h, 0,
					 color_format, GL_UNSIGNED_BYTE, surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Create vertex shader
	// const char *vertexSource = R"glsl(
	// 	#version 150 core

	// 	in vec2 position;
	// 	//in vec3 color;
	// 	in vec4 color;
	// 	in vec2 texcoord;

	// 	//out vec3 Color;
	// 	out vec4 Color;
	// 	out vec2 Texcoord;

	// 	void main()
	// 	{
	// 		Color = color;
	// 		Texcoord = texcoord;
	// 		gl_Position = vec4(position, 0.0, 1.0);
	// 	}
	// )glsl";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shaders::vertexSource, NULL);
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
	// const char *fragmentSource = R"glsl(
	// 	#version 150 core
		
	// 	//in vec3 Color;
	// 	in vec4 Color;
	// 	in vec2 Texcoord;
		
	// 	out vec4 outColor;
	// 	uniform sampler2D tex;
		
	// 	void main()
	// 	{
	// 		//outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
	// 		outColor = texture(tex, Texcoord) * Color;
	// 	}
	// )glsl";
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaders::fragmentSource, NULL);
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
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  0);
	glEnableVertexAttribArray(posAttrib);

	// Link color data to the shader program attribute
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(colorAttrib);

	// Link texture data to te shader program attribute
	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(texAttrib);

	GLint texUniform = glGetUniformLocation(shaderProgram, "tex");

	// Event handling & drawing loop
	SDL_Event windowEvent;
	while (true)
	{
		// Handle left & right keypresses and window closure
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
			{
				break;
			}
			else if (windowEvent.type == SDL_KEYDOWN)
			{
				switch (windowEvent.key.keysym.sym)
				{
				case SDLK_LEFT:
					if (active_game == leftmost_game && leftmost_game > 0)
					{
						leftmost_game--;
						rightmost_game--;
					}
					if (active_game > 0)
						active_game--;
					break;
				case SDLK_RIGHT:
					if (active_game == rightmost_game && 
						rightmost_game < game_list.GetListSize() - 1)
					{
						rightmost_game++;
						leftmost_game++;
					}
					if (active_game < game_list.GetListSize() - 1)
						active_game++;
					break;
				}
			}
		}
		
		// Draw triangles
		for (size_t i = 0; i < num_textures; i++)
		{
			size_t element_location;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glUniform1i(texUniform, 0);

			if (i == 0)
			{
				// Draw background
				element_location = 0;
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
							   (GLvoid*)(element_location));
			}
			else if (i >= leftmost_game + 1 && i <= rightmost_game + 1)
			{
				if (i != active_game + 1)
					element_location = (i - leftmost_game) * 6 * sizeof(GLuint);
				else
					element_location = (i - leftmost_game + NUM_THUMBNAILS * 3) * 6 * sizeof(GLuint);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
							   (GLvoid*)(element_location));
			}
			else if (i == game_list.GetListSize() * 2 + active_game + 1 &&
					 i >= game_list.GetListSize() * 2 + leftmost_game + 1 &&
					 i <= game_list.GetListSize() * 2 + rightmost_game + 1)
			{
				element_location = (i /*- 1*/ - 2 * (game_list.GetListSize() - NUM_THUMBNAILS) - leftmost_game) * 6 * sizeof(GLuint);
				//element_location = (i)
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
							   (GLvoid*)(element_location));
			}
			else if (i == game_list.GetListSize() + active_game + 1 &&
					 i >= game_list.GetListSize() + leftmost_game + 1 &&
					 i <= game_list.GetListSize() + rightmost_game + 1)
			{
				element_location = (i /*+ 1*/ - (game_list.GetListSize() - NUM_THUMBNAILS) - leftmost_game)  * 6 * sizeof(GLuint);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
							   (GLvoid*)(element_location));
			}
			//else
			//{
				//std::cout << "undrawn texture: " << i << std::endl;
			//}
			
		}
		
		SDL_GL_SwapWindow(window);
		glClear(GL_COLOR_BUFFER_BIT);
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
