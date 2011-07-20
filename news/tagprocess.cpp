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

int checkWordTruePairs(const string &src)
{
    unsigned int i;
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
                char ch = topTag % 128 - 128;
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

int getStringFreq(const string &src, const string &str, short **table, 
                  unsigned int tableSz, int pos)
{
    unsigned int freq = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    for (i = 0; i < tableSz; ++i)
    {
        if (table[i][pos] != 0)
        {
            unsigned int temp = i;
            string data;
            while (j < str.size() && temp < tableSz && table[temp][pos + j] != 0)
            {
                data += src[temp];
                ++temp;
                ++j;
            }
            if (!strcmp(str.c_str(), data.c_str()))
                ++freq;
            j = 0;
        }
    }
    return freq;
}

int getTagSubs(set <pair <string, int>, ltstr> &freq, const string &src, const string &dataString,
               short **table, unsigned int tableSz, int pos, int &avgLen, int &avgFreq)
{
    // �������� ��� ��������� ����� �� 1 ������
    string first(src, 0, src.size() - 1);
    string second(src, 1 ,src.size() - 1);
    set <pair <string, int>, ltstr>::iterator setIter;
    int flag = 0;

    if (first.size() < MINSZ)
        return 1;

    if (first.size() == MINSZ)
    {
        setIter = freq.find(make_pair(first, 0));
        if (setIter == freq.end())
        {
            if (checksum(first) && checkWordTruePairs(first))
            {
                int strFreq = getStringFreq(dataString, first, table, tableSz, pos);
                if (strFreq >= MINSZ)
                {
                    freq.insert(make_pair(first, strFreq));
                    avgLen += first.size();
                    avgFreq += strFreq;
                }
                else
                    flag = 1;
            }
        }
        setIter = freq.find(make_pair(second, 0));
        if (setIter == freq.end())
        {
            if (checksum(second) && checkWordTruePairs(second))
            {
                int strFreq = getStringFreq(dataString, second, table, tableSz, pos + 1);
                if (strFreq >= MINSZ)
                {
                    freq.insert(make_pair(second, strFreq));
                    avgLen += second.size();
                    avgFreq += strFreq;
                }
                else
                    flag = 1;
            }
        }
        if (flag)
            return 0;
        return 1;
    }

    if (getTagSubs(freq, first, dataString, table, tableSz, pos, avgLen, avgFreq) != 0 && 
        getTagSubs(freq, second, dataString, table, tableSz, pos, avgLen, avgFreq) != 0)
    {
        setIter = freq.find(make_pair(src, 0));
        if (setIter == freq.end())
        {
            if (checksum(src) && checkWordTruePairs(src))
            {
                int strFreq = getStringFreq(dataString, src, table, tableSz, pos);
                if (strFreq < MINSZ)
                    return 0;
                freq.insert(make_pair(src, strFreq));
                avgLen += src.size();
                avgFreq += strFreq;
            }
            else
                return 0;
        }
    }
    else
        return 0;
    return 1;
}
string getNews(const string &data, char *srcBegin, const string &newsBegin, const string &newsEnd,
               vector< pair<int, int> > &clearedTagPosition, vector< pair<int, int> > &modifiedTagPosition,
               vector< pair<int, int> > &realTagPosition, int &offset)
{
    int i = 0;
    // ���� �������, � ������� ���������� �������
    int begin = strstr(srcBegin + offset, newsBegin.c_str()) - srcBegin;
    // �������, �� ������� ������� �������������
    int end = strstr(srcBegin + begin, newsEnd.c_str()) - srcBegin;
    if (end < 0 || begin < 0)
    {
        string str;
        return str;
    }
    offset = end + 1;
    // ����� ����, � �������� ������� ���������� � �������������, � modifiedTagPosition
    int rbegin;
    int rend;

    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (clearedTagPosition[begin].first == modifiedTagPosition[i].first)
            rbegin = i;
        if (clearedTagPosition[end].second == modifiedTagPosition[i].second)
            rend = i;
    }
    string ret(data, realTagPosition[rbegin].first, 
        realTagPosition[rend].first - realTagPosition[rbegin].first);
    return ret;

}