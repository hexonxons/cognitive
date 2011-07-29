/**
 * \file    tagprocess.h
 *
 * \brief   Объявление функций, оперирующих со строками.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include "utils.h"

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <windows.h>

using std::vector;

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

    void init(vector<std::pair<std::string, std::string>> &remDoubleTag, vector<std::string> &remTag);

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
     * \brief   Предикат сравнения пар из Vстрок и частот их встречи.
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
     * \brief   Предикат сравнения Vстрок в set.
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

    /**
     * \struct  alphacpr
     *
     * \brief   Предикат сравнения по строке, соответствующей CTag. 
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    struct alphacpr
    {
        int operator()(CPair <CTag, std::string> left, CPair <CTag, std::string> right) const
        {
            return strcmp(left.second.c_str(), right.second.c_str()) < 0;
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
        int operator()(const vector<CPair<CTag, CPair<int, int>>> &left,
                        const vector<CPair<CTag, CPair<int, int>>> &right) const
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
     * \fn  inline void CNewsFinder::LowerCase(string * pstr)
     *
     * \brief   Приведение строки к строчным буквам.
     *
     * \param [in,out]  pstr    - Изменяемая строка.
     */

    inline void LowerCase(std::string * pstr)
    {
        if(pstr->size())
            CharLower(&(*pstr->begin()));
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

    CTriple<CTag, CPair<int, int>, std::string> getNextTag();

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

    int checksum(const vector<CPair<CTag, CPair<int, int>>> &src);

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

    int checkWordTruePairs(const vector<CPair<CTag, CPair<int, int>>> &src);

    /**
     * \fn  int CNewsFinder::getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &str,
     *      unsigned int pos);
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

    int getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &str,
                      unsigned int pos);

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

    int getTagSubs(const vector<CPair<CTag, CPair<int, int>>> &src, int pos);

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

    std::string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &newsBegin,
                                     vector<CPair<CTag, CPair<int, int>>> &newsEnd,
                                     unsigned int &offset);

    /**
     * \fn  void CNewsFinder::printTable();
     *
     * \brief   Печать таблицы.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    void printTable();

private:
    ///< Минимальный размер последовательности тегов.
    unsigned int m_minSz;

    ///< Минимальная часота встречи последовательности тегов.
    unsigned int m_minFreq;

    ///< Входной файл.
    std::fstream m_fileIn;

    ///< Выходной файл.
    std::fstream m_fileOut;

    ///< Строчка, содержащая в себе входной файл.
    std::string m_fileData;

    ///< Set из пар <Vстрока, частота встречи строки>.
    //   Повторяющиеся строки не добавляются.
    //   От частот никак не зависит.
    std::set <CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>, ltstr> m_freq;

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

    ///< Vстрока начала новости
    vector<CPair<CTag, CPair<int, int>>> m_newsBegin;

    ///< Vстрока конца новости
    vector<CPair<CTag, CPair<int, int>>> m_newsEnd;

    ///< Set из всех подстрок
    std::set <vector<CPair<CTag, CPair<int, int>>>, tagcodecpr> m_subsArr;

    ///< m_fileData, переведенная в Vстроку тегов
    vector<CPair<CTag, CPair<int, int>>> m_VtagFileData;

    ///< Алфавит
    std::set <CPair <CTag, std::string>, alphacpr> m_alphabet;

    ///< Текущая позиция в m_fileData
    unsigned int currFileDataPos;

};
#endif