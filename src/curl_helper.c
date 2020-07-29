// Parts of this file are provided as an example from curl (getinmemory.c)
// See curl_notice.txt for license

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "curl_helper.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
						   void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL)
	{
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

void CurlFromUrl(struct MemoryStruct* mem_struct, const char* url)
{
	mem_struct->memory = (char *)malloc(1);
	mem_struct->size = 0;

	CURL *curl = curl_easy_init();
	
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)mem_struct);

#ifdef DEBUG
		CURLcode curl_error = curl_easy_perform(curl);

		// Print any curl errors
		if (curl_error != CURLE_OK)
		{
			fprintf(stderr,
					"curl_easy_perform() failed: %s\n",
					curl_easy_strerror(curl_error));
		}
#else
		curl_easy_perform(curl);
#endif //DEBUG
		curl_easy_cleanup(curl);
	}
}
