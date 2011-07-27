/**
 * \file    tagprocess.cpp
 *
 * \brief   Определение функций, оперирующих со строками.
 */

#include "tagprocess.h"
#include <iostream>

using std::ios;
using std::make_pair;


CNewsFinder::CNewsFinder(char *filename, unsigned int minSize, unsigned int minFreq):
    m_minSz(minSize),
    m_fileIn(filename, ios::in),
    m_fileOut("out", ios::out),
    m_fileData((std::istreambuf_iterator<char>(m_fileIn)), std::istreambuf_iterator<char>()),
    m_avgLen(0),
    m_avgFreq(0),
    m_minFreq(minFreq),
    currFileDataPos(0)
{
}

unsigned short CNewsFinder::getTagCode(const string &tag)
{
    unsigned short code = 0;
    for (int i = 0; i < tag.size(); ++i)
    {
        code += tag[i];
    }
    
    return code;
}

void CNewsFinder::removeTags(vector<string> &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;
    if (tagsToRemove.size() == 0)
        return;
    

    // проходим по всем тегам
    for (i = 0; i < mod.size(); ++i)
    {
        // проходим по всему массиву тегов для удаления
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // если тег совпал с тегом для удаления, то стираем его
            if (getTagCode(tagsToRemove[j]) == mod[i].first.tag)
            {
                mod.erase(mod.begin() + i);
                break;
            }
        }
    }
}


void CNewsFinder::removeTags(vector< pair<string, string> > &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;

    // проходим по всем тегам
    for (i = 0; i < mod.size(); ++i)
    {
        // проходим по всему массиву тегов для удаления
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // если тег совпал с тегом для удаления
            if (getTagCode(tagsToRemove[j].first) == mod[i].first.tag)
            {
                // стираем тег
                mod.erase(mod.begin() + i);
                // пока не получим закрывающий тег
                while (getTagCode(tagsToRemove[j].second) != mod[i].first.tag)
                {
                    mod.erase(mod.begin() + i);
                }
                mod.erase(mod.begin() + i);
                --i;
                // идем к следующему тегу
                break;
            }
        }
    }
}

