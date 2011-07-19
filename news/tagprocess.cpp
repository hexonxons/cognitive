/**
 * \file    tagprocess.cpp
 *
 * \brief   Определение функций, оперирующих со строками.
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
    // счетчик тегов
    unsigned int cnt = 0;
    unsigned int i = 0;
    string ret;

    // Отсчитываем до начала tagNum`го тега 
    for (i = 0; i < src.size() && cnt != tagNum + 1; ++i)
    {
        if (src[i] == '<')
            ++cnt;
    }
    // заносим в ret '<'
    ret = '<';
    // пока не встретим '>', записываем в ret символы тега
    while (src[i] != '>')
    {
        ret += src[i++];
    }
    // записываем конец тега, '>'
    ret += '>';
    // возвращаем тег
    return ret;
}

void removeTags(vector< pair<int, int> > *tagsPos, const string &src, vector<string> &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;
    string tag;

    // проходим по всем тегам
    for (i = 0; i < tagsPos->size(); ++i)
    {
        // если этот тег уже стертый, то продолжаем
        if ((*tagsPos)[i].first == -1)
            continue;
        // получим тег с данной позиции
        tag = getTag((*tagsPos)[i], src);
        // приведем его к нижнему регистру
        transform(tag.begin(), tag.end(), tag.begin(), tolower);
        // проходим по всему массиву тегов для удаления
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // если тег совпал с тегом для удаления, то стираем его
            if (!strncmp(tag.c_str(), tagsToRemove[j].c_str(), tagsToRemove[j].size() - 1))
            {
                (*tagsPos)[i] = make_pair(-1, -1);
                // идем к следующему тегу
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

    // проходим по всем тегам
    for (i = 0; i < tagsPos->size(); ++i)
    {
        // если этот тег уже стертый, то продолжаем
        if ((*tagsPos)[i].first == -1)
            continue;
        // получим тег с данной позиции
        tag = getTag((*tagsPos)[i], src);
        // приведем его к нижнему регистру
        transform(tag.begin(), tag.end(), tag.begin(), tolower);
        // проходим по всему массиву тегов для удаления
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // если тег совпал с тегом для удаления
            if (!strncmp(tag.c_str(), tagsToRemove[j].first.c_str(),
                tagsToRemove[j].first.size() - 1))
            {
                // стираем тег
                (*tagsPos)[i] = make_pair(-1, -1);
                // пока не получим закрывающий тег
                while (strncmp(tag.c_str(), tagsToRemove[j].second.c_str(),
                    tagsToRemove[j].second.size() - 1))
                {
                    ++i;
                    if ((*tagsPos)[i].first == -1)
                        continue;
                    // получаем следующий тег
                    tag = getTag((*tagsPos)[i], src);
                    // и стираем его
                    (*tagsPos)[i] = make_pair(-1, -1);
                }
                // идем к следующему тегу
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
        // если встретили '/' - тег закрывающий. Увеличиваем счетчик.
        if (src[i] == '/')
            ++cnt;
    // если закрывающих тегов больше, чем открывающих - возвратим 0
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
        // если он - открывающий - положим в стек
        if (src[i] > 0)
            st.push(src[i]);
        else
        {
            // если стек пуст - закрывающему тегу нет открывающего.
            if (st.empty())
                return 0;
            // возьмем верхний тег
            char topTag = st.top();
            // если он не является открывающим для данного закрывающего
            if (topTag > 0)
            {
                // пока не получим открывающий для текущего тега, извлекаем
                // теги из стека и проверяем их
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
    string first(src, 0, src.size() - 1);
    string second(src, 1 ,src.size() - 1);
    set <pair <string, int>, ltstr>::iterator setIter;
    int flag = 0;
    if (first.size() < MINSZ)
        return 1;
    // useless
    if (second.size() < MINSZ)
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
            }
        }
    }
    // в любом случае if не нужен
    if (second.size() == MINSZ)
    {
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
            }
        }
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
        }
    }
    else
        return 0;

    return 0;

}