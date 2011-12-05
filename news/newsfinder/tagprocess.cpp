/**
 * \file    tagprocess.cpp
 *
 * \brief   Определение функций, оперирующих со строками.
 */

#include <iostream>
#include <algorithm>
#include <stack>

#include "../suffixtrie/suffixtrie.h"
#include "tagprocess.h"


using std::ios;
using std::make_pair;
using std::string;
using std::pair;
using std::set;
using std::stack;

struct pred
{
    bool operator () (const vector<pair<int, int>> &left, const vector<pair<int, int>> &right)
    {
        return left.size() > right.size();
    }
};

//##################################  PUBLIC SECTION  ###################################

CNewsFinder:: CNewsFinder(__in LPCSTR fileName, __in int minSize, __in int minFreq, __in int numberOfNews):
    m_minLen(minSize),
    m_minFreq(minFreq),
    m_avgLen(0),
    m_avgFreq(0),
    m_unCurrFileDataPos(0),
    m_lLastError(0),
    m_numberOfNews(numberOfNews),
    m_lVisibleHtmlLen(0)
{
    std::fstream fileIn(fileName, ios::in);
    m_fileData = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    fileIn.close();
}

CNewsFinder::~CNewsFinder()
{
}

void CNewsFinder::Init(vector<pair<string, string>> &remDoubleTag, vector<string> &remTag)
{
    if (m_lLastError == 1)
        return;

    m_lLastError = 1;

    //LowerCase(&m_fileData);
    // M.A.P. плохая реализация преобразования в нижний регистр.  
    // Я знаю((
    // 
    transform(m_fileData.begin(), m_fileData.end(), m_fileData.begin(), tolower);

    CTagDescription tag;
    tag = getNextTag();
    // 103 == <body>
    while (tag.nTagCode != 103)
    {
        tag = getNextTag();
    }

    tag = getNextTag();
    while (tag.nTagCode != -1)
    {
        m_mod.push_back(tag);
        tag = getNextTag();
    }
    
    removeTags(remTag);
    removeTags(remDoubleTag);
}

int compare(const CTagDescription &left, const CTagDescription &right)
{
    if (left.nTagCode == right.nTagCode)
        return 0;
    if (left.nTagCode > right.nTagCode)
        return 1;
    else
        return -1;
}

vector<CTagSequence> CNewsFinder::GetRanges()
{
    return tags;
}

void CNewsFinder::GetPossibleRanges()
{
    // строим суффиксное дерево на основе вектора тегов
    CTrie<vector<CTagDescription>, CTagDescription> tree(m_mod, m_mod.size(), compare);
    // получаем все координаты начала-конца построк более заданной длины и встречающиеся более чем заданное число раз
    vector< vector<pair<int, int>>> substrings = tree.GetRanges(m_minLen, m_minFreq);

    for (vector< vector<pair<int, int>>>::iterator it = substrings.begin(); it != substrings.end(); ++it)
    {
        // создаем структуру расположения послудовательности тегов в исходной строке
        CTagSequence currTagSeq;
        vector<CTagDescription> word;
        int allSubsLen = 0;

        // формируем саму подстроку структур тегов
        for (int i = it->front().first; i <= it->front().second; ++i)
        {
            word.push_back(m_mod[i]);
        }

        currTagSeq.tag = word;

        for (vector<pair<int, int>>::iterator j = it->begin(); j != it->end(); ++j)
        {
            CTagRange newRange;
            newRange.begin = m_mod[j->first].nTagBegin;
            newRange.end = m_mod[j->second].nTagEnd;
            newRange.tagString = string(m_fileData, newRange.begin, newRange.end - newRange.begin + 1);
            newRange.percToHtml = (double)(newRange.end - newRange.begin) / m_fileData.size() * 100;
            newRange.percToVisibleHtml = 0;

            allSubsLen += newRange.end - newRange.begin;
            currTagSeq.tagRange.push_back(newRange);
        }

        currTagSeq.percToHtml = (double) allSubsLen / m_fileData.size() * 100;
        tags.push_back(currTagSeq);
    }

    vector<vector<CTagRange>> tagRanges;

    for (vector<CTagSequence>::iterator it = tags.begin(); it != tags.end(); ++it)
    {
        tagRanges.push_back(it->tagRange);
    }
    std::sort(tagRanges.begin(), tagRanges.end(), pred1());
}

