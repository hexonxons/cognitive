/**
 * \file    tagprocess.cpp
 *
 * \brief   Определение функций, оперирующих со строками.
 */

#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <functional>
#include <cmath>

using std::ios;
using std::make_pair;
using std::string;
using std::pair;
using std::set;
using std::stack;
using std::list;

struct pred
{
    bool operator () (const vector<pair<int, int>> &left, const vector<pair<int, int>> &right)
    {
        return left.size() > right.size();
    }
};

// первое число пары - флаг
// второе число - длина пересечения
inline pair<int, int> intersect(const CNews &left, const CNews &right)
{
    if (left.NewsEnd < right.NewsBegin || right.NewsEnd < left.NewsBegin)
        return make_pair(-1, 0);
    // если left включается полностью в right
    if (right.NewsEnd >= left.NewsEnd && right.NewsBegin <= left.NewsBegin)
        return make_pair(1, left.NewsEnd - left.NewsBegin);
    // если наоборот
    if (left.NewsEnd >= right.NewsEnd && left.NewsBegin <= right.NewsBegin)
        return make_pair(0, right.NewsEnd - right.NewsBegin);
    // если конец left пересекается с началом right
    if(left.NewsEnd >= right.NewsBegin)
        return make_pair(-1, left.NewsEnd - right.NewsBegin + 1);
    // если наоборот
    if(right.NewsEnd >= left.NewsBegin)
        return make_pair(-1, right.NewsEnd - left.NewsBegin + 1);
    
    return make_pair(-2, 0);
}

//##################################  PUBLIC SECTION  ###################################

CNewsFinder:: CNewsFinder(__in LPCSTR fileName, __in int minSize, __in int minFreq, __in int numberOfNews):
    m_minLen(minSize),
    m_minFreq(minFreq),
    m_avgLen(0),
    m_avgFreq(0),
    m_unCurrFileDataPos(0),
    m_lLastError(0),
    m_numberOfNews(numberOfNews),
    m_lVisibleHtmlLen(0),
    AvgDispersion(0.0)
{
    std::fstream fileIn(fileName, ios::in);
    m_fileData = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    fileIn.close();
}

CNewsFinder:: CNewsFinder(__in LPCSTR fileData, __in int minSize, __in int minFreq):
    m_minLen(minSize),
    m_minFreq(minFreq),
    m_avgLen(0),
    m_avgFreq(0),
    m_unCurrFileDataPos(0),
    m_lLastError(0),
    m_numberOfNews(0),
    m_lVisibleHtmlLen(0),
    AvgDispersion(0.0)
{
    m_fileData = fileData;
}

CNewsFinder::~CNewsFinder()
{
}

void CNewsFinder::Init()
{
    if (m_lLastError == 1)
        return;

    m_lLastError = 1;

    transform(m_fileData.begin(), m_fileData.end(), m_fileData.begin(), tolower);
    CTag tag = getNextTag();

    // 103 == <body>
    while (tag.tag.compare("<body>") != 0)
    {
        tag = getNextTag();
    }

    // 59 == </body>
    while (tag.tag.compare("</body>") != 0)
    {
        if(tag.tag.compare("<script>") == 0)
            while (tag.tag.compare("</script>") != 0)
                tag = getNextTag();
        else
        {
            m_mod.push_back(tag);
            tag = getNextTag();
        }
    }
    m_mod.push_back(tag);
}

int compare(const CTag &left, const CTag &right)
{
    if (left.TagHashCode == right.TagHashCode)
        return 0;
    if (left.TagHashCode > right.TagHashCode)
        return 1;
    else
        return -1;
}

// Пишем-пишем через жопу
// Пишем-пишем-траляля...


