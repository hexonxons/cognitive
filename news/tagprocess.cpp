/**
 * \file    tagprocess.cpp
 *
 * \brief   ����������� �������, ����������� �� ��������.
 */

#include "tagprocess.h"

using std::make_pair;
using std::ios;

CNewsFinder::CNewsFinder(char *filename, unsigned int minSize):
    MINSZ(minSize),
    fin(filename, ios::in),
    fout("out", ios::out),
    data((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>()),
    avgLen(0),
    avgFreq(0)
{
}

string CNewsFinder::getTag(pair<int, int> &tagPos, const string &src)
{
    string ret(src, tagPos.first, tagPos.second - tagPos.first);
    return ret;
}

void CNewsFinder::removeTags(vector<string> &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;
    string tag;

    // �������� �� ���� �����
    for (i = 0; i < modifiedTagPosition.size(); ++i)
    {
        // ���� ���� ��� ��� �������, �� ����������
        if (modifiedTagPosition[i].first == -1)
            continue;
        // ������� ��� � ������ �������
        tag = getTag(modifiedTagPosition[i], modifiedData);
        // �������� ��� � ������� ��������
        transform(tag.begin(), tag.end(), tag.begin(), tolower);
        // �������� �� ����� ������� ����� ��� ��������
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // ���� ��� ������ � ����� ��� ��������, �� ������� ���
            if (!strncmp(tag.c_str(), tagsToRemove[j].c_str(), tagsToRemove[j].size() - 1))
            {
                modifiedTagPosition[i] = make_pair(-1, -1);
                // ���� � ���������� ����
                break;
            }
        }
    }
}


void CNewsFinder::removeTags(vector< pair<string, string> > &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;
    string tag;

    // �������� �� ���� �����
    for (i = 0; i < modifiedTagPosition.size(); ++i)
    {
        // ���� ���� ��� ��� �������, �� ����������
        if (modifiedTagPosition[i].first == -1)
            continue;
        // ������� ��� � ������ �������
        tag = getTag(modifiedTagPosition[i], modifiedData);
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
                modifiedTagPosition[i] = make_pair(-1, -1);
                // ���� �� ������� ����������� ���
                while (strncmp(tag.c_str(), tagsToRemove[j].second.c_str(),
                    tagsToRemove[j].second.size() - 1))
                {
                    ++i;
                    if (modifiedTagPosition[i].first == -1)
                        continue;
                    // �������� ��������� ���
                    tag = getTag(modifiedTagPosition[i], modifiedData);
                    // � ������� ���
                    modifiedTagPosition[i] = make_pair(-1, -1);
                }
                // ���� � ���������� ����
                break;
            }
        }
    }
}

int CNewsFinder::getWordCount(const string &src)
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

int CNewsFinder::checksum(const string &src)
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

int CNewsFinder::checkWordTruePairs(const string &src)
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