void CNewsFinder::GetNewsRange()
{
    set <pair<vector<CTagDescription>, unsigned int>, ltstr>::iterator setIter;
    vector<pair<vector<CTagDescription>, unsigned int>>::iterator vectorIter;
    
    for(setIter = m_freq.begin(); setIter != m_freq.end(); ++setIter)
    {
        if (setIter->second >= m_minFreq && 
            setIter->first.size() <= m_avgLen &&
            setIter->second <= m_avgFreq)
            possibleTags.push_back(*setIter);
    }
    // сортируем пары
    sort(possibleTags.begin(), possibleTags.end(), CNewsFinder::pred());


    // стираем элементы, являющиеся подстрокамы более длинных элементов
    unsigned int cnt = 1;
    while (cnt < possibleTags.size())
    {
        vector<CTagDescription> temp = (possibleTags.end() - cnt)->first;
        vectorIter = possibleTags.end() - cnt - 1;
        for (unsigned int i = 0; i < possibleTags.size(); ++i)
        {
            while(vIsSubstr(temp, vectorIter->first) || vIsSubstr(vectorIter->first, temp))
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

    for (vectorIter = possibleTags.end() - 1; vectorIter != possibleTags.begin(); --vectorIter)
    {
        vector<CTagDescription> possibleBegin = vectorIter->first;
        for (vector<pair<vector<CTagDescription>, unsigned int>>::iterator it = vectorIter - 1;; --it)
        {
            vector<CTagDescription> possibleEnd = it->first;
            vector<CTagDescription>::iterator ite = pStrStr(m_mod, possibleBegin);
            unsigned int beg = ite->nTagBegin;
            ite = pStrStr(m_mod, possibleEnd);
            unsigned int end = ite->nTagEnd;
            if (beg - end < m_fileData.size() / m_minLen || end - beg < m_fileData.size() / m_minLen)
            {
                if (beg > end)
                {
                    m_newsBegin = possibleEnd;
                    m_newsEnd = possibleBegin;
                    string news(m_fileData, end, beg - end);
                    if (news.size() > 0.01 * m_fileData.size() && news.size() < 0.2 * m_fileData.size())
                    {
                        return;
                    }
                }
                else
                {
                    m_newsBegin = possibleBegin;
                    m_newsEnd = possibleEnd;
                    string news(m_fileData, beg, end - beg);
                    if (news.size() > 0.01 * m_fileData.size() && news.size() < 0.2 * m_fileData.size())
                    {
                        return;
                    }

                }
            }
            if (it == possibleTags.begin())
                break; 
        }
    }
}

void CNewsFinder::WriteNews(LPCSTR lpcszOutFileName)
{
    ///< Выходной файл.
    std::fstream fileOut(lpcszOutFileName, ios::out);

    fileOut << "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n"
    "<body>\n";

    unsigned int offset = 0;
    // Получаем строчку новости и выводим её
    for(;;)
    {
        string res = getNews(m_newsBegin, m_newsEnd, offset);
        if (res.empty())
            break;
        fileOut << res;
        fileOut << "\n#########################################################################################\n";
    }
    fileOut << "\n</body></html>";
}

//#################################  PRIVATE SECTION  #####################################

void CNewsFinder::removeTags(vector<std::string> &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;
    if (tagsToRemove.size() == 0)
        return;

    // M.A.P. очень грубо реализован механизм вычистки тегов
    // 1. поиск перебором 2. многократный персчет кода

    // проходим по всем тегам
    for (i = 0; i < m_mod.size(); ++i)
    {
        // проходим по всему массиву тегов для удаления
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // если тег совпал с тегом для удаления, то стираем его
            if (tagsToRemove[j] == m_mod[i].tag)
            {
                m_mod.erase(m_mod.begin() + i);
                break;
            }
        }
    }
}

void CNewsFinder::removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove)
{
    unsigned int i = 0;
    unsigned int j = 0;

    // M.A.P. очень грубо реализован механизм вычистки тегов 
    // 1. поиск перебором 2. многократный персчет кода

    // проходим по всем тегам
    for (i = 0; i < m_mod.size(); ++i)
    {
        // проходим по всему массиву тегов для удаления
        for (j = 0; j < tagsToRemove.size(); ++j)
        {
            // если тег совпал с тегом для удаления
            if (tagsToRemove[j].first == m_mod[i].tag)
            {
                // стираем тег
                m_mod.erase(m_mod.begin() + i);
                // пока не получим закрывающий тег
                while (tagsToRemove[j].second != m_mod[i].tag)
                {
                    m_mod.erase(m_mod.begin() + i);
                }
                m_mod.erase(m_mod.begin() + i);
                --i;
                // идем к следующему тегу
                break;
            }
        }
    }
}

