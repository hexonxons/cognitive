/**
 * \file    utils.h
 *
 * \brief   Вспомогательные функции и структуры данных. Объявления.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

/************************************************************************/
/* Возможные эвристики блока новостей
/************************************************************************/
struct NewsBlockHeuristics
{
    // Конструктор
    NewsBlockHeuristics();
    // Процентное отношение длины HTML блока к длине всей страницы
    double PercentageToAllHtmlLen;
    // Процентное отношение длины видимого текста блока к длине видимого текста всей страницы
    double PercentageToAllVisibleHtmlLen;
    // Дисперсия новостей по отношению друг к другу
    // Вычисляется, как сумма квадратов разности (среднего расстояния между новостями) и 
    // (расстояния между 2мя соседними новостями * количество новостей)
    // Является отрицательной характеристикой
    double Dispersion;
    // Сумма расстояний между новостями
    long NewsDistanceSumm;
    // Сумма пересечений новостей данного блока с новостями других блоков
    // Увеличивается только тогда, когда новость другого блока полностью попадает в новость этого блока
    // Является отрицательной характеристикой
    long IntersectionWithOtherBlocks;
    // Вес эвристики. Чем меньше - тем лучше.
    int Weight;
};

/************************************************************************/
/* Возможные эвристики каждой новости в отдельности
/************************************************************************/
struct NewsHeuristics
{
    // Конструктор
    NewsHeuristics();
    // Расстояние до следующей новости
    long DistanceToNextNews;
    // Процентное отношение длины HTML новости к длине всей страницы
    double PercentageToAllHtmlLen;
    // Процентное отношение длины видимого текста новости к длине видимого текста всей страницы
    double PercentageToAllVisibleHtmlLen;
};

/************************************************************************/
/* Структура одного тега
/************************************************************************/
struct CTag
{
    // текстовое представление тега
    std::string tag;
    // начало тега в строке
    int TagBegin;
    // конец тега в строке
    int TagEnd;
    // хеш тега
    int TagHashCode;
    // закрывающий тег или нет
    bool IsCloseTag;
 
    CTag();
    void Clear();
};

/************************************************************************/
/* Структура одной новости
/************************************************************************/
struct CNews
{
    // Конструктор
    CNews();
    // Начало последовательности тегов новости в тексте
    int NewsBegin;
    // Конец последовательности
    int NewsEnd;
    // Вся строка новости
    std::string TagString;
    // Структура эвристики новости
    NewsHeuristics Heuristics;  
};

/************************************************************************/
/* Структура новостного блока
/************************************************************************/
struct CNewsBlock 
{
    // Последовательность тегов, основывающая блок
    std::vector<CTag> tag;
    // Набор всех новостей
    std::vector<CNews> NewsRange;
    // Структура эвристики блока
    NewsBlockHeuristics BlockHeuristics;
};

//#########################################################################################

int vStrCmp(const std::vector<CTag> &left,
            const std::vector<CTag> &right);

/**
 * \fn  int vIsSubstr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   Функция производит поиск vStr2 в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   Строка, в которой ищем.
 * \param   vStr2   Строка, которую ищем.
 *
 * \return  1, если в vStr1 содержится vStr2
 * 			0, иначе.
 */

int vIsSubstr(const std::vector<CTag> &vStr1,
              const std::vector<CTag> &vStr2);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   Функция производит поиск позиции, с которой vStr2 совпадает с vStr1,
 * 			начиная с позиции offset в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   Строка, в которой ищем.
 * \param [in,out]  vStr2   Строка, которую ищем.
 * \param   offset          Сдвиг.
 *
 * \return  Итератор на начало совпадения.
 * 			Итератор на vStr1.end(), если совпадений не было.
 */

std::vector<CTag>::iterator pStrStr(std::vector<CTag> &vStr1,
                                               std::vector<CTag> &vStr2,
                                               int offset);


std::vector<CTag>::iterator pStrStr(std::vector<CTag> &vStr1,
                                               std::vector<CTag> &vStr2);

char *GetPageSource(const char *URL);

#endif // __UTILS_H__