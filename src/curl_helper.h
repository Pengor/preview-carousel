// Parts of this file are provided as an example from curl (getinmemory.c)
// See curl_notice.txt for license
#ifdef __cplusplus
extern "C" {
#endif

#ifndef CURL_HELPER_H
#define CURL_HELPER_H

struct MemoryStruct {
	char *memory;
	size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *data);

#endif /* CURL_HELPER_H */

#ifdef __cplusplus
}
#endif