int CNewsFinder::getStringFreq(const string &src, const string &str, unsigned int pos)
{
    unsigned int freq = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    for (i = 0; i < tableSize; ++i)
    {
        if (table[i][pos] != 0)
        {
            unsigned int temp = i;
            string data;
            while (j < str.size() && temp < tableSize && table[temp][pos + j] != 0)
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

int CNewsFinder::getTagSubs(const string &src, int pos)
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
                unsigned int strFreq = getStringFreq(clearedData, first, pos);
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
                unsigned int strFreq = getStringFreq(clearedData, second, pos + 1);
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


    if (getTagSubs(first, pos) != 0 && 
        getTagSubs(second, pos) != 0)
    {
        setIter = freq.find(make_pair(src, 0));
        if (setIter == freq.end())
        {
            if (checksum(src) && checkWordTruePairs(src))
            {
                unsigned int strFreq = getStringFreq(clearedData, src, pos);
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
string CNewsFinder::getNews(char *srcBegin, const string &newsBegin, const string &newsEnd, unsigned int &offset)
{
    unsigned int i = 0;
    // ���� �������, � ������� ���������� �������
    int begin = strstr(srcBegin + offset, newsBegin.c_str()) - srcBegin;
    if (begin < 0)
    {
        string str;
        return str;
    }
    // �������, �� ������� ������� �������������
    int end = strstr(srcBegin + begin, newsEnd.c_str()) - srcBegin;
    if (end < 0)
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

void CNewsFinder::init(vector<pair<string, string>> &remDoubleTag, vector<string> remTag)
{
    unsigned int i = 0;
    unsigned int j = 0;

    // �������� �� ����� �����
    for(i = 0; i < data.size(); ++i)
    {
        // ���� (��������) ����������� ���
        if (data[i] == '<')
        {
            // � �������, ���� < � js
            if (tagPosition.first != -1)
                tagPosition.first = i;
            // �����, � ����� ������� ����� ���
            tagPosition.first = i;
            modTagPosition.first = j;
            // ���� �� ������ �� �������/����������� ������ - ����������
            while (data[i] != ' ' && data[i] != '>')
            {
                modifiedData += data[i];
                ++j;
                ++i;				
            }
        }
        // ���� ����������� ������ - ����������
        if (data[i] == '>')
        {
            modifiedData += data[i];
            ++j;
            tagPosition.second = i;
            modTagPosition.second = j;
            realTagPosition.push_back(tagPosition);
            modifiedTagPosition.push_back(modTagPosition);
            tagPosition.first = -1;
            tagPosition.second = -1;
        }
    }

    removeTags(remTag);
    removeTags(remDoubleTag);

    // ���������� clearedData
    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (modifiedTagPosition[i].first != -1)
        {
            string tag = getTag(modifiedTagPosition[i], modifiedData);
            int ch = 0;
            if (tag[1] != '/')
                // <tag[1]>           -- tag[1] + 47
                // <tag[1]tag[2]..>   -- tag[1] + tag[2]
                // VV Overflow 256    -> "ch > 0"
                ch = tag[1] + tag[2] + 128;
            else
                // </tag[2]>          -- tag[2] + 47
                // </tag[2][tag[3]..> -- tag[2] + tag[3]
                // VV Overflow 128    -> "ch < 0"
                ch = 128 + (tag[2] + tag[3]) % 128; 
            clearedData += ch;
            clearedTagPosition.push_back(modifiedTagPosition[i]);
        }
    }

    // ������ ������ �������
    tableSize = clearedData.size();
    // �������� ������ �� �������
    table = new short *[tableSize];
    for (i = 0; i < tableSize; ++i)
        table[i] = new short[tableSize];

    // ��������� �������
    for (i = 0; i < tableSize; ++i)
    {
        for (j = 0; j < tableSize; ++j)
        {
            if (clearedData[i] == clearedData[j])
            {
                table[i][j] = 1;
            }
            else
                table[i][j] = 0;
        }
    }
}

void CNewsFinder::getPossibleRanges()
{
    unsigned int i = 0;
    // ������ � table �� ����������
    // diag - ����� ���������
    // diag = 0 - ������� ���������, ��������� 
    int diag = 1;
    while(diag != tableSize)
    {
        for (i = 0; i < tableSize - diag; ++i)
        {
            if (table[i][i + diag] != 0)
            {
                // ��������� �������
                string str;
                int beg = i;
                while (i < tableSize - diag && table[i][i + diag] != 0)
                {
                    str += clearedData[i];
                    ++i;
                }
                // � ��������� ����������� ���� �������(��� � ��������) ���� ������� ��� ������ �������
                if (str.size() >= MINSZ)
                    getTagSubs(str, beg + diag);
            }
        }
        // ������ ��������� ���������
        ++diag;
    }
    // ��������� ������� �����/������� �����
    avgLen = avgLen / freq.size();
    avgFreq = avgFreq / freq.size();
}

void CNewsFinder::getNewsRange()
{
    set <pair <string, int>, CNewsFinder::ltstr>::iterator setIter;
    vector<pair<string, int>>::iterator vectorIter;

    for(setIter = freq.begin(); setIter != freq.end(); ++setIter)
    {
        if (setIter->second >= 8 && setIter->first.size() < avgLen * 1.5 && setIter->second < avgFreq * 1.6)
            possibleTags.push_back(*setIter);
    }
    // ��������� ����
    sort(possibleTags.begin(), possibleTags.end(), CNewsFinder::pred());


    // ���������� ������ ����� � ������ �������� ������ ���� ���������
    vectorIter = possibleTags.end();
    --vectorIter;
    while (vectorIter->second != (vectorIter - 1)->second)
    {
        --vectorIter;
    }

    // ���� �������, � ������� ���������� �������
    int begin = strstr(clearedData.c_str(), vectorIter->first.c_str()) - clearedData.c_str();
    newsBegin = vectorIter->first;
    while (!strstr(vectorIter->first.c_str(), (vectorIter - 1)->first.c_str()) == NULL)
    {
        --vectorIter;
    }
    // �������, �� ������� ������� �������������
    int end = strstr(clearedData.c_str(), (vectorIter - 1)->first.c_str()) - clearedData.c_str();
    newsEnd = (vectorIter - 1)->first;
    // ���� � possibleTags �� ���������� ������ � �����
    if (begin > end)
    {
        int t = begin;
        string ts = newsBegin;
        begin = end;
        newsBegin = newsEnd;
        end  = t;
        newsEnd = ts;
    }
}

void CNewsFinder::writeNews()
{
    char *strBegin = (char *)clearedData.c_str();
    unsigned int offset = 0;
    // �������� ������� ������� � ������� �
    while (strlen(strBegin) > offset)
    {
        string res = getNews(strBegin, newsBegin, newsEnd, offset);
        if (res.empty())
            break;
        fout << res;
    }
}

