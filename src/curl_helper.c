// Parts of this file are provided as an example from curl (getinmemory.c)
// See curl_notice.txt for license

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl_helper.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;
 
	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(ptr == NULL) {
		/* out of memory! */ 
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}
 
	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
 
	return realsize;
}
