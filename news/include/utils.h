/**
 * \file    utils.h
 *
 * \brief   ��������������� ������� � ��������� ������. ����������.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

/************************************************************************/
/* ��������� ��������� ����� ��������
/************************************************************************/
struct NewsBlockHeuristics
{
    // �����������
    NewsBlockHeuristics();
    // ���������� ��������� ����� HTML ����� � ����� ���� ��������
    double PercentageToAllHtmlLen;
    // ���������� ��������� ����� �������� ������ ����� � ����� �������� ������ ���� ��������
    double PercentageToAllVisibleHtmlLen;
    // ��������� �������� �� ��������� ���� � �����
    // �����������, ��� ����� ��������� �������� (�������� ���������� ����� ���������) � 
    // (���������� ����� 2�� ��������� ��������� * ���������� ��������)
    // �������� ������������� ���������������
    double Dispersion;
    // ����� ���������� ����� ���������
    long NewsDistanceSumm;
    // ����� ����������� �������� ������� ����� � ��������� ������ ������
    // ������������� ������ �����, ����� ������� ������� ����� ��������� �������� � ������� ����� �����
    // �������� ������������� ���������������
    long IntersectionWithOtherBlocks;
    // ��� ���������. ��� ������ - ��� �����.
    int Weight;
};

/************************************************************************/
/* ��������� ��������� ������ ������� � �����������
/************************************************************************/
struct NewsHeuristics
{
    // �����������
    NewsHeuristics();
    // ���������� �� ��������� �������
    long DistanceToNextNews;
    // ���������� ��������� ����� HTML ������� � ����� ���� ��������
    double PercentageToAllHtmlLen;
    // ���������� ��������� ����� �������� ������ ������� � ����� �������� ������ ���� ��������
    double PercentageToAllVisibleHtmlLen;
};

/************************************************************************/
/* ��������� ������ ����
/************************************************************************/
struct CTag
{
    // ��������� ������������� ����
    std::string tag;
    // ������ ���� � ������
    int TagBegin;
    // ����� ���� � ������
    int TagEnd;
    // ��� ����
    int TagHashCode;
    // ����������� ��� ��� ���
    bool IsCloseTag;
 
    CTag();
    void Clear();
};

/************************************************************************/
/* ��������� ����� �������
/************************************************************************/
struct CNews
{
    // �����������
    CNews();
    // ������ ������������������ ����� ������� � ������
    int NewsBegin;
    // ����� ������������������
    int NewsEnd;
    // ��� ������ �������
    std::string TagString;
    // ��������� ��������� �������
    NewsHeuristics Heuristics;  
};

/************************************************************************/
/* ��������� ���������� �����
/************************************************************************/
struct CNewsBlock 
{
    // ������������������ �����, ������������ ����
    std::vector<CTag> tag;
    // ����� ���� ��������
    std::vector<CNews> NewsRange;
    // ��������� ��������� �����
    NewsBlockHeuristics BlockHeuristics;
};

//#########################################################################################

int vStrCmp(const std::vector<CTag> &left,
            const std::vector<CTag> &right);

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

int vIsSubstr(const std::vector<CTag> &vStr1,
              const std::vector<CTag> &vStr2);

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

std::vector<CTag>::iterator pStrStr(std::vector<CTag> &vStr1,
                                               std::vector<CTag> &vStr2,
                                               int offset);


std::vector<CTag>::iterator pStrStr(std::vector<CTag> &vStr1,
                                               std::vector<CTag> &vStr2);

char *GetPageSource(const char *URL);

#endif // __UTILS_H__