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

int getWordFreq(const string &src, const string &word)
{
    unsigned int cnt = 0;
    unsigned int i = 0;

    // проходим по всей строке
    for(i = 0; i < src.size(); ++i)
    {
        // если слово нашлось в строке
        if (!strncmp(src.c_str() + i, word.c_str(), word.size()))
        {
            // увеличиваем счетчик повторений
            ++cnt;
            // пропускаем word.size() символов в src (++i)
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

    // проходим по всей строке
    for (i = 0; i < src.size(); ++i)
    {
        // считаем каждое слово
        if (src[i] == '<')
            ++cnt;
        // как досчитали до нужного
        if (cnt == begin + 1)
        {
            // пока не дойдем до конца end-го слова, пишем все в строку ret
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
        // если встретили '/' - тег закрывающий. Увеличиваем счетчик.
        if (src[i] == '/')
            ++cnt;
    // если закрывающих тегов больше, чем открывающих - возвратим 0
    if (cnt > getWordCount(src) / 2 + 1)
        return 0;

    return 1;
}

int checksum(const string &src, int flag)
{
    unsigned int i = 0;
    int cnt = 0;

    for (i = 0; i < src.size(); ++i)
        // если встретили '/' - тег закрывающий. Увеличиваем счетчик.
        if (src[i] < 0)
            ++cnt;
    // если закрывающих тегов больше, чем открывающих - возвратим 0
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
        // получим очередной тег
        str = getTag(i, src);
        // если он - открывающий - положим в стек
        if (strncmp(str.c_str(), "</", 2))
            st.push(str);
        else
        {
            // если стек пуст - закрывающему тегу нет открывающего.
            if (st.empty())
                return 0;
            // возьмем верхний тег
            string topTag = st.top();
            // если он не является открывающим для данного закрывающего
            if (strncmp(topTag.c_str() + 1, str.c_str() + 2, 10))
            {
                // пока не получим открывающий для текущего тега, извлекаем
                // теги из стека и проверяем их
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
    // находим первое совпадение word с src
    const char *ptr = strstr(src.c_str(), word.c_str());
    // строим строчку с 0 символа до начала совпадения
    string temp(src, 0, ptr - src.c_str());
    // возвращаем количество слов в этой строчке
    return getWordCount(temp);
}

int wordSubPtr(const string &src, const string &word, int offset)
{
    // находим первое совпадение word с src, начиная с позиции src + offset
    const char *ptr = strstr(src.c_str() + offset, word.c_str());
    // строим строчку с 0 символа до начала совпадения
    string temp(src, 0, ptr - src.c_str());
    // возвращаем количество слов в этой строчке
    return getWordCount(temp);
}