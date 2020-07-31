// Parts of this file are provided as an example from curl (getinmemory.c)
// See curl_notice.txt for license
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CURL_HELPER_H
#define CURL_HELPER_H

	struct MemoryStruct
	{
		char *memory;
		size_t size;
	};

	void CurlFromUrl(struct MemoryStruct* mem_struct, const char* url);

#endif /* CURL_HELPER_H */

#ifdef __cplusplus
}
#endif
