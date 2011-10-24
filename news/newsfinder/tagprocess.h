/**
 * \file    tagprocess.h
 *
 * \brief   Объявление функций, оперирующих со строками.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include "utils.h"
#include "../include/debug.h"

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <windows.h>

using std::vector;
using std::pair;

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
     * \fn  CNewsFinder::CNewsFinder(char *filename, UINT minSize, UINT minFreq);
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

    CNewsFinder(LPCSTR lpcszInFileName, UINT unMinSize, UINT unMinFreq);

    /**
     * \fn  CNewsFinder::~CNewsFinder();
     *
     * \brief   Деструктор.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    ~CNewsFinder();

    /**
     * \fn  VOID CNewsFinder::init(vector<std::pair<std::string, std::string>> &remDoubleTag,
     *      vector<std::string> &remTag);
     *
     * \brief   Инициализация.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  remDoubleTag    Список двойных тегов.
     * \param [in,out]  remTag          Список одинарных тегов.
     */

    VOID Init(vector<std::pair<std::string, std::string>> &remDoubleTag, vector<std::string> &remTag);

    /**
     * \fn  VOID CNewsFinder::getPossibleRanges();
     *
     * \brief   Получение всех возможных последовательностей тегов начала/конца новостей.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    VOID GetPossibleRanges();

    /**
     * \fn  VOID CNewsFinder::getNewsRange();
     *
     * \brief   Получение тегов начала/конца новости.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    VOID GetNewsRange();

    /**
     * \fn  VOID CNewsFinder::writeNews(LPCSTR lpcszOutFileName);
     *
     * \brief   Запись новостей в файл.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    VOID WriteNews(LPCSTR lpcszOutFileName);

    /**
     * \fn  ULONG CNewsFinder::dwGetlastError();
     *
     * \brief   Возвращает код последней ошибки.
     *
     * \author  Alexander
     * \date    8/31/2011
     */

    long GetlastError();

private:

    /**
     * \struct  pred
     *
     * \brief   Предикат сравнения пар из Vстрок и частот их встречи.
     *
     * \author  Alexander
     * \date    7/13/2011
     */

    struct pred
    {
        bool operator()(pair<vector<CTagDescription>, UINT> left,
                        pair<vector<CTagDescription>, UINT> right) const
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
     * \brief   Предикат сравнения Vстрок в set.
     *
     * \author  Alexander
     * \date    7/19/2011
     */

    struct ltstr
    {
        bool operator()(pair<vector<CTagDescription>, UINT> left,
                        pair<vector<CTagDescription>, UINT> right) const
        {
            return vStrCmp(left.first, right.first) < 0;
        }
    };

    /**
     * \struct  tagcodecpr
     *
     * \brief   Предикат сравнения Vстрок. 
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    struct tagcodecpr
    {
        bool operator()(const vector<CTagDescription> &left,
                        const vector<CTagDescription> &right) const
        {
                return vStrCmp(left, right) < 0;
        }
    };

    /**
     * \fn  CNewsFinder::CNewsFinder();
     *
     * \brief   Конструктор по умолчанию.
     * 			Запрещен вызов.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    CNewsFinder();

    /**
     * \fn  inline VOID CNewsFinder::LowerCase(string *pStr)
     *
     * \brief   Приведение строки к строчным буквам.
     *
     * \param [in,out]  pstr    - Изменяемая строка.
     */

    inline VOID LowerCase(std::string *pStr)
    {
        if(pStr->size())
            CharLower(&(*pStr->begin()));
    }

    /**
     * \fn  VOID CNewsFinder::removeTags(vector<std::string> &tagsToRemove);
     *
     * \brief   Удаляет теги из Vстроки mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    Массив тегов для удаления.
     */

    VOID removeTags(vector<std::string> &tagsToRemove);

    /**
     * \fn  VOID CNewsFinder::removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);
     *
     * \brief   Удаляет парные теги и содержимое между ними из Vстроки mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    Массив пар тегов для удаления.
     */

     VOID removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);

    /**
     * \fn  unsigned short CNewsFinder::getTagCode(const std::string &tag);
     *
     * \brief   Получает код тега.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   tag Тег.
     *
     * \return  Код тега.
     */

    unsigned short CNewsFinder::getTagCode(const std::string &tag);

    /**
     * \fn  CTriple<CTag, CPair<int, int>, std::string> CNewsFinder::getNextTag();
     *
     * \brief   Получает следующий тег.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \return  CTriple из тега, пары <начало, конец> тега в тексте и сам тег.
     */

    CTagDescription getNextTag();

    /**
     * \fn  int CNewsFinder::checksum(const vector<CPair<CTag, CPair<int, int>>> &src);
     *
     * \brief   Выполняет проверку на превышение количества открывающих тегов количества закрывающих.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src Vстрока тегов.
     *
     * \return  1, если количество открывающих тегов больше количества закрывающих 0 иначе.
     */

    int checksum(const vector<CTagDescription> &src);

    /**
     * \fn  int CNewsFinder::checkWordTruePairs(const vector<CPair<CTag, CPair<int, int>>> &src);
     *
     * \brief   Выполняет проверку на наличие открывающего тега каждому закрывающему тегу,
     *          присутствующему в этой Vстроке.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src Vстрока тегов.
     *
     * \return  1, если для каждого закрывающего тега есть открывающий
     * 			0, иначе.
     */

    int checkWordTruePairs(const vector<CTagDescription> &src);

    /**
     * \fn  int CNewsFinder::getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &str,
     *      UINT pos);
     *
     * \brief   Получает частоту встречи str в mod.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   str Vстрока, частоту встречи которой мы ищем.
     * \param   pos Позиция в таблице по вертикали, с которой начались совпадения str c src.
     *
     * \return  Честота встречи str в mod.
     */

    int getStringFreq(const vector<CTagDescription> &str,
                      UINT unPos);

    /**
     * \fn  int CNewsFinder::getTagSubs(const vector<CPair<CTag, CPair<int, int>>> &src, int pos);
     *
     * \brief   Получает все последовательности тегов, подходящие под условия.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   src Vстрока тегов, для которой (и всех её подстрок) мы хотим найти частоту встречи и
     *              которая является возможным началом/концом новости.
     * \param   pos Позиция в таблице по вертикали, с которой начались совпадения str c src.
     *
     * \return  1/0.
     */

    int getTagSubs(const vector<CTagDescription> &src, int nPos);

    /**
     * \fn  std::string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &newsBegin,
     *      vector<CPair<CTag, CPair<int, int>>> &newsEnd, UINT &offset);
     *
     * \brief   возвращает текст новости.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  newsBegin   Vстрока тегов, с которой начинается новость.
     * \param [in,out]  newsEnd     Vстрока тегов, которой заканчивается новость.
     * \param [in,out]  offset      Сдвиг в mod.
     *
     * \return  Текст новости.
     */

    std::string CNewsFinder::getNews(vector<CTagDescription> &newsBegin,
                                     vector<CTagDescription> &newsEnd,
                                     UINT &unOffset);
