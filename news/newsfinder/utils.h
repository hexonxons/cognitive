/**
 * \file    utils.h
 *
 * \brief   Вспомогательные функции и структуры данных. Объявления.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <windows.h>

struct CTagDescription
{
    int nTagCode;
    bool bIsClose;
    int nTagBegin;
    int nTagEnd;
    std::string tag;

    CTagDescription();
    void Clear();
} ;

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
 * \fn  int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   Функция производит поиск позиции, с которой vStr2 совпадает с vStr1,
 * 			начиная с позиции offset в vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   Строка, в которой ищем.
 * \param   vStr2   Строка, которую ищем.
 * \param   offset  Сдвиг.
 *
 * \return  Позицию, с которой начинается совпадение.
 * 			0, если совпадений нет.
 */

int vStrStr(const std::vector<CTagDescription> &vStr1,
            const std::vector<CTagDescription> &vStr2,
            int offset);

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