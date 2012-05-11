/**
 * \file    tagprocess.h
 *
 * \brief   Объявление функций, оперирующих со строками.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include "utils.h"
#include "debug.h"

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
    CNewsFinder(__in LPCSTR fileData, __in int minSize, __in int minFreq);
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

    void Init();
    vector<CNewsBlock> GetRanges();
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

    inline double getAvgDispersion()
    {
        return AvgDispersion;
    }

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
        bool operator()(pair<vector<CTag>, unsigned int> &left,
                        pair<vector<CTag>, unsigned int> &right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.size() < right.first.size();
        }
    };

    struct pred1
    {
        bool operator()(vector<CNews> &left, vector<CNews> &right) const
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
        bool operator()(pair<vector<CTag>, unsigned int> left,
                        pair<vector<CTag>, unsigned int> right) const
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
        bool operator()(const vector<CTag> &left,
                        const vector<CTag> &right) const
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

    CTag getNextTag();

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

    std::string CNewsFinder::getNews(vector<CTag> &newsBegin,
                                     vector<CTag> &newsEnd,
                                     unsigned int &unOffset);

private:
    void calcHeur();
    ///< Минимальный размер последовательности тегов.
    unsigned int m_minLen;

    ///< Минимальная часота встречи последовательности тегов.
    unsigned int m_minFreq;

    ///< Строчка, содержащая в себе входной файл.
    std::string m_fileData;

    ///< Set из пар <Vстрока, частота встречи строки>.
    //   Повторяющиеся строки не добавляются.
    //   От частот никак не зависит.
    std::set <pair<vector<CTag>, unsigned int>, ltstr> m_freq;

    ///< Средняя длина строки в freq.
    unsigned int m_avgLen;

    ///< Средняя частота встречи строки.
    unsigned int m_avgFreq;

    ///< массив возможных начал/концов новостей.
    vector<pair<vector<CTag>, unsigned int>> possibleTags;

    ///< Vстрока начала новости
    vector<CTag> m_newsBegin;

    ///< Vстрока конца новости
    vector<CTag> m_newsEnd;

    // M.A.P. верктор в данном случае оправдан?
    ///< m_fileData, переведенная в Vстроку тегов
    vector<CTag> m_mod;

    ///< Текущая позиция в m_fileData
    unsigned int m_unCurrFileDataPos;
    vector<CNewsBlock> NewsBlocks;
    int m_numberOfNews;
    double AvgDispersion;

    ///< Код последней ошибки
    // 1  : Инициализация пройдена.
    // 0  : Все нормально
    // -1 : Ошибка выделения памяти.
    long m_lLastError;

    // длина видимого в html текста
    long m_lVisibleHtmlLen;
};
#endif