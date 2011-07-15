/**
 * \file    tagprocess.cpp
 *
 * \brief   ����������� �������, ����������� �� ��������.
 */

#include "tagprocess.h"

using std::make_pair;

string getTag(pair<int, int> &tagPos, const string &src)
{
    string ret(src, tagPos.first, tagPos.second - tagPos.first);
    return ret;
}

string getTag(int tagNum, const string &src)
{
    // ������� �����
    unsigned int cnt = 0;
    unsigned int i = 0;
    string ret;

    // ����������� �� ������ tagNum`�� ���� 
    for (i = 0; i < src.size() && cnt != tagNum + 1; ++i)
    {
        if (src[i] == '<')
            ++cnt;
    }
    // ������� � ret '<'
    ret = '<';
    // ���� �� �������� '>', ���������� � ret ������� ����
    while (src[i] != '>')
    {
        ret += src[i++];
    }
    // ���������� ����� ����, '>'
    ret += '>';
    // ���������� ���
    return ret;
}

void removeTags(vector< pair<int, int> > *tagsPos, const string &src, vector<string> &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;
    string tag;

    // �������� �� ���� �����
    for (i = 0; i < tagsPos->size(); ++i)
    {
        // ���� ���� ��� ��� �������, �� ����������
        if ((*tagsPos)[i].first == -1)
            continue;
        // ������� ��� � ������ �������
        tag = getTag((*tagsPos)[i], src);
        // �������� ��� � ������� ��������
        transform(tag.begin(), tag.end(), tag.begin(), tolower);
        // �������� �� ����� ������� ����� ��� ��������
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // ���� ��� ������ � ����� ��� ��������, �� ������� ���
            if (!strncmp(tag.c_str(), tagsToRemove[j].c_str(), tagsToRemove[j].size() - 1))
            {
                (*tagsPos)[i] = make_pair(-1, -1);
                // ���� � ���������� ����
                break;
            }
        }
    }
}


void removeTags(vector< pair<int, int> > *tagsPos, const string &src, 
                vector< pair<string, string> > &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;
    string tag;

    // �������� �� ���� �����
    for (i = 0; i < tagsPos->size(); ++i)
    {
        // ���� ���� ��� ��� �������, �� ����������
        if ((*tagsPos)[i].first == -1)
            continue;
        // ������� ��� � ������ �������
        tag = getTag((*tagsPos)[i], src);
        // �������� ��� � ������� ��������
        transform(tag.begin(), tag.end(), tag.begin(), tolower);
        // �������� �� ����� ������� ����� ��� ��������
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // ���� ��� ������ � ����� ��� ��������
            if (!strncmp(tag.c_str(), tagsToRemove[j].first.c_str(),
                tagsToRemove[j].first.size() - 1))
            {
                // ������� ���
                (*tagsPos)[i] = make_pair(-1, -1);
                // ���� �� ������� ����������� ���
                while (strncmp(tag.c_str(), tagsToRemove[j].second.c_str(),
                    tagsToRemove[j].second.size() - 1))
                {
                    ++i;
                    if ((*tagsPos)[i].first == -1)
                        continue;
                    // �������� ��������� ���
                    tag = getTag((*tagsPos)[i], src);
                    // � ������� ���
                    (*tagsPos)[i] = make_pair(-1, -1);
                }
                // ���� � ���������� ����
                break;
            }
        }
    }
}

int getWordFreq(const string &src, const string &word)
{
    unsigned int cnt = 0;
    unsigned int i = 0;

    // �������� �� ���� ������
    for(i = 0; i < src.size(); ++i)
    {
        // ���� ����� ������� � ������
        if (!strncmp(src.c_str() + i, word.c_str(), word.size()))
        {
            // ����������� ������� ����������
            ++cnt;
            // ���������� word.size() �������� � src (++i)
            i += word.size() - 1;
        }
    }
    return cnt;
}

