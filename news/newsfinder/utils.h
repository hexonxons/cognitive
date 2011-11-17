/**
 * \file    utils.h
 *
 * \brief   Вспомогательные функции и структуры данных. Объявления.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <windows.h>

/**
 * \struct  CTagDescription
 *
 * \brief   Структура закодированного тега
 *
 * \author  Alexander
 * \date    11/17/2011
 */
struct CTagDescription
{
    // хеш тега
    int nTagCode;
    // закрывающий тег или нет
    bool bIsClose;
    // начала тега в строке
    int nTagBegin;
    // конец тега в строке
    int nTagEnd;
    // сам тег
    std::string tag;

    CTagDescription();
    void Clear();
};

struct CTagSequence 
{
    std::vector<CTagDescription> tag;
    std::vector<std::pair<int, int>> tagRange;
};
//#########################################################################################

int vStrCmp(const std::vector<CTagDescription> &left,
            const std::vector<CTagDescription> &right);

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

int vIsSubstr(const std::vector<CTagDescription> &vStr1,
              const std::vector<CTagDescription> &vStr2);

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

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                               std::vector<CTagDescription> &vStr2,
                                               int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   Функция производит поиск позиции, с которой vStr2 совпадает с vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   Строка, в которой ищем.
 * \param [in,out]  vStr2   Строка, которую ищем.
 *
 * \return  Итератор на начало совпадения.
 * 			Итератор на vStr1.end(), если совпадений не было.
 */

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                               std::vector<CTagDescription> &vStr2);

#endif // __UTILS_H__