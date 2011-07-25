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
     * \fn  CNewsFinder::CNewsFinder(char *filename, unsigned int minSize);
     *
     * \brief   Конструктор.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  filename    Имя файла с новостями.
     * \param   minSize             Минимальная длина последовательности тегов, ограничивающих
     *                              новость и частота встречи новости.
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
     * \fn  void CNewsFinder::init(vector<pair<string, string>> &remDoubleTag, vector<string> remTag);
     *
     * \brief   Инчиализация.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  remDoubleTag    Список двойных тегов.
     * \param   remTag                  Список одинарных тегов.
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
        bool operator()(pair<string, int> left, pair<string, int> right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.length() < right.first.length();
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
        bool operator()(pair<string, int> left, pair<string, int> right) const
        {
            return strcmp(left.first.c_str(), right.first.c_str()) < 0;
        }
    };

    /**
     * \fn  inline void CNewsFinder::LowerCase(CtDK::string * pstr)
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

    string getTag(pair<int, int> &tagPos, const string &src);

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

    int checksum(const string &src);

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

    int checkWordTruePairs(const string &src);

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

    int getStringFreq(const string &src, const string &str, unsigned int pos);

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

    int getTagSubs(const string &src, int pos);

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

    string getNews(char *srcBegin, const string &newsBegin, const string &newsEnd, unsigned int &offset);


    unsigned int MINSZ;
    unsigned int m_minFreq;
    ///< Входной файл
    fstream m_fileIn;
    ///< Выходной файл
    fstream m_fileOut;
    ///< Строчка, содержащая в себе входной файл
    string m_fileData;
    ///< Измененная строчка входного файла
    string m_modifiedData;
    ///< Измененная modifiedData. В clearedData нет удаленных тегов
    string m_clearedData;
    ///< Позиция тега в data
    vector< pair<int, int> > m_realTagPosition;
    ///< Позиция тега в modifiedData
    vector< pair<int, int> > m_modifiedTagPosition;
    ///< Позиция тега в modifiedData
    vector< pair<int, int> > m_clearedTagPosition;

    ///< Set из пар <строка, частота встречи строки>
    //   Повторяющиеся строки не добавляются
    //   От частот никак не зависит
    set <pair <string, int>, CNewsFinder::ltstr> m_freq;
    ///<  Таблица для поиска повторяющихся строк
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
    ///< Размер таблицы - tableSize X tableSize
    unsigned int m_tableSize;
    ///< Средняя длина строки в freq
    int m_avgLen;
    ///< Средняя частота встречи строки
    int m_avgFreq;
    ///< массив возможных начал/концов новостей
    vector<pair<string, int>> possibleTags;
    pair <int, int> tagPosition;
    pair <int, int> modTagPosition;
    string newsBegin;
    string newsEnd;
    set <string> m_subsArr;
};
#endif