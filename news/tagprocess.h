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

using std::pair;
using std::string;
using std::vector;
using std::stack;

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

struct ltstr
{
    bool operator()(pair<string, int> left, pair<string, int> right) const
    {
        return strcmp(left.first.c_str(), right.first.c_str()) < 0;
    }
};

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
 * \fn  string getTag(int tagNum, const string &src);
 *
 * \brief   Получение тега по его порядковому номеру
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   tagNum  Порядковый номер тега.
 * \param   src     Строчка с тегами.
 *
 * \return  Тег.
 */
string getTag(int tagNum, const string &src);

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
void removeTags(vector< pair<int, int> > *tagsPos, const string &src, 
                vector<string> &tagsToRemove);

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
void removeTags(vector< pair<int, int> > *tagsPos, const string &src, 
                vector< pair<string, string> > &tagsToRemove);

/**
 * \fn  int getWordFreq(const string &src, const string &word);
 *
 * \brief   Получение количества встреч word в src. 
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     Строка слов.
 * \param   word    Слово.
 *
 * \return  Частота встречи word в src.
 */
int getWordFreq(const string &src, const string &word);

/**
 * \fn  string getWordString(const string &src, int begin, int end);
 *
 * \brief   Получает строку из слов, с begin`го до end`го слова
 * 			Слово - <*>. Нумерация слов - 1..n
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     Строка.
 * \param   begin   Позиция в src, с которой считывать слова.
 * \param   end     Позиция в src, по которую считывать слова.
 *
 * \return  Строчку из слов c с begin`го до end`го, или до последнего, если
 * 			end > количества слов.
 * 			NULL, если begin > количества слов.
 */
string getWordString(const string &src, int begin, int end);

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
int checksum(const string &src, int flag);

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
int checkWordTruePairs(const string &src, int flag);
/**
 * \fn  int wordSubPtr(const string &src, const string &word);
 *
 * \brief   Возвращает количество тегов в строке, предшествующее началу
 * 			искомой последовательности word
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     Строка.
 * \param   word    Последовательность тегов-слов, подстрока src.
 *
 * \return  Количество тегов.
 */
int wordSubPtr(const string &src, const string &word);

/**
 * \fn  int wordSubPtr(const string &src, const string &word, int offset);
 *
 * \brief   Возвращает количество тегов в строке со сдвигом на offset 
 * 			элементов, предшествующее началу искомой последовательности
 * 			word
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     Строка.
 * \param   word    Последовательность тегов-слов, подстрока src.
 * \param   offset  Сдвиг в src.
 *
 * \return  Количество тегов.
 */
int wordSubPtr(const string &src, const string &word, int offset);

#endif