void CNewsFinder::printTable()
{
    for (int i = 0; i < m_tableSize; ++i)
    {
        for (int j = 0; j < m_tableSize; ++j)
        {
            std::cout << m_pTable[i][j] << " ";
        }
        std::cout << "\n";
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

int CNewsFinder::checksum(const vector<CPair<CTag, CPair<int, int>>> &src)
{
    unsigned int i = 0;
    int cnt = 0;

    for (i = 0; i < src.size(); ++i)
        // если встретили '/' - тег закрывающий. Увеличиваем счетчик.
        if (src[i].first.isClose == 1)
            ++cnt;
    // если закрывающих тегов больше, чем открывающих - возвратим 0
    if (cnt > src.size() / 2 + 1)
        return 0;

    return 1;
}

int CNewsFinder::checkWordTruePairs(const vector<CPair<CTag, CPair<int, int>>> &src)
{
    unsigned int i;
    stack<CPair<CTag, CPair<int, int>>> st;

    for(i = 0; i < src.size(); ++i)
    {
        // если он - открывающий - положим в стек
        if (src[i].first.isClose == 0)
            st.push(src[i]);
        else
        {
            // если стек пуст - закрывающему тегу нет открывающего.
            if (st.empty())
                return 0;
            // пока не получим открывающий для текущего тега, извлекаем
            // теги из стека и проверяем их
            while (st.top().first.tag + '/' != src[i].first.tag)
            {
                st.pop();
                if (st.empty())
                    return 0;
            }
            st.pop();
        }
    }
    return 1;
}

int CNewsFinder::getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &src,
                               const vector<CPair<CTag, CPair<int, int>>> &str,
                               unsigned int pos)
{
    unsigned int freq = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    for (i = 0; i < m_tableSize; ++i)
    {
        if (m_pTable[i][pos] != 0)
        {
            unsigned int temp = i;
            vector<CPair<CTag, CPair<int, int>>> data;
            while (j < str.size() && temp < m_tableSize && m_pTable[temp][pos + j] != 0)
            {
                data.push_back(src[temp]);
                ++temp;
                ++j; 
            }
            if (!vStrCmp(str, data))
                ++freq;
            j = 0;
        }
    }
    return freq;
}


int CNewsFinder::getTagSubs(const vector<CPair<CTag, CPair<int, int>>> &src, int pos)
{
    // выбираем две подстроки длины на 1 меньше
    vector<CPair<CTag, CPair<int, int>>> first(src.begin(), src.end() - 1);
    vector<CPair<CTag, CPair<int, int>>> second(src.begin() + 1 ,src.end());
    set <vector<CPair<CTag, CPair<int, int>>>, tagcodecpr>::iterator setIter;
    int flag = 0;

    if (first.size() < m_minSz)
        return 1;

    if (first.size() == m_minSz)
    {
        setIter = m_subsArr.find(first);
        if (setIter == m_subsArr.end())
        {
            m_subsArr.insert(first);
            if (checksum(first) && checkWordTruePairs(first))
            {
                unsigned int strFreq = getStringFreq(mod, first, pos);
                if (strFreq >= m_minFreq)
                {
                    m_freq.insert(make_cpair(first, strFreq));
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
                unsigned int strFreq = getStringFreq(mod, second, pos + 1);
                if (strFreq >= m_minFreq)
                {
                    m_freq.insert(make_cpair(second, strFreq));
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
                unsigned int strFreq = getStringFreq(mod, src, pos);
                if (strFreq < m_minFreq)
                    return 0;
                m_freq.insert(make_cpair(src, strFreq));
                m_avgLen += src.size();
                m_avgFreq += strFreq;
            }
        }
        else
            return 0;
    }
    
    return 1;
}

string CNewsFinder::getNews(const vector<CPair<CTag, CPair<int, int>>> &srcBegin,
                            const vector<CPair<CTag, CPair<int, int>>> &newsBegin,
                            const vector<CPair<CTag, CPair<int, int>>> &newsEnd,
                            unsigned int &offset)
{
    unsigned int i = 0;
    // Ищем позицию, с которой начинается новость
    int begin = newsBegin[0].second.first;
    // Позиция, на которой новость заканчивается
    int end = newsEnd[newsEnd.size() - 1].second.second;
    offset = end + 1;
    // номер тега, с которого новость начинается и заканчивается, в modifiedTagPosition
    string ret(m_fileData, begin, end);
    return ret;
}

CTriple<CTag, CPair<int, int>, string> CNewsFinder::getNextTag()
{
    CTag tagCode(0,0);
    CTriple<CTag, CPair<int, int>, string> reti;
    string tag;
    CPair<int, int> tagPosition(-1,-1);

    while (currFileDataPos != m_fileData.size())
    {
        // если (возможно) открывающий тег
        if (m_fileData[currFileDataPos] == '<')
        {
            // к примеру, знак < в js
            if (tagPosition.first != -1)
            {
                tagPosition.first = currFileDataPos;
                ++currFileDataPos;
            }
            else
                tagPosition.first = currFileDataPos;

            // пока не дойдем до пробела/закрывающей скобки - записываем
            while (m_fileData[currFileDataPos] != ' ' && m_fileData[currFileDataPos] != '>')
            {
                tag += m_fileData[currFileDataPos];
                tagCode.tag += m_fileData[currFileDataPos];
                ++currFileDataPos;				
            }
        }
        // если закрывающая скобка - записываем
        if (m_fileData[currFileDataPos] == '>')
        {
            tag += m_fileData[currFileDataPos];
            //LowerCase(&tag);
            tagCode.tag += m_fileData[currFileDataPos];
            tagPosition.second = currFileDataPos;

            if(tag[1] == '/')
                tagCode.isClose = 1;
            ++currFileDataPos;
            reti.first = tagCode;
            reti.second = tagPosition;
            reti.third = tag;
            return reti;
        }
        ++currFileDataPos;
    }
    return CTriple<CTag, CPair<int, int>, string>(CTag(-1, -1), CPair<int, int>(-1, -1), "");
}

void CNewsFinder::init(vector<pair<string, string>> &remDoubleTag, vector<string> &remTag)
{
    unsigned int i = 0;
    unsigned int j = 0;
    int flag = 0;
    LowerCase(&m_fileData);
    CTriple<CTag, CPair<int, int>, string> tagCode;
    while (tagCode.third != "<body>")
        tagCode = getNextTag();

    tagCode = getNextTag();
    while (tagCode.first.tag != -1)
    {
        alphabet.insert(CPair<CTag, string>(tagCode.first, tagCode.third));
        mod.push_back(CPair<CTag, CPair<int, int>>(tagCode.first, tagCode.second));
        tagCode = getNextTag();
    }
    

    removeTags(remTag);
    removeTags(remDoubleTag);

    // Задаем размер таблицы
    m_tableSize = mod.size();
    // Выделяем память на таблицу
    m_pTable = new short *[m_tableSize];
    for (i = 0; i < m_tableSize; ++i)
        m_pTable[i] = new short[m_tableSize];

    // Заполняем таблицу
    for (i = 0; i < m_tableSize; ++i)
    {
        for (j = 0; j < m_tableSize; ++j)
        {
            if (mod[i].first.tag == mod[j].first.tag)
            {
                m_pTable[i][j] = 1;
            }
            else
                m_pTable[i][j] = 0;
        }
    }
    //printTable();
}

void CNewsFinder::getPossibleRanges()
{
    unsigned int i = 0;
    // Читаем в table по диагоналям
    // diag - Номер диагонали
    // diag = 0 - главная диагональ, заполнена 
    int diag = 1;
    while(diag < m_tableSize)
    {
        for (i = 0; i < m_tableSize - diag; ++i)
        {
            if (m_pTable[i][i + diag] != 0)
            {
                vector<CPair<CTag, CPair<int, int>>> range;
                int beg = i;
                while (i < m_tableSize - diag && m_pTable[i][i + diag] != 0)
                {
                    range.push_back(mod[i]);
                    ++i;
                }
                // И проверяем возможность этой строчки(или её подстрок) быть началом или концом новости
                if (range.size() >= m_minSz)
                {
                    getTagSubs(range, beg + diag);
                    i += range.size() - 1;
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
    set <CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>, ltstr>::iterator setIter;
    vector<CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>>::iterator vectorIter;
    
    for(setIter = m_freq.begin(); setIter != m_freq.end(); ++setIter)
    {
        if (setIter->second >= m_minFreq && 
            setIter->first.size() <= m_avgLen&&
            setIter->second <= m_avgFreq)
            possibleTags.push_back(*setIter);
    }
    // сортируем пары
    sort(possibleTags.begin(), possibleTags.end(), CNewsFinder::pred());


    unsigned int cnt = 1;
    while (cnt < possibleTags.size())
    {
        vector<CPair<CTag, CPair<int, int>>> temp = (possibleTags.end() - cnt)->first;
        vectorIter = possibleTags.end() - cnt - 1;
        for (unsigned int i = 0; i < possibleTags.size(); ++i)
        {
            while(vStrStr(temp, vectorIter->first) || vStrStr(vectorIter->first, temp))
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
    for (unsigned int i = 0; i < possibleTags.size(); ++i)
    {
        newAvgFreq += possibleTags[i].second;
    }
    newAvgFreq = newAvgFreq / possibleTags.size() + 1;

    for (vectorIter = possibleTags.end() - 1; 
         /*vectorIter->second > newAvgFreq && */vectorIter != possibleTags.begin();
         --vectorIter)
    {
        vector<CPair<CTag, CPair<int, int>>> possibleBegin = vectorIter->first;
        for (vector<CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>>::iterator it = vectorIter - 1;; --it
             /*it->second > newAvgFreq && it != possibleTags.begin();
             --it*/)
        {
            vector<CPair<CTag, CPair<int, int>>> possibleEnd = it->first;
            unsigned int beg = possibleBegin[0].second.first;
            unsigned int end = possibleEnd[possibleEnd.size() - 1].second.first;
            if (beg - end < m_fileData.size() / m_minSz || end - beg < m_fileData.size() / m_minSz)
            {
                if (beg > end)
                {
                    m_newsBegin = possibleEnd;
                    m_newsEnd = possibleBegin;
                    unsigned int a = 0;
                    string news = getNews(mod, m_newsBegin, m_newsEnd, a);
                    if (news.size() > 0.01 * m_fileData.size() && news.size() < 0.2 * m_fileData.size())
                    {
                        m_fileOut << news;
                        return;
                    }
                }
                else
                {
                    m_newsBegin = possibleBegin;
                    m_newsEnd = possibleEnd;
                    unsigned int a = 0;
                    string news = getNews(mod, m_newsBegin, m_newsEnd, a);
                    if (news.size() > 0.01 * m_fileData.size() && news.size() < 0.2 * m_fileData.size())
                    {
                        m_fileOut << news;
                        return;
                    }

                }
            }
            if (it == possibleTags.begin())
                break; 
        }
    }
}

void CNewsFinder::writeNews()
{
    //char *strBegin = (char *)m_clearedData.c_str();
   // unsigned int offset = 0;
    // Получаем строчку новости и выводим её
    //while (strlen(strBegin) > offset)
   // {
        //string res = getNews(strBegin, m_newsBegin, m_newsEnd, offset);
        //if (res.empty())
          //  break;
        //m_fileOut << res;
     //   m_fileOut << "\n#########################################################################################\n";
   // }
}