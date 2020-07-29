#include <GL/glew.h>
#include "element_helper.h"

void elem_help::BuildElements(GLuint* elements, size_t num_items)
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
