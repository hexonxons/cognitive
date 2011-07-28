/**
 * \file    tagprocess.h
 *
 * \brief   Объявление функций, оперирующих со строками.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <set>
#include <fstream>
#include <windows.h>
#include "utils.h"

using std::pair;
using std::string;
using std::vector;
using std::stack;
using std::set;
using std::fstream;

/**
 * \class   CNewsFinder
 *
 * \brief   Класс поиска новостей. 
 *
 * \author  Alexander
 * \date    7/21/2011
 */

class CNewsFinder
{
public:

    /**
     * \fn  CNewsFinder::CNewsFinder(char *filename, unsigned int minSize, unsigned int minFreq);
     *
     * \brief   Конструктор.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  filename    Имя файла с новостями.
     * \param   minSize             Минимальная длина последовательности тегов, ограничивающих
     *                              новость и частота встречи новости.
     * \param   minFreq             Минимальная частота встречи последовательности тегов.
     */

    CNewsFinder(char *filename, unsigned int minSize, unsigned int minFreq);

    /**
     * \fn  void CNewsFinder::removeTags(vector<string> &tagsToRemove);
     *
     * \brief   Удаляет теги из строки src.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    Массив тегов для удаления.
     */

    void removeTags(vector<string> &tagsToRemove);

    /**
     * \fn  void CNewsFinder::removeTags(vector< pair<string, string> > &tagsToRemove);
     *
     * \brief   Удаляет парные теги и содержимое между ними из строки src.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    Массив пар тегов для удаления.
     */

    void removeTags(vector< pair<string, string> > &tagsToRemove);

    /**
     * \fn  void CNewsFinder::init(vector<pair<string, string>> &remDoubleTag,
     *      vector<string> &remTag);
     *
     * \brief   Инициализация.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  remDoubleTag    Список двойных тегов.
     * \param [in,out]  remTag          Список одинарных тегов.
     */

    void init(vector<pair<string, string>> &remDoubleTag, vector<string> &remTag);

    /**
     * \fn  void CNewsFinder::getPossibleRanges();
     *
     * \brief   Получение всех возможных последовательностей тегов начала/конца новостей.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void getPossibleRanges();

    /**
     * \fn  void CNewsFinder::getNewsRange();
     *
     * \brief   Получение тегов начала/конца новости.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void getNewsRange();

    /**
     * \fn  void CNewsFinder::writeNews();
     *
     * \brief   Запись новостей в файл.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void writeNews();

private:

    /**
     * \struct  pred
     *
     * \brief   Предикат сравнения пар из строк и частот их встречи.
     *
     * \author  Alexander
     * \date    7/13/2011
     */

    struct pred
    {
        bool operator()(CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> left,
                        CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.size() < right.first.size();
        }
    };

    /**
     * \struct  ltstr
     *
     * \brief   Предикат сравнения в set.
     *
     * \author  Alexander
     * \date    7/19/2011
     */

    struct ltstr
    {
        int operator()(CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> left,
                       CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> right) const
        {
            return vStrCmp(left.first, right.first) < 0;
        }
    };


    struct alphacpr
    {
        int operator()(CPair <CTag, string> left, CPair <CTag, string> right) const
        {
            return strcmp(left.second.c_str(), right.second.c_str()) < 0;
        }
    };

    struct tagcodecpr
    {
        int operator()(const vector<CPair<CTag, CPair<int, int>>> &left,
                        const vector<CPair<CTag, CPair<int, int>>> &right) const
        {
                return vStrCmp(left, right) < 0;
        }
    };


    /**
     * \fn  inline void CNewsFinder::LowerCase(string * pstr)
     *
     * \brief   Приведение строки к строчным буквам.
     *
     * \param [in,out]  pstr    - Изменяемая строка.
     */

    inline void LowerCase(string * pstr)
    {
        if(pstr->size())
            CharLower(&(*pstr->begin()));
    }

    /**
     * \fn  string CNewsFinder::getTag(pair<int, int> &tagPos, const string &src);
     *
     * \brief   Получает тег по его позиции начало-конец в строчке.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagPos  Позиция начало-конец.
     * \param   src             Строчка с тегами.
     *
     * \return  Тег.
     */

    unsigned short CNewsFinder::getTagCode(const string &tag);

    CTriple<CTag, CPair<int, int>, string> getNextTag();
    /**
     * \fn  int CNewsFinder::checksum(const string &src);
     *
     * \brief   Выполняет проверку на превышение количества открывающих тегов количества закрывающих.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src Строка тегов.
     *
     * \return  1, если количество открывающих тегов больше количества закрывающих 0 иначе.
     */

