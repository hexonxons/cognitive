/**
 * \file    tagprocess.cpp
 *
 * \brief   Определение функций, оперирующих со строками.
 */

#define _CRT_SECURE_NO_WARNINGS

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

//##################################  PUBLIC SECTION  ###################################

CNewsFinder::CNewsFinder(LPCSTR lpcszInFileName, unsigned int unMinSize, unsigned int unMinFreq):
    m_unMinSz(unMinSize),
    m_unAvgLen(0),
    m_unAvgFreq(0),
    m_unMinFreq(unMinFreq),
    m_unCurrFileDataPos(0),
    m_lLastError(0)
{
    std::fstream fileIn(lpcszInFileName, ios::in);
    m_fileData = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
#ifdef _DEBUG
    plpszTagTable = new LPTSTR[1000];
    memset(plpszTagTable, 0, 1000 * sizeof(LPTSTR));
#endif
}

CNewsFinder::~CNewsFinder()
{
    if (m_unTableSize == 0)
        return;

    for (unsigned int i = 0; i < m_unTableSize; ++i)
        delete m_pTable[i];
    delete m_pTable;

#ifdef _DEBUG
    delete(plpszTagTable);
#endif
}

void CNewsFinder::Init(vector<pair<string, string>> &remDoubleTag, vector<string> &remTag)
{
    if (m_lLastError == 1)
        return;

    m_lLastError = 1;

    unsigned int i = 0;
    unsigned int j = 0;
    int flag = 0;

    //LowerCase(&m_fileData);
    // M.A.P. плохая реализация преобразования в нижний регистр.  
    // 
    transform(m_fileData.begin(), m_fileData.end(), m_fileData.begin(), tolower);

    CTagDescription tag;
    tag = getNextTag();
    // 551 == <body>
    while (tag.nTagCode != 551)
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

#ifdef _DEBUG
    dbgPrintData();
#endif
}