void CNewsFinder::calcHeur()
{

    struct DISPRANGESSORT 
    {
        bool operator() (CNewsBlock &left, CNewsBlock &right)
        {
            if(left.BlockHeuristics.Dispersion == right.BlockHeuristics.Dispersion)
                return left.NewsRange.size() > right.NewsRange.size();
            else
                return left.BlockHeuristics.Dispersion < right.BlockHeuristics.Dispersion;
        }
    };

    struct INTERSECTRANGESSORT 
    {
        bool operator() (CNewsBlock &left, CNewsBlock &right)
        {
            return left.BlockHeuristics.IntersectionWithOtherBlocks < right.BlockHeuristics.IntersectionWithOtherBlocks;
        }
    };

    struct PERCTOVISHTMLTRANGESSORT 
    {
        bool operator() (CNewsBlock &left, CNewsBlock &right)
        {
            return left.BlockHeuristics.PercentageToAllVisibleHtmlLen > right.BlockHeuristics.PercentageToAllVisibleHtmlLen;
        }
    };

    struct PERCTOHTMLTRANGESSORT 
    {
        bool operator() (CNewsBlock &left, CNewsBlock &right)
        {
            return left.BlockHeuristics.PercentageToAllHtmlLen > right.BlockHeuristics.PercentageToAllHtmlLen;
        }
    };

    sort(NewsBlocks.begin(), NewsBlocks.end(), DISPRANGESSORT());
    for (int i = 0; i < NewsBlocks.size(); ++i)
        NewsBlocks[i].BlockHeuristics.Weight += i;

    sort(NewsBlocks.begin(), NewsBlocks.end(), INTERSECTRANGESSORT());
    for (int i = 0; i < NewsBlocks.size(); ++i)
        NewsBlocks[i].BlockHeuristics.Weight += i;

    sort(NewsBlocks.begin(), NewsBlocks.end(), PERCTOVISHTMLTRANGESSORT());
    for (int i = 0; i < NewsBlocks.size(); ++i)
        NewsBlocks[i].BlockHeuristics.Weight += i;

    sort(NewsBlocks.begin(), NewsBlocks.end(), PERCTOHTMLTRANGESSORT());
    for (int i = 0; i < NewsBlocks.size(); ++i)
        NewsBlocks[i].BlockHeuristics.Weight += i;
}


vector<CNewsBlock> CNewsFinder::GetRanges()
{
    calcHeur();
    struct WEIGHTRANGESSORT 
    {
        bool operator() (CNewsBlock &left, CNewsBlock &right)
        {
            return left.BlockHeuristics.Weight < right.BlockHeuristics.Weight;
        }
    };
    sort(NewsBlocks.begin(), NewsBlocks.end(), WEIGHTRANGESSORT());
    return NewsBlocks;
}

int getVisibleCharNum(int begin, int end, vector<CTag> &m_mod)
{
    int len = 0;
    for (int i = begin; i < end; ++i)
    {
        // ну не считаем код скриптов как видимый, ет..
        if (m_mod[i + 1].tag.compare("</script>") == 0)
            continue;
        len += m_mod[i + 1].TagBegin - m_mod[i].TagEnd - 1;
    }
    return len;
}

vector<pair<int, int>> ispairnews(vector<pair<int, int>> &left, vector<pair<int, int>> &right)
{
    vector<pair<int, int>> newRow;
    vector<pair<int, int>>::iterator it = left.begin();
    vector<pair<int, int>>::iterator jt = right.begin();

    bool itbeforejt = false;
    bool jtbeforeit = false;

    while(it != left.end() && jt != right.end())
    {
        if (it->first > jt->second)
        {
            jtbeforeit = true;
            if (itbeforejt || newRow.size() != 0 && newRow[newRow.size() - 1].second >= jt->first)
                return vector<pair<int, int>>();

            newRow.push_back(make_pair(jt->first, it->second));
        }
        else
            if (jt->first > it->second)
            {
                itbeforejt = true;
                if (jtbeforeit || newRow.size() != 0 && newRow[newRow.size() - 1].second >= it->first)
                {
                    return vector<pair<int, int>>();
                }
                newRow.push_back(make_pair(it->first, jt->second));
            }
            else
                 return vector<pair<int, int>>();
        ++it;
        ++jt;
    }
    return newRow;
}

