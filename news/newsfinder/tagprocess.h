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

    CNewsFinder(__in LPCSTR fileName, __in int minSize, __in int minFreq, __in int numberOfNews);

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
     * \fn  void CNewsFinder::init(vector<std::pair<std::string, std::string>> &remDoubleTag,
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

    void Init(vector<std::pair<std::string, std::string>> &remDoubleTag, vector<std::string> &remTag);
    vector<CTagSequence> GetRanges();
    void GetPossibleRanges();

    /**
     * \fn  void CNewsFinder::getNewsRange();
     *
     * \brief   Получение тегов начала/конца новости.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void GetNewsRange();

    /**
     * \fn  void CNewsFinder::writeNews(LPCSTR lpcszOutFileName);
     *
     * \brief   Запись новостей в файл.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void WriteNews(LPCSTR lpcszOutFileName);

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
        bool operator()(pair<vector<CTagDescription>, unsigned int> left,
                        pair<vector<CTagDescription>, unsigned int> right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.size() < right.first.size();
        }
    };

    struct pred1
    {
        bool operator()(vector<pair<int, int>> left,
            vector<pair<int, int>> right) const
        {
            return left.size() < right.size();
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
        bool operator()(pair<vector<CTagDescription>, unsigned int> left,
                        pair<vector<CTagDescription>, unsigned int> right) const
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
     * \fn  inline void CNewsFinder::LowerCase(string *pStr)
     *
     * \brief   Приведение строки к строчным буквам.
     *
     * \param [in,out]  pstr    - Изменяемая строка.
     */

    inline void LowerCase(std::string *pStr)
    {
        if(pStr->size())
            CharLower(&(*pStr->begin()));
    }

    /**
     * \fn  void CNewsFinder::removeTags(vector<std::string> &tagsToRemove);
     *
     * \brief   Удаляет теги из Vстроки mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    Массив тегов для удаления.
     */

    void removeTags(vector<std::string> &tagsToRemove);

    /**
     * \fn  void CNewsFinder::removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);
     *
     * \brief   Удаляет парные теги и содержимое между ними из Vстроки mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    Массив пар тегов для удаления.
     */

     void removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);

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

    int checkTag(const vector<CTagDescription> &src);

    /**
     * \fn  std::string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &newsBegin,
     *      vector<CPair<CTag, CPair<int, int>>> &newsEnd, unsigned int &offset);
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
                                     unsigned int &unOffset);
#ifdef _DEBUG

    void dbgPrintData(void);

    std::string getTagWord(vector<CTagDescription> &tagSeq);
#endif

private:
    ///< Минимальный размер последовательности тегов.
    unsigned int m_minLen;

    ///< Минимальная часота встречи последовательности тегов.
    unsigned int m_minFreq;

    ///< Строчка, содержащая в себе входной файл.
    std::string m_fileData;

    ///< Set из пар <Vстрока, частота встречи строки>.
    //   Повторяющиеся строки не добавляются.
    //   От частот никак не зависит.
    std::set <pair<vector<CTagDescription>, unsigned int>, ltstr> m_freq;

    ///< Средняя длина строки в freq.
    unsigned int m_avgLen;

    ///< Средняя частота встречи строки.
    unsigned int m_avgFreq;

    ///< массив возможных начал/концов новостей.
    vector<pair<vector<CTagDescription>, unsigned int>> possibleTags;

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
    unsigned int m_unCurrFileDataPos;
    vector<CTagSequence> tags;
    int m_numberOfNews;

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