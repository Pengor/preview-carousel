#include <GL/glew.h>
#include <iostream> //TODO TEMP
#include "vertex_helper.h"

float* vert_help::BuildVertices(float height, float width, float scale, size_t num_items)
{
	//Format:
	// 2 floats for position coordinates
	// 3 floats for rgb values
	// 2 floats for texture coordinates
	const size_t VERTEX_SIZE = 7;

	// 4 sets (of 4 vertices) for each item:
	// game thumbnail
	// headline text
	// details text
	// focused thumbnail
	const size_t CORNERS = 4;
	const size_t SETS_PER_ITEM = 4;

	// 1 additional set of 4 vertices for background
	size_t BACKGROUND = 1;

	size_t array_length = (BACKGROUND + num_items * SETS_PER_ITEM) * 
							CORNERS * VERTEX_SIZE;

	float vertices[BACKGROUND + num_items * SETS_PER_ITEM][CORNERS]
					[VERTEX_SIZE];

	for (size_t i = 0; i < BACKGROUND + num_items * SETS_PER_ITEM; i++)
	{
		for (size_t j = 0; j < CORNERS; j++)
		{
			for (size_t k = 0; k < VERTEX_SIZE; k++)
			{
				vertices[i][j][k] = (float)(0.01f * i + 0.001f * j + 0.0001f * k);
			}
		}
	}
	//memset()
	//float* rtn[0][1] = vertices;

	float* ptr = **vertices;
	for (size_t i = 0; i < array_length; i++)
	{
		std::cout << *(ptr + i) << std::endl;
	}

	return **vertices;
}
void vert_help::BuildElements(GLuint* elements, size_t num_items)
{
	GLuint vertex_counter = 0;

	for (size_t i = 0; i < num_items * VERTICES_PER_RECT; i += VERTICES_PER_RECT)
	{
		elements[i]     = vertex_counter++;
		elements[i + 1] = vertex_counter++;
		elements[i + 2] = vertex_counter;
		elements[i + 3] = vertex_counter++;
		elements[i + 4] = vertex_counter++;
		elements[i + 5] = elements[i];
	}
}
