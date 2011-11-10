/**
 * \file    utils.h
 *
 * \brief   ��������������� ������� � ��������� ������. ����������.
 */

#ifndef __UTILS_H__
#define __UTILS_H__
#include "stdafx.h"

#include <vector>
#include <windows.h>

typedef struct 
{
    int nTagCode;
    bool bIsClose;
    int nTagBegin;
    int nTagEnd;
} CTagDescription;

typedef struct 
{
    std::vector<CTagDescription> tag;
    std::vector<std::pair<int, int>> tagRange;
} CTagSequence;
//#########################################################################################

int vStrCmp(const std::vector<CTagDescription> &left,
            const std::vector<CTagDescription> &right);

/**
 * \fn  int vIsSubstr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   ������� ���������� ����� vStr2 � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   ������, � ������� ����.
 * \param   vStr2   ������, ������� ����.
 *
 * \return  1, ���� � vStr1 ���������� vStr2
 * 			0, �����.
 */

int vIsSubstr(const std::vector<CTagDescription> &vStr1,
              const std::vector<CTagDescription> &vStr2);

/**
 * \fn  int vStrStr(const std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      const std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   ������� ���������� ����� �������, � ������� vStr2 ��������� � vStr1,
 * 			������� � ������� offset � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param   vStr1   ������, � ������� ����.
 * \param   vStr2   ������, ������� ����.
 * \param   offset  �����.
 *
 * \return  �������, � ������� ���������� ����������.
 * 			0, ���� ���������� ���.
 */

int vStrStr(const std::vector<CTagDescription> &vStr1,
            const std::vector<CTagDescription> &vStr2,
            int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2, int offset);
 *
 * \brief   ������� ���������� ����� �������, � ������� vStr2 ��������� � vStr1,
 * 			������� � ������� offset � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   ������, � ������� ����.
 * \param [in,out]  vStr2   ������, ������� ����.
 * \param   offset          �����.
 *
 * \return  �������� �� ������ ����������.
 * 			�������� �� vStr1.end(), ���� ���������� �� ����.
 */

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                               std::vector<CTagDescription> &vStr2,
                                               int offset);

/**
 * \fn  std::vector<CPair<CTag, CPair<int, int>>>::iterator pStrStr(std::vector<CPair<CTag, CPair<int, int>>> &vStr1,
 *      std::vector<CPair<CTag, CPair<int, int>>> &vStr2);
 *
 * \brief   ������� ���������� ����� �������, � ������� vStr2 ��������� � vStr1.
 *
 * \author  Alexander
 * \date    7/28/2011
 *
 * \param [in,out]  vStr1   ������, � ������� ����.
 * \param [in,out]  vStr2   ������, ������� ����.
 *
 * \return  �������� �� ������ ����������.
 * 			�������� �� vStr1.end(), ���� ���������� �� ����.
 */

std::vector<CTagDescription>::iterator pStrStr(std::vector<CTagDescription> &vStr1,
                                               std::vector<CTagDescription> &vStr2);





#endif // __UTILS_H__