#ifdef _DEBUG
    /**
     * \fn  VOID CNewsFinder::printTable();
     *
     * \brief   Печать таблицы.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    VOID printTable();

    VOID dbgPrintData(VOID);

    std::string getTagWord(vector<CTagDescription> &tagSeq);
#endif

private:
    ///< Минимальный размер последовательности тегов.
    UINT m_unMinSz;

    ///< Минимальная часота встречи последовательности тегов.
    UINT m_unMinFreq;

    ///< Строчка, содержащая в себе входной файл.
    std::string m_fileData;

    ///< Set из пар <Vстрока, частота встречи строки>.
    //   Повторяющиеся строки не добавляются.
    //   От частот никак не зависит.
    std::set <pair<vector<CTagDescription>, UINT>, ltstr> m_freq;

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
    bool **m_pTable;

    ///< Размер таблицы - tableSize X tableSize.
    UINT m_unTableSize;

    ///< Средняя длина строки в freq.
    UINT m_unAvgLen;

    ///< Средняя частота встречи строки.
    UINT m_unAvgFreq;

    ///< массив возможных начал/концов новостей.
    vector<pair<vector<CTagDescription>, UINT>> possibleTags;

    ///< Vстрока начала новости
    vector<CTagDescription> m_newsBegin;

    ///< Vстрока конца новости
    vector<CTagDescription> m_newsEnd;

    ///< Set из всех подстрок
    std::set <vector<CTagDescription>, tagcodecpr> m_subsArr;

    // M.A.P. верктор в данном случае оправдан?
    ///< m_fileData, переведенная в Vстроку тегов
    vector<CTagDescription> m_mod;

    ///< Текущая позиция в m_fileData
    UINT m_unCurrFileDataPos;

    ///< Код последней ошибки
    // 1  : Инициализация пройдена.
    // 0  : Все нормально
    // -1 : Ошибка выделения памяти.
    long m_lLastError;

#ifdef _DEBUG
    LPTSTR *plpszTagTable;
#endif
};
#endif