#include "tagprocess.h"

#include <fstream>

using namespace std;

/**
 * \fn  int main()
 *
 * \brief   Main entry-point for this application.
 * \algo
 * 			ќписание алгоритма:
 * 			- ‘айл считываетс€ в строчку data. N символов.\n\n
 * 			
 * 			- ѕроходим по всей data, ищем '<' и '>', помечаем из позиции и 
 * 			кладем номера позиций в tagPosition.\n
 * 			¬ этот же проход записываем в modifiedData теги <*>, 
 * 			т.е выкидываем всю информацию, кроме названи€ тега,\n
 * 			и записываем в modTagPosition позиции тега в modifiedData.\n
 * 			<b>O(N)</b>\n\n
 * 			
 * 			- »з modifiedTagPosition стираем все заданные теги, 
 * 			т.е записываем на место позиций тегов (-1, -1). \n
 * 			ѕроход по всем тегам        * свер€ем со всеми тегами дл€ удалени€ * tolower каждый тег  * сравнение тегов  * примерно така€ же сложность дл€ удалени€ remDoubleTag.\n
 * 			<b>O(modifiedTagPosition.size) * O(remTag.size)                       * O(each tag size)    * O(each tag size) * Const</b>\n\n
 * 			
 * 			- ѕроходим по modifiedTagPosition и записываем в clearedData все нестертые теги. 
 * 			ќткрывающие теги будут иметь код > 0, закрывающие < 0.\n
 * 			¬ clearedTagPosition записываем номера этих тегов в modifiedTagPosition.\n
 * 			<b>O(modifiedTagPosition.size)</b>\n\n
 * 			
 * 			- «аполн€ем таблицу [clearedData.size x clearedData.size].\n
 * 			<b>O(clearedData.size ^ 2)</b>\n\n
 * 			
 * 			- —читываем строчки по диагонали, начина€ с самой ближней к главной диагонали.\n
 * 		    <b>O(clearedData.size ^ 2 / 2)</b>\n\n
 * 		    
 * 		    - ƒл€ каждой подстроки длиной >= MINSZ выполн€ем 2 проверки,\n
 * 		    <b>O(2 * substring.size)</b>\n
 * 		    узнаем частоту встречи подстроки,\n
 * 		    <b>O(clearedData.size * substring.size)</b>\n
 * 		    кладем еЄ в set, прибавл€€ длину строки и частоту встречи к avgLen и avgFreq.\n
 * 		    <b>O(log(freq.size))</b>\n
 * 		    «апускаем все это рекурсивно, дл€ 2х подстрок длины length - 1.\n
 * 		    ≈сли частота встречи подстроки < MINSZ - возвращаем 0.\n
 * 		    ≈сли результат запуска дл€ 2х подстрок == 0 - возвращаем 0.\n
 * 		    »наче выполн€ем все дл€ вход€щей строки.\n
 * 		    
 * 		    - –ассчитываем avgLen и avgFreq, перегон€ем из set`а в vector все,\n
 * 		    у чего частота встречи < avgFreq * 1.6 и длина < avgLen * 1.5.\n
 * 		    —ортируем этот вектор.\n
 * 		    <b>O(possibleTags.size * log(possibleTags.size))</b>\n\n
 * 		    
 * 		    - »дем с конца вектора, ищем 2 значени€ с одинаковым количеством встреч - конец и начало новости.\n
 * 		    ѕолучаем набор тегов из clearedData.  ќтдаем его getNews.\n
 * 		    <b>[O(2 * clearedData.size) + O(clearedTagPosition.size)] * possibleTags.size </b>\n\n
 * 		    
 * 		    
 * 		    
 * 			
 *
 * \author  Alexander
 * \date    7/20/2011
 *
 * \return  Exit-code for the process - 0 for success, else an error code.
 */


int main()
{
    // массивы ненужных тегов
    vector<pair<string, string>> remDoubleTag;
    vector<string> remTag;
    CNewsFinder finder("news", 8);

    remDoubleTag.push_back(make_pair("<script>", "</script>"));
    remDoubleTag.push_back(make_pair("<noscript>", "</noscript>"));
    remDoubleTag.push_back(make_pair("<form>", "</form>"));
    remDoubleTag.push_back(make_pair("<iframe>", "</iframe>"));
    remDoubleTag.push_back(make_pair("<ul>", "</ul>"));
    remDoubleTag.push_back(make_pair("<span>", "</span>"));
    remDoubleTag.push_back(make_pair("<p>", "</p>"));

    remTag.push_back("<!doctype>");
    remTag.push_back("<html>");
    remTag.push_back("</html>");
    remTag.push_back("<head>");
    remTag.push_back("</head>");
    remTag.push_back("<body>");
    remTag.push_back("</body>");
    remTag.push_back("<link>");
    remTag.push_back("<title>");
    remTag.push_back("</title>");
    remTag.push_back("<meta>");
    remTag.push_back("</meta>");
    remTag.push_back("<base>");
    remTag.push_back("<!-->");
    remTag.push_back("<li>");
    remTag.push_back("</li>");
    remTag.push_back("<p>");
    remTag.push_back("</p>");

    finder.init(remDoubleTag, remTag);
    finder.getPossibleRanges();
    finder.getNewsRange();
    finder.writeNews();

	return 0;
}