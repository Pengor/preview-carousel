#ifndef VERTEX_HELPER_H
#define VERTEX_HELPER_H

//#include <stddef.h>
#include <GL/glew.h>

namespace vert_help
{
	const size_t VERTICES_PER_RECT = 6;

	float* BuildVertices(float height, float width, float scale, size_t num_items);
	void BuildElements(GLuint* elements, size_t num_items);
}

#endif /* VERTEX_HELPER_H */