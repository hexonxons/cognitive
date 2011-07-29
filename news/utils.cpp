/**
 * \file    utils.cpp
 *
 * \brief   Вспомогательные функции и структуры данных. Определения.
 */

#include "utils.h"

using std::vector;

CTag::CTag(short _Tag, char _IsClose)
{
    tag = _Tag;
    isClose = _IsClose;
}

CTag::CTag()
{
    tag = 0;
    isClose = 0;
}

//#########################################################################################

int vStrCmp(const std::vector<CPair<CTag, CPair<int, int>>> &left,
            const std::vector<CPair<CTag, CPair<int, int>>> &right)
{
    unsigned int i = left.size();
    unsigned int j = right.size();
    if (i == j)
    {
        for (i = 0; i < left.size(); ++i)
        {
            if (left[i].first.tag != right[i].first.tag)
            {
                return left[i].first.tag - right[i].first.tag;
            }
        }
    }
    else
        return i - j;
    return 0;
}

int vIsSubstr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
              const std::vector<CPair<CTag, CPair<int, int>>> &vStr2)
{
    unsigned int j = 0;
    for (unsigned int i = 0; i < vStr1.size(); ++i)
    {
        if (vStr2[j].first.tag == vStr1[i].first.tag)
        {
            ++j;
            ++i;
            while (i < vStr1.size())
            {
                if (j == vStr2.size())
                {
                    return 1;
                }
                if (vStr2[j].first.tag != vStr1[i].first.tag)
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

int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
            const std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
            int offset)
{
    unsigned int j = 0;
    for (unsigned int i = offset; i < vStr1.size(); ++i)
    {
        if (vStr2[j].first.tag == vStr1[i].first.tag)
        {
            ++j;
            ++i;
            while (i < vStr1.size())
            {
                if (j == vStr2.size())
                {
                    return i - j;
                }
                if (vStr2[j].first.tag != vStr1[i].first.tag)
                {
                    j = 0;
                    break;
                }
            }
        }
    }
    return 0;
}

std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2,
                                                            int offset)
{
    vector<CPair<CTag, CPair<int, int>>>::iterator it1 = vStr1.begin() + offset;
    vector<CPair<CTag, CPair<int, int>>>::iterator it2 = vStr2.begin();
    vector<CPair<CTag, CPair<int, int>>>::iterator reti;

    while (1)
    {
        while (it1->first != it2->first)
        {
            ++it1;
            if (it1 == vStr1.end())
            {
                return vStr1.end();
            }
        }
        reti = it1;
        while (it1->first == it2->first)
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

std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
                                                            std::vector<CPair<CTag, CPair<int, int>>> &vStr2)
{
    vector<CPair<CTag, CPair<int, int>>>::iterator it1 = vStr1.begin();
    vector<CPair<CTag, CPair<int, int>>>::iterator it2 = vStr2.begin();
    vector<CPair<CTag, CPair<int, int>>>::iterator reti;

    while (1)
    {
        while (it1->first != it2->first)
        {
            ++it1;
            if (it1 == vStr1.end())
            {
                return vStr1.end();
            }
        }
        reti = it1;
        while (it1->first == it2->first)
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