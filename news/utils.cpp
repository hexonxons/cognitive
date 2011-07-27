#include "utils.h"

CTag::CTag(short _Val1, char _Val2)
{
    tag = _Val1;
    isClose = _Val2;
}
CTag::CTag()
{
    tag = 0;
    isClose = 0;
}

int vStrCmp(const vector<CPair<CTag, CPair<int, int>>> &left,
            const vector<CPair<CTag, CPair<int, int>>> &right)
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

// str2 â str1
int vStrStr(const vector<CPair<CTag, CPair<int, int>>> &vStr1,
            const vector<CPair<CTag, CPair<int, int>>> &vStr2)
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

int vStrStr(const vector<CPair<CTag, CPair<int, int>>> &vStr1,
            const vector<CPair<CTag, CPair<int, int>>> &vStr2,
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