    int checksum(const vector<CPair<CTag, CPair<int, int>>> &src);

    /**
     * \fn  int CNewsFinder::checkWordTruePairs(const string &src);
     *
     * \brief   Выполняет проверку на наличие открывающего тега каждому закрывающему тегу,
     *          присутствующему в этой строке.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src Строка тегов.
     *
     * \return  1, если для каждого закрывающего тега есть открывающий 0, иначе.
     */

    int checkWordTruePairs(const vector<CPair<CTag, CPair<int, int>>> &src);

    /**
     * \fn  int CNewsFinder::getWordCount(const string &src);
     *
     * \brief   Получает количество слов в строке. Слово - &lt;*&gt;.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src Строка.
     *
     * \return  Количество слов.
     */

    int getWordCount(const string &src);

    /**
     * \fn  int CNewsFinder::getStringFreq(const string &src, const string &str, unsigned int pos);
     *
     * \brief   Получает частоту встречи str в src.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   src Исходная строка.
     * \param   str Строка, частоту встречи которой мы ищем.
     * \param   pos Позиция в таблице по вертикали, с которой начались совпадения str c src.
     *
     * \return  Честота встречи str в src.
     */

    int getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &src,
                      const vector<CPair<CTag, CPair<int, int>>> &str,
                      unsigned int pos);

    /**
     * \fn  int CNewsFinder::getTagSubs(const string &src, int pos);
     *
     * \brief   Получает все последовательности тегов, подходящие под условия.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   src Строка тегов, для которой (и всех её подстрок) мы хотим найти частоту встречи и
     *              которая является возможным началом/концом новости.
     * \param   pos Позиция в таблице по вертикали, с которой начались совпадения str c src.
     *
     * \return  1/0.
     */

    int getTagSubs(const vector<CPair<CTag, CPair<int, int>>> &src, int pos);

    /**
     * \fn  string CNewsFinder::getNews(char *srcBegin, const string &newsBegin,
     *      const string &newsEnd, unsigned int &offset);
     *
     * \brief   возвращает текст новости.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  srcBegin    Начало строки тегов.
     * \param   newsBegin           Строка тегов, с которой начинается новость.
     * \param   newsEnd             Строка тегов, которой заканчивается новость.
     * \param [in,out]  offset      Сдвиг в srcBegin.
     *
     * \return  Текст новости.
     */

    string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &srcBegin,
                                vector<CPair<CTag, CPair<int, int>>> &newsBegin,
                                vector<CPair<CTag, CPair<int, int>>> &newsEnd,
                                unsigned int &offset);

    void printTable();

    ///< Минимальный размер последовательности тегов.
    unsigned int m_minSz;
    ///< Минимальная часота встречи последовательности тегов.
    unsigned int m_minFreq;
    ///< Входной файл.
    fstream m_fileIn;
    ///< Выходной файл.
    fstream m_fileOut;
    ///< Строчка, содержащая в себе входной файл.
    string m_fileData;

    ///< Set из пар <строка, частота встречи строки>.
    //   Повторяющиеся строки не добавляются.
    //   От частот никак не зависит.
    set <CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>, ltstr> m_freq;
    ///<  Таблица для поиска повторяющихся строк.
    // Например: abcabcac
    //   a b c a b c a c
    // a * 0 0 * 0 0 * 0
    // b 0 * 0 0 * 0 0 0
    // c 0 0 * 0 0 * 0 *
    // a * 0 0 * 0 0 * 0
    // b 0 * 0 0 * 0 0 0
    // c 0 0 * 0 0 * 0 *
    // a * 0 0 * 0 0 * 0
    // c 0 0 * 0 0 * 0 *
    short **m_pTable;
    ///< Размер таблицы - tableSize X tableSize.
    unsigned int m_tableSize;
    ///< Средняя длина строки в freq.
    unsigned int m_avgLen;
    ///< Средняя частота встречи строки.
    unsigned int m_avgFreq;
    ///< массив возможных начал/концов новостей.
    vector<CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>> possibleTags;
    vector<CPair<CTag, CPair<int, int>>> m_newsBegin;
    vector<CPair<CTag, CPair<int, int>>> m_newsEnd;
    set <vector<CPair<CTag, CPair<int, int>>>, tagcodecpr> m_subsArr;
    vector<CPair<CTag, CPair<int, int>>> mod;
    set <CPair <CTag, string>, alphacpr> alphabet;
    unsigned int currFileDataPos;
};
#endif