void CNewsFinder::GetPossibleRanges()
{
    // строим суффиксное дерево на основе вектора тегов
    CTrie<vector<CTag>, CTag> tree(m_mod, m_mod.size(), compare);
    // получаем все координаты начала-конца построк более заданной длины и встречающиеся более чем заданное число раз
    multimap<int, vector< pair<int, int>>> treeSubstrings = tree.GetRanges(m_minLen, m_minFreq);

    for (multimap<int, vector< pair<int, int>>>::iterator it = treeSubstrings.begin(); it != treeSubstrings.end(); ++it)
    {
        // создаем структуру расположения последовательности тегов в исходной строке
        CNewsBlock newsBlock;
        NewsBlockHeuristics blockHeuristics;

        int allSubsLen = 0;

        // Формируем подстроку тегов
        newsBlock.tag.assign(m_mod.begin() + it->second.front().first, m_mod.begin() + it->second.front().second + 1);
        // Заполняем структуру новости
        for (vector<pair<int, int>>::iterator j = it->second.begin(); j != it->second.end(); ++j)
        {
            CNews news;
            NewsHeuristics newsHeuristics;

            news.NewsBegin = m_mod[j->first].TagBegin;
            news.NewsEnd = m_mod[j->second].TagEnd;
            news.TagString = string(m_fileData, news.NewsBegin, news.NewsEnd - news.NewsBegin + 1);

            newsHeuristics.PercentageToAllVisibleHtmlLen = (double)getVisibleCharNum(j->first, j->second, m_mod) / m_lVisibleHtmlLen * 100;
            newsHeuristics.PercentageToAllHtmlLen = (double)(news.NewsEnd - news.NewsBegin) / m_fileData.size() * 100;

            blockHeuristics.PercentageToAllVisibleHtmlLen += newsHeuristics.PercentageToAllVisibleHtmlLen;
            blockHeuristics.PercentageToAllHtmlLen += newsHeuristics.PercentageToAllHtmlLen;

            if (j + 1 != it->second.end())
            {
                newsHeuristics.DistanceToNextNews = m_mod[(j + 1)->first].TagBegin - m_mod[j->second].TagEnd - 1;
                blockHeuristics.NewsDistanceSumm += newsHeuristics.DistanceToNextNews;
            }

            news.Heuristics = newsHeuristics;
            newsBlock.NewsRange.push_back(news);
        }

        // Досчитаем эвристику блока новостей

        // Дисперсия расстояния между новостями
        for (vector<CNews>::iterator jt = newsBlock.NewsRange.begin(); jt != newsBlock.NewsRange.end() - 1; ++jt)
        {
            NewsHeuristics newsHeuristics = jt->Heuristics;
            blockHeuristics.Dispersion += (newsHeuristics.DistanceToNextNews - blockHeuristics.NewsDistanceSumm / (newsBlock.NewsRange.size() - 1)) * 
                                          (newsHeuristics.DistanceToNextNews - blockHeuristics.NewsDistanceSumm / (newsBlock.NewsRange.size() - 1));
            AvgDispersion += blockHeuristics.Dispersion;
        }

        newsBlock.BlockHeuristics = blockHeuristics;
        NewsBlocks.push_back(newsBlock);
    }

    AvgDispersion /= treeSubstrings.size();

    // Пишем-пишем через жопу
    // Пишем-пишем-траляля...
    // Считаем IntersectionWithOtherBlocks каждого блока
    for(vector<CNewsBlock>::iterator it = NewsBlocks.begin(); it != NewsBlocks.end(); ++it)
    {
        vector<CNews> CurrentNewsRange = it->NewsRange;
        // проходим по оставшимся блокам
        for(vector<CNewsBlock>::iterator jt = it + 1; jt != NewsBlocks.end(); ++jt)
        {
            // берем блок новостей
            vector<CNews> NewsRange = jt->NewsRange;
            // проходим по всем элементам блока текущего блока новостей
            for (vector<CNews>::iterator CurrentNewsRangeIterator = CurrentNewsRange.begin(); CurrentNewsRangeIterator != CurrentNewsRange.end(); ++CurrentNewsRangeIterator)
            {
                CNews CurrNews = *CurrentNewsRangeIterator;
                // для каждой новости нового блока
                for (vector<CNews>::iterator NewsRangeIterator = NewsRange.begin(); NewsRangeIterator != NewsRange.end(); ++NewsRangeIterator)
                {
                    CNews News = *NewsRangeIterator;
                    if(CurrNews.NewsEnd < News.NewsBegin)
                        break;
                    pair<int, int> intersection = intersect(CurrNews, News);
                    switch (intersection.first)
                    {
                        case -1:
                        {
                            it->BlockHeuristics.IntersectionWithOtherBlocks += intersection.second;
                            jt->BlockHeuristics.IntersectionWithOtherBlocks += intersection.second;
                    	    break;
                        }
                        case 0:
                        {
                            it->BlockHeuristics.IntersectionWithOtherBlocks += intersection.second;
                            break;
                        }
                        case 1:
                        {
                            jt->BlockHeuristics.IntersectionWithOtherBlocks += intersection.second;
                            break;
                        }
                        default:
                        {
                            assert(intersection.first == -2);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void CNewsFinder::GetNewsRange()
{
    set <pair<vector<CTag>, unsigned int>, ltstr>::iterator setIter;
    vector<pair<vector<CTag>, unsigned int>>::iterator vectorIter;
    
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
        vector<CTag> temp = (possibleTags.end() - cnt)->first;
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
        vector<CTag> possibleBegin = vectorIter->first;
        for (vector<pair<vector<CTag>, unsigned int>>::iterator it = vectorIter - 1;; --it)
        {
            vector<CTag> possibleEnd = it->first;
            vector<CTag>::iterator ite = pStrStr(m_mod, possibleBegin);
            unsigned int beg = ite->TagBegin;
            ite = pStrStr(m_mod, possibleEnd);
            unsigned int end = ite->TagEnd;
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

//#################################  PRIVATE SECTION  #####################################

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

CTag CNewsFinder::getNextTag()
{
    CTag tagCode;
    tagCode.IsCloseTag = 0;
    tagCode.TagHashCode = -1;
    tagCode.TagBegin = -1;
    tagCode.TagEnd = -1;
    string tag;
    bool isComment = false;

    for(; m_unCurrFileDataPos != m_fileData.size(); ++m_unCurrFileDataPos)
    {
        // если (возможно) открывающий тег
        if (m_fileData[m_unCurrFileDataPos] == '<')
        {
            // M.A.P. как данная проверка согласуется со стандартом html?
            // к примеру, знак < в js
            if (tagCode.TagBegin != -1)
                ++m_unCurrFileDataPos;
            else
                tagCode.TagBegin = m_unCurrFileDataPos;

            // пока не дойдем до пробела/закрывающей скобки - записываем
            while (m_fileData[m_unCurrFileDataPos] != ' ' && m_fileData[m_unCurrFileDataPos] != '>' && m_fileData[m_unCurrFileDataPos] != '\n')
            {
                tag += m_fileData[m_unCurrFileDataPos];
                tagCode.TagHashCode += m_fileData[m_unCurrFileDataPos];
                ++m_unCurrFileDataPos;				
            }
            if(tag.compare(0, 4, "<!--") == 0)
                isComment = true;
        }
        // если закрывающая скобка - записываем
        if (m_fileData[m_unCurrFileDataPos] == '>' && tagCode.TagBegin != -1)
        {
            if(isComment)
            {
                if (m_fileData[m_unCurrFileDataPos - 2] != '-' && m_fileData[m_unCurrFileDataPos] != '-')
                {
                    ++m_unCurrFileDataPos;
                    continue;
                }
            }

            if (m_fileData[m_unCurrFileDataPos - 1] == '/' && tag.compare("<a") != 0)
            {
                tag += m_fileData[m_unCurrFileDataPos - 1];
            }
            
            tag += m_fileData[m_unCurrFileDataPos];
            tagCode.TagHashCode += m_fileData[m_unCurrFileDataPos];
            tagCode.TagEnd = m_unCurrFileDataPos;
            tagCode.TagHashCode = tagCode.TagHashCode / tag[1] + tag[1];
            if(tag[1] == '/')
                tagCode.IsCloseTag = 1;
            else
                tagCode.IsCloseTag = 0;

            tagCode.tag = tag;

            while (tagCode.TagHashCode != 121 && m_fileData[m_unCurrFileDataPos] != '<' && m_unCurrFileDataPos != m_fileData.size())
            {
                m_unCurrFileDataPos++;
                if (m_fileData[m_unCurrFileDataPos] != '<')
                {
                    m_lVisibleHtmlLen++;
                }    
            }
            
            return tagCode;
        }
    }
    return tagCode;
}

string CNewsFinder::getNews(vector<CTag> &newsBegin,
                            vector<CTag> &newsEnd,
                            unsigned int &unOffset)
{
    unsigned int i = 0;
    // Ищем позицию, с которой начинается новость
    vector<CTag>::iterator it = pStrStr(m_mod, newsBegin, unOffset);
    if (it == m_mod.end())
    {
        return string("");
    }
    int begin = it->TagBegin;
    unOffset = it - m_mod.begin() + 1;
    // Позиция, на которой новость заканчивается
    it = pStrStr(m_mod, newsEnd, unOffset);
    if (it == m_mod.end())
    {
        return string("");
    }
    int end = it->TagEnd;
    // номер тега, с которого новость начинается и заканчивается, в modifiedTagPosition
    string ret(m_fileData, begin, end - begin);
    return ret;
}