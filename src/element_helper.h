// Helper for building list of elements

#ifndef ELEMENT_HELPER_H
#define ELEMENT_HELPER_H

#include <GL/glew.h>

namespace elem_help
{
	const size_t VERTICES_PER_RECT = 6;
	void BuildElements(GLuint *elements, size_t num_items);
} // namespace elem_help

#endif /* ELEMENT_HELPER_H */
