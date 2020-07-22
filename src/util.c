#include <stddef.h>

// Function to prevent libcurl from writing the HTTP response to the console
size_t no_print(void *ptr, size_t size, size_t nmemb, void *data) {
	return size * nmemb;
}

