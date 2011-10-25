/**
 * \file    utils.cpp
 *
 * \brief   Вспомогательные функции и структуры данных. Определения.
 */

#include "utils.h"

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

int vStrStr(const std::vector<CTagDescription> &vStr1,
            const std::vector<CTagDescription> &vStr2,
            int offset)
{
    unsigned int j = 0;
    for (unsigned int i = offset; i < vStr1.size(); ++i)
    {
        if (vStr2[j].nTagCode == vStr1[i].nTagCode)
        {
            ++j;
            ++i;
            while (i < vStr1.size())
            {
                if (j == vStr2.size())
                {
                    return i - j;
                }
                if (vStr2[j].nTagCode != vStr1[i].nTagCode)
                {
                    j = 0;
                    break;
                }
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