string getWordString(const string &src, int begin, int end)
{
    unsigned int i = 0;
    unsigned int cnt = 0;
    string ret;

    // �������� �� ���� ������
    for (i = 0; i < src.size(); ++i)
    {
        // ������� ������ �����
        if (src[i] == '<')
            ++cnt;
        // ��� ��������� �� �������
        if (cnt == begin + 1)
        {
            // ���� �� ������ �� ����� end-�� �����, ����� ��� � ������ ret
            while (cnt != end + 1 && i < src.size())
            {
                ret += src[i];
                if (src[i] == '>')
                    ++cnt;
                ++i;
            }
            return ret;
        }
    }
    return NULL;
}

int getWordCount(const string &src)
{
    unsigned int cnt = 0;
    unsigned int i = 0;

    for (i = 0; i < src.size(); ++i)
    {
        if (src[i] == '>')
            ++cnt;
    }
    return cnt;
}

int checksum(const string &src)
{
    unsigned int i = 0;
    int cnt = 0;

    for (i = 0; i < src.size(); ++i)
        // ���� ��������� '/' - ��� �����������. ����������� �������.
        if (src[i] == '/')
            ++cnt;
    // ���� ����������� ����� ������, ��� ����������� - ��������� 0
    if (cnt > getWordCount(src) / 2 + 1)
        return 0;

    return 1;
}

int checksum(const string &src, int flag)
{
    unsigned int i = 0;
    int cnt = 0;

    for (i = 0; i < src.size(); ++i)
        // ���� ��������� '/' - ��� �����������. ����������� �������.
        if (src[i] < 0)
            ++cnt;
    // ���� ����������� ����� ������, ��� ����������� - ��������� 0
    if (cnt > src.size() / 2 + 1)
        return 0;

    return 1;
}

int checkWordTruePairs(const string &src)
{
    int i;
    stack<string> st;
    string str;

    for(i = 0; i < getWordCount(src); ++i)
    {
        // ������� ��������� ���
        str = getTag(i, src);
        // ���� �� - ����������� - ������� � ����
        if (strncmp(str.c_str(), "</", 2))
            st.push(str);
        else
        {
            // ���� ���� ���� - ������������ ���� ��� ������������.
            if (st.empty())
                return 0;
            // ������� ������� ���
            string topTag = st.top();
            // ���� �� �� �������� ����������� ��� ������� ������������
            if (strncmp(topTag.c_str() + 1, str.c_str() + 2, 10))
            {
                // ���� �� ������� ����������� ��� �������� ����, ���������
                // ���� �� ����� � ��������� ��
                while (strncmp(topTag.c_str() + 1, str.c_str() + 2, 10))
                {
                    st.pop();
                    if (st.empty())
                        return 0;
                    topTag = st.top();
                }
            }
            st.pop();
        }
    }
    return 1;
}

int checkWordTruePairs(const string &src, int flag)
{
    int i;
    stack<char> st;

    for(i = 0; i < src.size(); ++i)
    {
        // ���� �� - ����������� - ������� � ����
        if (src[i] > 0)
            st.push(src[i]);
        else
        {
            // ���� ���� ���� - ������������ ���� ��� ������������.
            if (st.empty())
                return 0;
            // ������� ������� ���
            char topTag = st.top();
            // ���� �� �� �������� ����������� ��� ������� ������������
            if (topTag > 0)
            {
                // ���� �� ������� ����������� ��� �������� ����, ���������
                // ���� �� ����� � ��������� ��
                char ch = topTag + '/';
                while (ch != src[i])
                {
                    st.pop();
                    if (st.empty())
                        return 0;
                    topTag = st.top();
                }
            }
            st.pop();
        }
    }
    return 1;
}

int wordSubPtr(const string &src, const string &word)
{
    // ������� ������ ���������� word � src
    const char *ptr = strstr(src.c_str(), word.c_str());
    // ������ ������� � 0 ������� �� ������ ����������
    string temp(src, 0, ptr - src.c_str());
    // ���������� ���������� ���� � ���� �������
    return getWordCount(temp);
}

int wordSubPtr(const string &src, const string &word, int offset)
{
    // ������� ������ ���������� word � src, ������� � ������� src + offset
    const char *ptr = strstr(src.c_str() + offset, word.c_str());
    // ������ ������� � 0 ������� �� ������ ����������
    string temp(src, 0, ptr - src.c_str());
    // ���������� ���������� ���� � ���� �������
    return getWordCount(temp);
}