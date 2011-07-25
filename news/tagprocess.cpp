/**
 * \file    tagprocess.cpp
 *
 * \brief   Определение функций, оперирующих со строками.
 */

#include "tagprocess.h"

using std::make_pair;
using std::ios;

CNewsFinder::CNewsFinder(char *filename, unsigned int minSize, unsigned int minFreq):
    MINSZ(minSize),
    m_fileIn(filename, ios::in),
    m_fileOut("out", ios::out),
    m_fileData((std::istreambuf_iterator<char>(m_fileIn)), std::istreambuf_iterator<char>()),
    m_avgLen(0),
    m_avgFreq(0),
    m_minFreq(minFreq)
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

    // проходим по всем тегам
    for (i = 0; i < m_modifiedTagPosition.size(); ++i)
    {
        // если этот тег уже стертый, то продолжаем
        if (m_modifiedTagPosition[i].first == -1)
            continue;
        // получим тег с данной позиции
        tag = getTag(m_modifiedTagPosition[i], m_modifiedData);
        // приведем его к нижнему регистру
        LowerCase(&tag);
        // проходим по всему массиву тегов для удаления
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // если тег совпал с тегом для удаления, то стираем его
            if (!strncmp(tag.c_str(), tagsToRemove[j].c_str(), tagsToRemove[j].size() - 1))
            {
                m_modifiedTagPosition[i] = make_pair(-1, -1);
                // идем к следующему тегу
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

    // проходим по всем тегам
    for (i = 0; i < m_modifiedTagPosition.size(); ++i)
    {
        // если этот тег уже стертый, то продолжаем
        if (m_modifiedTagPosition[i].first == -1)
            continue;
        // получим тег с данной позиции
        tag = getTag(m_modifiedTagPosition[i], m_modifiedData);
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
                m_modifiedTagPosition[i] = make_pair(-1, -1);
                // пока не получим закрывающий тег
                while (strncmp(tag.c_str(), tagsToRemove[j].second.c_str(),
                    tagsToRemove[j].second.size() - 1))
                {
                    ++i;
                    if (m_modifiedTagPosition[i].first == -1)
                        continue;
                    // получаем следующий тег
                    tag = getTag(m_modifiedTagPosition[i], m_modifiedData);
                    // и стираем его
                    m_modifiedTagPosition[i] = make_pair(-1, -1);
                }
                // идем к следующему тегу
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
        // если встретили '/' - тег закрывающий. Увеличиваем счетчик.
        if (src[i] == '/')
            ++cnt;
    // если закрывающих тегов больше, чем открывающих - возвратим 0
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

int CNewsFinder::getStringFreq(const string &src, const string &str, unsigned int pos)
{
    unsigned int freq = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    for (i = 0; i < m_tableSize; ++i)
    {
        if (m_pTable[i][pos] != 0)
        {
            unsigned int temp = i;
            string data;
            while (j < str.size() && temp < m_tableSize && m_pTable[temp][pos + j] != 0)
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
    // выбираем две подстроки длины на 1 меньше
    string first(src, 0, src.size() - 1);
    string second(src, 1 ,src.size() - 1);
    set <string>::iterator setIter;
    int flag = 0;

    if (first.size() < MINSZ)
        return 1;

    if (first.size() == MINSZ)
    {
        setIter = m_subsArr.find(first);
        if (setIter == m_subsArr.end())
        {
            m_subsArr.insert(first);
            if (checksum(first) && checkWordTruePairs(first))
            {
                unsigned int strFreq = getStringFreq(m_clearedData, first, pos);
                if (strFreq >= m_minFreq)
                {
                    m_freq.insert(make_pair(first, strFreq));
                    m_avgLen += first.size();
                    m_avgFreq += strFreq;
                }
                else
                    flag = 1;
            }
        }
        setIter = m_subsArr.find(second);
        if (setIter == m_subsArr.end())
        {
            m_subsArr.insert(second);
            if (checksum(second) && checkWordTruePairs(second))
            {
                unsigned int strFreq = getStringFreq(m_clearedData, second, pos + 1);
                if (strFreq >= m_minFreq)
                {
                    m_freq.insert(make_pair(second, strFreq));
                    m_avgLen += second.size();
                    m_avgFreq += strFreq;
                }
                else
                    flag = 1;
            }
        }
        if (flag)
            return 0;
        return 1;
    }

    setIter = m_subsArr.find(src);
    if (setIter == m_subsArr.end())
    {
        m_subsArr.insert(src);
        if (getTagSubs(first, pos) != 0 && getTagSubs(second, pos + 1) != 0)
        {
            if (checksum(src) && checkWordTruePairs(src))
            {
                unsigned int strFreq = getStringFreq(m_clearedData, src, pos);
                if (strFreq < m_minFreq)
                    return 0;
                m_freq.insert(make_pair(src, strFreq));
                m_avgLen += src.size();
                m_avgFreq += strFreq;
            }
        }
        else
            return 0;
    }
    
    return 1;
}
string CNewsFinder::getNews(char *srcBegin, const string &newsBegin, const string &newsEnd, unsigned int &offset)
{
    unsigned int i = 0;
    // Ищем позицию, с которой начинается новость
    int begin = strstr(srcBegin + offset, newsBegin.c_str()) - srcBegin;
    if (begin < 0)
    {
        string str;
        return str;
    }
    // Позиция, на которой новость заканчивается
    int end = strstr(srcBegin + begin, newsEnd.c_str()) - srcBegin;
    if (end < 0)
    {
        string str;
        return str;
    }
    offset = end + 1;
    // номер тега, с которого новость начинается и заканчивается, в modifiedTagPosition
    int rbegin;
    int rend;

    for(i = 0; i < m_modifiedTagPosition.size(); ++i)
    {
        if (m_clearedTagPosition[begin].first == m_modifiedTagPosition[i].first)
            rbegin = i;
        if (m_clearedTagPosition[end].second == m_modifiedTagPosition[i].second)
            rend = i;
    }
    string ret(m_fileData, m_realTagPosition[rbegin].first, 
        m_realTagPosition[rend].first - m_realTagPosition[rbegin].first);
    return ret;
}

void CNewsFinder::init(vector<pair<string, string>> &remDoubleTag, vector<string> &remTag)
{
    unsigned int i = 0;
    unsigned int j = 0;

    // проходим по всему файлу
    for(i = 0; i < m_fileData.size(); ++i)
    {
        // если (возможно) открывающий тег
        if (m_fileData[i] == '<')
        {
            // к примеру, знак < в js
            if (tagPosition.first != -1)
                tagPosition.first = i;
            // знаем, с какой позиции лежит тег
            tagPosition.first = i;
            modTagPosition.first = j;
            // пока не дойдем до пробела/закрывающей скобки - записываем
            while (m_fileData[i] != ' ' && m_fileData[i] != '>')
            {
                m_modifiedData += m_fileData[i];
                ++j;
                ++i;				
            }
        }
        // если закрывающая скобка - записываем
        if (m_fileData[i] == '>')
        {
            m_modifiedData += m_fileData[i];
            ++j;
            tagPosition.second = i;
            modTagPosition.second = j;
            m_realTagPosition.push_back(tagPosition);
            m_modifiedTagPosition.push_back(modTagPosition);
            tagPosition.first = -1;
            tagPosition.second = -1;
        }
    }

    removeTags(remTag);
    removeTags(remDoubleTag);

    // составляем clearedData
    for(i = 0; i < m_modifiedTagPosition.size(); ++i)
    {
        if (m_modifiedTagPosition[i].first != -1)
        {
            string tag = getTag(m_modifiedTagPosition[i], m_modifiedData);
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
            m_clearedData += ch;
            m_clearedTagPosition.push_back(m_modifiedTagPosition[i]);
        }
    }

    // Задаем размер таблицы
    m_tableSize = m_clearedData.size();
    // Выделяем память на таблицу
    m_pTable = new short *[m_tableSize];
    for (i = 0; i < m_tableSize; ++i)
        m_pTable[i] = new short[m_tableSize];

    // Заполняем таблицу
    for (i = 0; i < m_tableSize; ++i)
    {
        for (j = 0; j < m_tableSize; ++j)
        {
            if (m_clearedData[i] == m_clearedData[j])
            {
                m_pTable[i][j] = 1;
            }
            else
                m_pTable[i][j] = 0;
        }
    }
}

void CNewsFinder::getPossibleRanges()
{
    unsigned int i = 0;
    // Читаем в table по диагоналям
    // diag - Номер диагонали
    // diag = 0 - главная диагональ, заполнена 
    int diag = 1;
    while(diag != m_tableSize)
    {
        for (i = 0; i < m_tableSize - diag; ++i)
        {
            if (m_pTable[i][i + diag] != 0)
            {
                // Считываем строчку
                string str;
                int beg = i;
                while (i < m_tableSize - diag && m_pTable[i][i + diag] != 0)
                {
                    str += m_clearedData[i];
                    ++i;
                }
                // И проверяем возможность этой строчки(или её подстрок) быть началом или концом новости
                if (str.size() >= MINSZ)
                {
                    getTagSubs(str, beg + diag);
                    diag += str.size() - 1;
                }
            }
        }
        // Читаем следующую диагональ
        ++diag;
    }
    // Вычисляем средние длины/частоты строк
    m_avgLen = m_avgLen / m_freq.size();
    m_avgFreq = m_avgFreq / m_freq.size();
}

void CNewsFinder::getNewsRange()
{
    set <pair <string, int>, CNewsFinder::ltstr>::iterator setIter;
    vector<pair<string, int>>::iterator vectorIter;
    
    for(setIter = m_freq.begin(); setIter != m_freq.end(); ++setIter)
    {
        if (setIter->second >= m_minFreq && 
            setIter->first.size() <= m_avgLen * 1.3 + 1 &&
            setIter->second <= m_avgFreq)
            possibleTags.push_back(*setIter);
    }
    // сортируем пары
    sort(possibleTags.begin(), possibleTags.end(), CNewsFinder::pred());


    int cnt = 1;
    while (cnt < possibleTags.size())
    {
        string temp = (possibleTags.end() - cnt)->first;
        vectorIter = possibleTags.end() - cnt - 1;
        for (int i = 0; i < possibleTags.size(); ++i)
        {
            while(strstr(temp.c_str(), vectorIter->first.c_str()) || strstr(vectorIter->first.c_str(), temp.c_str()))
            {
                possibleTags.erase(vectorIter);
                if (cnt == possibleTags.size())
                    break;            
                vectorIter = possibleTags.end() - cnt - 1;
            }
            ++cnt;
            if (cnt >= possibleTags.size())
                break;
            vectorIter = possibleTags.end() - cnt - 1;
        }
        ++cnt;
    }

    int newAvgFreq = 0;
    for (int i = 0; i < possibleTags.size(); ++i)
    {
        newAvgFreq += possibleTags[i].second;
    }
    newAvgFreq = newAvgFreq / possibleTags.size() + 1;

    for (vectorIter = possibleTags.end() - 1; 
         /*vectorIter->second > newAvgFreq && */vectorIter != possibleTags.begin();
         --vectorIter)
    {
        string possibleBegin = vectorIter->first;
        for (vector<pair<string, int>>::iterator it = vectorIter - 1;; --it
             /*it->second > newAvgFreq && it != possibleTags.begin();
             --it*/)
        {
            string possebleEnd = it->first;
            int beg = strstr(m_clearedData.c_str(), possibleBegin.c_str()) - m_clearedData.c_str();
            int end = strstr(m_clearedData.c_str(), possebleEnd.c_str()) - m_clearedData.c_str();
            if (beg - end < m_clearedData.size() / MINSZ || end - beg < m_clearedData.size() / MINSZ)
            {
                int nextBegin = strstr(m_clearedData.c_str(), possibleBegin.c_str()) - m_clearedData.c_str();
                if (beg > end)
                {
                    newsBegin = possebleEnd;
                    newsEnd = possibleBegin;
                    unsigned int a = 0;
                    string news = getNews((char *)m_clearedData.c_str(), newsBegin, newsEnd, a);
                    if (news.size() > 0.01 * m_fileData.size() && news.size() < 0.2 * m_fileData.size())
                        return;
                    
                }
                else
                {
                    newsBegin = possibleBegin;
                    newsEnd = possebleEnd;
                    unsigned int a = 0;
                    string news = getNews((char *)m_clearedData.c_str(), newsBegin, newsEnd, a);
                    if (news.size() > 0.01 * m_fileData.size() && news.size() < 0.2 * m_fileData.size())
                        return;

                }
            }
            if (it == possibleTags.begin())
                break; 
        }
    }
}

void CNewsFinder::writeNews()
{
    char *strBegin = (char *)m_clearedData.c_str();
    unsigned int offset = 0;
    // Получаем строчку новости и выводим её
    while (strlen(strBegin) > offset)
    {
        string res = getNews(strBegin, newsBegin, newsEnd, offset);
        if (res.empty())
            break;
        m_fileOut << res;
        m_fileOut << "\n#########################################################################################\n";
    }
}