void CNewsFinder::GetPossibleRangesUsingTable()
{
    unsigned int i = 0;
    unsigned int j = 0;
    // M.A.P. Не лучше ли выделять память одним куском?

    // Задаем размер таблицы
    m_unTableSize = m_mod.size();
    // Выделяем память на таблицу
    m_pTable = new bool *[m_unTableSize];

    if (m_pTable == NULL)
    {
        m_lLastError = -1;
        return;
    }

    for (i = 0; i < m_unTableSize; ++i)
    {
        m_pTable[i] = new bool[m_unTableSize];
        if (m_pTable[i] == NULL)
        {
            m_lLastError = -1;
            return;
        }
    }

    // Заполняем таблицу
    for (i = 0; i < m_unTableSize; ++i)
    {
        for (j = 0; j < m_unTableSize; ++j)
        {
            if (m_mod[i].nTagCode == m_mod[j].nTagCode)
            {
                m_pTable[i][j] = 1;
            }
            else
                m_pTable[i][j] = 0;
        }
    }

    // Читаем в table по диагоналям
    // diag - Номер диагонали
    // diag = 0 - главная диагональ, заполнена 
    unsigned int diag = 1;
    while(diag < m_unTableSize)
    {
        for (i = 0; i < m_unTableSize - diag; ++i)
        {
            if (m_pTable[i][i + diag] != 0)
            {
                vector<CTagDescription> range;
                int beg = i;
                while (i < m_unTableSize - diag && m_pTable[i][i + diag] != 0)
                {
                    range.push_back(m_mod[i]);
                    ++i;
                }
                // И проверяем возможность этой строчки(или её подстрок) быть началом или концом новости
                if (range.size() >= m_unMinSz)
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
    m_unAvgLen = m_unAvgLen / m_freq.size();
    m_unAvgFreq = m_unAvgFreq / m_freq.size();
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

void CNewsFinder::GetPossibleRangesUsingTrie()
{
    CTrie<vector<CTagDescription>, CTagDescription> tree(m_mod, m_mod.size(), compare);
    //string str = "ababababab*";
    //CTrie<string, char> tree(str, str.size(), comparestring);
    tree.BuildSuffixTree();
    tree.GetRanges(m_unMinSz, m_unMinFreq);


}

void CNewsFinder::GetNewsRange()
{
    set <pair<vector<CTagDescription>, unsigned int>, ltstr>::iterator setIter;
    vector<pair<vector<CTagDescription>, unsigned int>>::iterator vectorIter;
    
    for(setIter = m_freq.begin(); setIter != m_freq.end(); ++setIter)
    {
        if (setIter->second >= m_unMinFreq && 
            setIter->first.size() <= m_unAvgLen &&
            setIter->second <= m_unAvgFreq)
            possibleTags.push_back(*setIter);
    }
    // сортируем пары
    sort(possibleTags.begin(), possibleTags.end(), CNewsFinder::pred());


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

    for (vectorIter = possibleTags.end() - 1; vectorIter != possibleTags.begin();
         --vectorIter)
    {
        vector<CTagDescription> possibleBegin = vectorIter->first;
        for (vector<pair<vector<CTagDescription>, unsigned int>>::iterator it = vectorIter - 1;; --it)
        {
            vector<CTagDescription> possibleEnd = it->first;
            vector<CTagDescription>::iterator ite = pStrStr(m_mod, possibleBegin);
            unsigned int beg = ite->nTagBegin;
            ite = pStrStr(m_mod, possibleEnd);
            unsigned int end = ite->nTagEnd;
            if (beg - end < m_fileData.size() / m_unMinSz || end - beg < m_fileData.size() / m_unMinSz)
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
            if (getTagCode(tagsToRemove[j]) == m_mod[i].nTagCode)
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
            if (getTagCode(tagsToRemove[j].first) == m_mod[i].nTagCode)
            {
                // стираем тег
                m_mod.erase(m_mod.begin() + i);
                // пока не получим закрывающий тег
                while (getTagCode(tagsToRemove[j].second) != m_mod[i].nTagCode)
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

            if(tag[1] == '/')
                tagCode.bIsClose = 1;
            else
                tagCode.bIsClose = 0;
#ifdef _DEBUG
            if (plpszTagTable[tagCode.nTagCode] == NULL)
            {
                plpszTagTable[tagCode.nTagCode] = (LPTSTR)tag.c_str();
            }
#endif
            return tagCode;
        }
    }

    return tagCode;
}

int CNewsFinder::checksum(const vector<CTagDescription> &src)
{
    unsigned int cnt = 0;

    for (unsigned int i = 0; i < src.size(); ++i)
    {
        // если тег закрывающий.
        if (src[i].bIsClose == 1)
            ++cnt;
    }
    // если закрывающих тегов больше, чем открывающих - возвратим 0
    if (cnt > src.size() / 2 + 1) // M.A.P. загадочная формула
        return 0;

    return 1;
}

// M.A.P. ничего не говорящее название фукнции
int CNewsFinder::checkWordTruePairs(const vector<CTagDescription> &src)
{
    unsigned int i;
    stack<CTagDescription> st;

    for(i = 0; i < src.size(); ++i)
    {
        // если он - открывающий - положим в стек
        if (src[i].bIsClose == 0)
            st.push(src[i]);
        else
        {
            // если стек пуст - закрывающему тегу нет открывающего.
            if (st.empty())
                return 0;
            // пока не получим открывающий для текущего тега, извлекаем
            // теги из стека и проверяем их
            while (st.top().nTagCode + '/' != src[i].nTagCode) // M.A.P. загадочная проверка. Зачем код " + '/'"?
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

int CNewsFinder::getStringFreq(const vector<CTagDescription> &str,
                               unsigned int unPos)
{
    unsigned int freq = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    for (i = 0; i < m_unTableSize; ++i)
    {
        if (m_pTable[i][unPos] != 0)
        {
            unsigned int temp = i;
            while (j < str.size() && temp < m_unTableSize && m_pTable[temp][unPos + j] != 0)
            {
                ++temp;
                ++j; 
            }
            if (j == str.size())
                ++freq;
            j = 0;
        }
    }
    return freq;
}

int CNewsFinder::getTagSubs(const vector<CTagDescription> &src, int nPos)
{
    // выбираем две подстроки длины на 1 меньше
    vector<CTagDescription> first(src.begin(), src.end() - 1);
    vector<CTagDescription> second(src.begin() + 1 ,src.end());
    set <vector<CTagDescription>, tagcodecpr>::iterator setIter;
    int flag = 0;

    if (first.size() < m_unMinSz)
        return 1;

    if (first.size() == m_unMinSz)
    {
        setIter = m_subsArr.find(first);
        if (setIter == m_subsArr.end())
        {
            m_subsArr.insert(first);
            if (checksum(first) && checkWordTruePairs(first))
            {
                unsigned int strFreq = getStringFreq(first, nPos);
                if (strFreq >= m_unMinFreq)
                {
                    m_freq.insert(make_pair(first, strFreq));
                    m_unAvgLen += first.size();
                    m_unAvgFreq += strFreq;
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
                unsigned int strFreq = getStringFreq(second, nPos + 1);
                if (strFreq >= m_unMinFreq)
                {
                    m_freq.insert(make_pair(second, strFreq));
                    m_unAvgLen += second.size();
                    m_unAvgFreq += strFreq;
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
        if (getTagSubs(first, nPos) != 0 && getTagSubs(second, nPos + 1) != 0)
        {
            if (checksum(src) && checkWordTruePairs(src))
            {
                unsigned int strFreq = getStringFreq(src, nPos);
                if (strFreq < m_unMinFreq)
                    return 0;
                m_freq.insert(make_pair(src, strFreq));
                m_unAvgLen += src.size();
                m_unAvgFreq += strFreq;
            }
        }
        else
            return 0;
    }
    return 1;
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

#ifdef _DEBUG

    void CNewsFinder::dbgPrintData(void)
    {
         vector<CTagDescription>::iterator it = m_mod.begin();
         for(; it != m_mod.end(); ++it)
         {
             string word(m_fileData, it->nTagBegin, it->nTagEnd - it->nTagBegin + 1);
             DebugPrint("%s", (LPTSTR)word.c_str());
         }
    }

    void CNewsFinder::printTable()
    {
        for (unsigned int i = 0; i < m_unTableSize; ++i)
        {
            for (unsigned int j = 0; j < m_unTableSize; ++j)
            {
                std::cout << m_pTable[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    string CNewsFinder::getTagWord(vector<CTagDescription> &tagSeq)
    {
        string reti;
        for(unsigned int i = 0; i < tagSeq.size(); ++i)
            reti += plpszTagTable[tagSeq[i].nTagCode];
        return reti;
    }

#endif