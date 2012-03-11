/**
 * \file    utils.cpp
 *
 * \brief   Вспомогательные функции и структуры данных. Определения.
 */

#include "stdafx.h"
#include <curl/curl.h>
using std::vector;

//#########################################################################################

int vStrCmp(const std::vector<CTagDescription> &left,
            const std::vector<CTagDescription> &right)
{
    unsigned int i = left.size();
    unsigned int j = right.size();
    if (i == j)
    {
        for (i = 0; i < left.size(); ++i)
        {
            if (left[i].nTagCode != right[i].nTagCode)
            {
                return left[i].nTagCode - right[i].nTagCode;
            }
        }
    }
    else
        return i - j;
    return 0;
}

int vIsSubstr(const std::vector<CTagDescription> &vStr1,
              const std::vector<CTagDescription> &vStr2)
{
    unsigned int j = 0;
    for (unsigned int i = 0; i < vStr1.size(); ++i)
    {
        if (vStr2[j].nTagCode == vStr1[i].nTagCode)
        {
            ++j;
            ++i;
            while (i < vStr1.size())
            {
                if (j == vStr2.size())
                {
                    return 1;
                }
                if (vStr2[j].nTagCode != vStr1[i].nTagCode)
                {
                    j = 0;
                    break;
                }
                ++i;
                ++j;
            }
            if (j == vStr2.size())
            {
                return 1;
            }
        }
    }
    return 0;
}

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                              std::vector<CTagDescription> &vStr2,
                                              int offset)
{
    vector<CTagDescription>::iterator it1 = vStr1.begin() + offset;
    vector<CTagDescription>::iterator it2 = vStr2.begin();
    vector<CTagDescription>::iterator reti;

    while (1)
    {
        while (it1->nTagCode != it2->nTagCode)
        {
            ++it1;
            if (it1 == vStr1.end())
            {
                return vStr1.end();
            }
        }
        reti = it1;
        while (it1->nTagCode == it2->nTagCode)
        {
            ++it1;
            ++it2;
            if (it2 == vStr2.end())
            {
                return reti;
            }
            if (it1 == vStr1.end())
            {
                return vStr1.end();
            }
        }
        it2 = vStr2.begin();
    }
}

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                              std::vector<CTagDescription> &vStr2)
{
    vector<CTagDescription>::iterator it1 = vStr1.begin();
    vector<CTagDescription>::iterator it2 = vStr2.begin();
    vector<CTagDescription>::iterator reti;

    while (1)
    {
        while (it1->nTagCode != it2->nTagCode)
        {
            ++it1;
            if (it1 == vStr1.end())
            {
                return vStr1.end();
            }
        }
        reti = it1;
        while (it1->nTagCode == it2->nTagCode)
        {
            ++it1;
            ++it2;
            if (it2 == vStr2.end())
            {
                return reti;
            }
            if (it1 == vStr1.end())
            {
                return vStr1.end();
            }
        }
        it2 = vStr2.begin();
    }
}

CTagDescription::CTagDescription()
{
    Clear();
}

void CTagDescription::Clear()
{
    this->bIsClose = 0;
    this->nTagCode = -1;
    this->nTagBegin = -1;
    this->nTagEnd = -1;
}

struct MemoryStruct 
{
    char *memory;
    size_t size;
};

unsigned int WriteMemoryCallback(void *contents, unsigned int size, unsigned int nmemb, void *userp)
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