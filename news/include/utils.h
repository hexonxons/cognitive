/**
 * \file    utils.h
 *
 * \brief   ��������������� ������� � ��������� ������. ����������.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>

/**
 * \struct  CTagDescription
 *
 * \brief   ��������� ��������������� ����
 *
 * \author  Alexander
 * \date    11/17/2011
 */
struct CTagDescription
{
    // ��� ���
    std::string tag;
    // ������ ���� � ������
    int nTagBegin;
    // ����� ���� � ������
    int nTagEnd;
    // ��� ����
    int nTagCode;
    // ����������� ��� ��� ���
    bool bIsClose;
 
    CTagDescription();
    void Clear();
};

struct CTagRange
{
    int CloseBegin;
    int CloseEnd;
    // ������ ������������������ ����� � ������
    int Begin;
    // ����� ������������������
    int End;

    // ��� ������ ����� begin � end
    std::string TagString;
    // ��������� ���� ����� ������� ������������������ �� ���� ����� html ���������
    double PercToHtml;
    // ��������� ������� ����� ������������������ � ������� ����� ���� html ���������
    double PercToVisibleHtml;

    long InnerIntersection;
    long DistanceToNextRange;

    
    std::string CloseTagString;
    double ClosePercToHtml;
    double ClosePercToVisibleHtml;

    CTagRange();
};

struct CTagSequence 
{
    std::vector<CTagDescription> tag;
    std::vector<CTagDescription> closedTag;
    std::vector<CTagRange> tagRange;
    // ����� ����������� ���� ����������%�������� �� ����� �������
    int innerIntersect;
    double dispersion;
    // ��������� ����� ���� ���� ������������������� �� ���� ����� html ���������
    double percToHtml;
    // ��������� ������� ����� ������������������ � ������� ����� ���� html ���������
    double percToVisibleHtml;
    // ����� ���������� ����� �������
    int InnerDistance;
    // ����� ������ ����� ����������� ��� ������ ������ � ������� ���������� ��� ���� ������������������
    double InnerDistanceDiffSum;
    // 
    int InternalIntersection;
    // ��������� ����� ���� ���� ������������������� �� ���� ����� html ���������
    double ClosePercToHtml;
    // ��������� ������� ����� ������������������ � ������� ����� ���� html ���������
    double ClosePercToVisibleHtml;
    int CloseLen;
    CTagSequence();
};

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

char *GetPageSource(const char *URL);

#endif // __UTILS_H__