unsigned short CNewsFinder::getTagCode(const string &tag)
{
    unsigned short code = 0;
    for (unsigned int i = 0; i < tag.size(); ++i)
    {
        code += tag[i];
    }

    code = code / tag[1] + tag[1];
    return code;
}

CTagDescription CNewsFinder::getNextTag()
{
    CTagDescription tagCode;
    tagCode.bIsClose = 0;
    tagCode.nTagCode = -1;
    tagCode.nTagBegin = -1;
    tagCode.nTagEnd = -1;
    string tag;

    for(; m_unCurrFileDataPos != m_fileData.size(); ++m_unCurrFileDataPos)
    {
        // если (возможно) открывающий тег
        if (m_fileData[m_unCurrFileDataPos] == '<')
        {
            // M.A.P. как данная проверка согласуется со стандартом html?
            // к примеру, знак < в js
            if (tagCode.nTagBegin != -1)
                ++m_unCurrFileDataPos;
            else
                tagCode.nTagBegin = m_unCurrFileDataPos;

            // пока не дойдем до пробела/закрывающей скобки - записываем
            while (m_fileData[m_unCurrFileDataPos] != ' ' && m_fileData[m_unCurrFileDataPos] != '>')
            {
                tag += m_fileData[m_unCurrFileDataPos];
                tagCode.nTagCode += m_fileData[m_unCurrFileDataPos];
                ++m_unCurrFileDataPos;				
            }
        }
        // если закрывающая скобка - записываем
        if (m_fileData[m_unCurrFileDataPos] == '>' && tagCode.nTagBegin != -1)
        {
            tag += m_fileData[m_unCurrFileDataPos];
            tagCode.nTagCode += m_fileData[m_unCurrFileDataPos];
            tagCode.nTagEnd = m_unCurrFileDataPos;
            tagCode.nTagCode = tagCode.nTagCode / tag[1] + tag[1];
            if(tag[1] == '/')
                tagCode.bIsClose = 1;
            else
                tagCode.bIsClose = 0;

            tagCode.tag = tag;
            return tagCode;
        }
    }
    return tagCode;
}

int CNewsFinder::checkTag(const vector<CTagDescription> &src)
{
    int closedTagCounter = 0;
    stack<CTagDescription> st;

    for (int i = 0; i < src.size(); ++i)
    {
        // если тег закрывающий.
        if (src[i].bIsClose == 0)
        {
            // кладем его в стек
            st.push(src[i]);
        }
        else
        {
            // увеличиваем счетчик закрывающих тегов
            ++closedTagCounter;

            // если стек пуст - закрывающему тегу нет открывающего.
            if (st.empty())
                return 0;
            // пока не получим открывающий для текущего тега, извлекаем
            // теги из стека и проверяем их
            while (st.top().nTagCode + '/' != src[i].nTagCode)
            {
                st.pop();
                if (st.empty())
                    return 0;
            }
            st.pop();
        }
    }

    // если закрывающих тегов больше, чем открывающих - возвратим 0
    return (closedTagCounter < src.size() / 2 + 1);
}

string CNewsFinder::getNews(vector<CTagDescription> &newsBegin,
                            vector<CTagDescription> &newsEnd,
                            unsigned int &unOffset)
{
    unsigned int i = 0;
    // Ищем позицию, с которой начинается новость
    vector<CTagDescription>::iterator it = pStrStr(m_mod, newsBegin, unOffset);
    if (it == m_mod.end())
    {
        return string("");
    }
    int begin = it->nTagBegin;
    unOffset = it - m_mod.begin() + 1;
    // Позиция, на которой новость заканчивается
    it = pStrStr(m_mod, newsEnd, unOffset);
    if (it == m_mod.end())
    {
        return string("");
    }
    int end = it->nTagEnd;
    // номер тега, с которого новость начинается и заканчивается, в modifiedTagPosition
    string ret(m_fileData, begin, end - begin);
    return ret;
}

long CNewsFinder::GetlastError()
{
    return m_lLastError;
}

//#################################  DEBUG SECTION  #####################################
