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

using std::pair;
using std::string;
using std::vector;
using std::stack;
using std::set;
using std::fstream;


class CNewsFinder
{
public:
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

    CNewsFinder(char *filename, unsigned int minSize);

    /**
     * \fn  string getTag(pair<int, int> &tagPos, const string &src);
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
     * \fn  void removeTags(vector< pair<int, int> > *tagsPos, 
     * 		const string &src, vector<string> &tagsToRemove);
     *
     * \brief   Удаляет теги из строки src.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsPos         Массив пар начало-конец тегов.
     * \param   src                     Строка с тегами.
     * \param [in,out]  tagsToRemove    Массив тегов для удаления.
     */
    void removeTags(vector<string> &tagsToRemove);

    /**
     * \fn  void removeTags(vector< pair<int, int> > *tagsPos,
     * 		const string &src, vector< pair<string, string> > &tagsToRemove);
     *
     * \brief   Удаляет парные теги и содержимое между ними из строки src.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsPos         Массив пар начало-конец тегов.
     * \param   src                     Строка с тегами.
     * \param [in,out]  tagsToRemove    Массив пар тегов для удаления.
     */
    void removeTags(vector< pair<string, string> > &tagsToRemove);


    /**
     * \fn  int getWordCount(const string &src);
     *
     * \brief   Получает количество слов в строке.
     * 			Слово - <*>.
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
     * \fn  int checksum(const string &src);
     *
     * \brief   Выполняет проверку на превышение количества открывающих тегов
     * 			количества закрывающих.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src Строка тегов.
     *
     * \return  1, если количество открывающих тегов больше количества
     * 			закрывающих
     * 			0 иначе
     */
    int checksum(const string &src);

    /**
     * \fn  int checkWordTruePairs(const string &src);
     *
     * \brief   Выполняет проверку на наличие открывающего тега каждому
     * 			закрывающему тегу, присутствующему в этой строке.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src Строка тегов.
     *
     * \return  1, если для каждого закрывающего тега есть открывающий
     * 			0, иначе
     */
    int checkWordTruePairs(const string &src);

    /**
     * \fn  int getStringFreq(const string &src, const string &str, short **table,
     *      unsigned int tableSz, int pos);
     *
     * \brief   Получает частоту встречи str в src.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   src             Исходная строка.
     * \param   str             Строка, частоту встречи которой мы ищем.
     * \param [in,out]  table   Таблица по которой ищем повторения.
     * \param   tableSz         Размер таблицы.
     * \param   pos             Позиция в таблице по вертикали, с которой начались совпадения str c
     *                          src.
     *
     * \return  Честота встречи str в src.
     */

    int getStringFreq(const string &src, const string &str, short **table, 
                      unsigned int tableSz, int pos);

    /**
     * \fn  int getTagSubs(set <pair <string, int>, ltstr> &freq, const string &src,
     *      const string &dataString, short **table, unsigned int tableSz, int pos, int &avgLen,
     *      int &avgFreq);
     *
     * \brief   Получает все последовательности тегов, подходящие под условия.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param [in,out]  freq        Set пар строка - частота встречи строки.
     * \param   src                 Строка тегов, для которой (и всех её подстрок) мы хотим найти
     *                              частоту встречи и которая является возможным началом/концом
     *                              новости.
     * \param   dataString          Исходная строка.
     * \param [in,out]  table       Таблица по которой ищем повторения.
     * \param   tableSz             Размер таблицы.
     * \param   pos                 Позиция в таблице по вертикали, с которой начались совпадения str
     *                              c src.
     * \param [in,out]  avgLen      Средняя длина последовательности.
     * \param [in,out]  avgFreq     Средняя частота последовательности.
     *
     * \return  1/0.
     */

    int getTagSubs(set <pair <string, int>, ltstr> &freq, const string &src, const string &dataString,
                   short **table, unsigned int tableSz, int pos, int &avgLen, int &avgFreq);

    /**
     * \fn  string getNews(const string &data, char *srcBegin, const string &newsBegin,
     *      const string &newsEnd, vector< pair<int, int> > &clearedTagPosition,
     *      vector< pair<int, int> > &modifiedTagPosition, vector< pair<int, int> > &realTagPosition,
     *      int &offset);
     *
     * \brief   возвращает текст новости.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param   data                        Строка, содержащая весь файл.
     * \param [in,out]  srcBegin            Начало строки тегов
     * \param   newsBegin                   Строка тегов, с которой начинается новость.
     * \param   newsEnd                     Строка тегов, которой заканчивается новость.
     * \param [in,out]  clearedTagPosition  Позиция тегов в clearedData.
     * \param [in,out]  modifiedTagPosition Позиция тегов в modifiedData.
     * \param [in,out]  realTagPosition     Позиция тегов в data/
     * \param [in,out]  offset              Сдвиг в srcBegin.
     *
     * \return  Текст новости.
     */

    string getNews(const string &data, char *srcBegin, const string &newsBegin, const string &newsEnd,
                   vector< pair<int, int> > &clearedTagPosition, vector< pair<int, int> > &modifiedTagPosition,
                   vector< pair<int, int> > &realTagPosition, int &offset);

    void init();
    void getPossibleRanges();
    void getNewsRange();
    void writeNews();
protected:
private:
    int MINSZ;
    // Входной файл
    fstream fin;
    // Выходной файл
    fstream fout;
    // Строчка, содержащая в себе входной файл
    string data;
    // Измененная строчка входного файла
    string modifiedData;
    // Измененная modifiedData. В clearedData нет удаленных тегов
    string clearedData;
    // Позиция тега в data
    vector< pair<int, int> > realTagPosition;
    // Позиция тега в modifiedData
    vector< pair<int, int> > modifiedTagPosition;
    // Позиция тега в modifiedData
    vector< pair<int, int> > clearedTagPosition;
    // Set из пар <строка, частота встречи строки>
    // Повторяющиеся строки не добавляются
    // От частот никак не зависит
    set <pair <string, int>, CNewsFinder::ltstr> freq;
    set <pair <string, int>, CNewsFinder::ltstr>::iterator setIter;
    // Таблица для поиска повторяющихся строк
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
    short **table;
    // Размер таблицы - tableSize X tableSize
    int tableSize;
    // Средняя длина строки в freq
    int avgLen;
    // Средняя частота встречи строки
    int avgFreq;
    // массив возможных начал/концов новостей
    vector<pair<string, int>> possibleTags;
    vector<pair<string, int>>::iterator vectorIter;
    pair <int, int> tagPosition;
    pair <int, int> modTagPosition;
    // чистка от ненужных тегов
    vector<pair<string, string>> remDoubleTag;
    vector<string> remTag;
};



#endif