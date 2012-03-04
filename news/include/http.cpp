#include <string>
#include <curl/curl.h>

#include "http.h"

struct MemoryStruct 
{
    char *memory;
    size_t size;
};

static unsigned int WriteMemoryCallback(void *contents, unsigned int size, unsigned int nmemb, void *userp)
{
  unsigned int realsize = size * nmemb;
  struct MemoryStruct *mem = (MemoryStruct *)userp;
 
  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL)
  {
    printf("not enough memory (realloc returned NULL)\n");
    exit(EXIT_FAILURE);
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
 
char *GetPageSource(const char *URL)
{
  CURL *curl_handle;
 
  struct MemoryStruct chunk;
 
  chunk.memory = (char *)malloc(1);
  chunk.size = 0;
 
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, URL);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_perform(curl_handle);
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
 
  return chunk.memory;
}