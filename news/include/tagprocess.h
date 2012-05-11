/**
 * \file    tagprocess.h
 *
 * \brief   ���������� �������, ����������� �� ��������.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include "utils.h"
#include "debug.h"

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <windows.h>

using std::vector;
using std::pair;

/**
 * \class   CNewsFinder
 *
 * \brief   ����� ������ ��������. 
 *
 * \author  Alexander
 * \date    7/21/2011
 */

class CNewsFinder
{
public:

    /**
     * \fn  CNewsFinder::CNewsFinder(char *filename, unsigned int minSize, unsigned int minFreq);
     *
     * \brief   �����������.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  filename    ��� ����� � ���������.
     * \param   minSize             ����������� ����� ������������������ �����, ��������������
     *                              ������� � ������� ������� �������.
     * \param   minFreq             ����������� ������� ������� ������������������ �����.
     */

    CNewsFinder(__in LPCSTR fileName, __in int minSize, __in int minFreq, __in int numberOfNews);
    CNewsFinder(__in LPCSTR fileData, __in int minSize, __in int minFreq);
    /**
     * \fn  CNewsFinder::~CNewsFinder();
     *
     * \brief   ����������.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    ~CNewsFinder();

    /**
     * \fn  void CNewsFinder::init(vector<std::pair<std::string, std::string>> &remDoubleTag,
     *      vector<std::string> &remTag);
     *
     * \brief   �������������.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  remDoubleTag    ������ ������� �����.
     * \param [in,out]  remTag          ������ ��������� �����.
     */

    void Init();
    vector<CNewsBlock> GetRanges();
    void GetPossibleRanges();

    /**
     * \fn  void CNewsFinder::getNewsRange();
     *
     * \brief   ��������� ����� ������/����� �������.
     *
     * \author  Alexander
     * \date    7/20/2011
     */
    void GetNewsRange();

    inline double getAvgDispersion()
    {
        return AvgDispersion;
    }

private:
    /**
     * \struct  pred
     *
     * \brief   �������� ��������� ��� �� V����� � ������ �� �������.
     *
     * \author  Alexander
     * \date    7/13/2011
     */
    
    struct pred
    {
        bool operator()(pair<vector<CTag>, unsigned int> &left,
                        pair<vector<CTag>, unsigned int> &right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.size() < right.first.size();
        }
    };

    struct pred1
    {
        bool operator()(vector<CNews> &left, vector<CNews> &right) const
        {
            return left.size() < right.size();
        }
    };

    /**
     * \struct  ltstr
     *
     * \brief   �������� ��������� V����� � set.
     *
     * \author  Alexander
     * \date    7/19/2011
     */

    struct ltstr
    {
        bool operator()(pair<vector<CTag>, unsigned int> left,
                        pair<vector<CTag>, unsigned int> right) const
        {
            return vStrCmp(left.first, right.first) < 0;
        }
    };

    /**
     * \struct  tagcodecpr
     *
     * \brief   �������� ��������� V�����. 
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    struct tagcodecpr
    {
        bool operator()(const vector<CTag> &left,
                        const vector<CTag> &right) const
        {
                return vStrCmp(left, right) < 0;
        }
    };

    /**
     * \fn  CNewsFinder::CNewsFinder();
     *
     * \brief   ����������� �� ���������.
     * 			�������� �����.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    CNewsFinder();

    /**
     * \fn  unsigned short CNewsFinder::getTagCode(const std::string &tag);
     *
     * \brief   �������� ��� ����.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   tag ���.
     *
     * \return  ��� ����.
     */

    unsigned short CNewsFinder::getTagCode(const std::string &tag);

    /**
     * \fn  CTriple<CTag, CPair<int, int>, std::string> CNewsFinder::getNextTag();
     *
     * \brief   �������� ��������� ���.
     *
     * \author  Alexander
     * \date    7/28/2011
     *
     * \return  CTriple �� ����, ���� <������, �����> ���� � ������ � ��� ���.
     */

    CTag getNextTag();

    /**
     * \fn  std::string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &newsBegin,
     *      vector<CPair<CTag, CPair<int, int>>> &newsEnd, unsigned int &offset);
     *
     * \brief   ���������� ����� �������.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  newsBegin   V������ �����, � ������� ���������� �������.
     * \param [in,out]  newsEnd     V������ �����, ������� ������������� �������.
     * \param [in,out]  offset      ����� � mod.
     *
     * \return  ����� �������.
     */

    std::string CNewsFinder::getNews(vector<CTag> &newsBegin,
                                     vector<CTag> &newsEnd,
                                     unsigned int &unOffset);

private:
    void calcHeur();
    ///< ����������� ������ ������������������ �����.
    unsigned int m_minLen;

    ///< ����������� ������ ������� ������������������ �����.
    unsigned int m_minFreq;

    ///< �������, ���������� � ���� ������� ����.
    std::string m_fileData;

    ///< Set �� ��� <V������, ������� ������� ������>.
    //   ������������� ������ �� �����������.
    //   �� ������ ����� �� �������.
    std::set <pair<vector<CTag>, unsigned int>, ltstr> m_freq;

    ///< ������� ����� ������ � freq.
    unsigned int m_avgLen;

    ///< ������� ������� ������� ������.
    unsigned int m_avgFreq;

    ///< ������ ��������� �����/������ ��������.
    vector<pair<vector<CTag>, unsigned int>> possibleTags;

    ///< V������ ������ �������
    vector<CTag> m_newsBegin;

    ///< V������ ����� �������
    vector<CTag> m_newsEnd;

    // M.A.P. ������� � ������ ������ ��������?
    ///< m_fileData, ������������ � V������ �����
    vector<CTag> m_mod;

    ///< ������� ������� � m_fileData
    unsigned int m_unCurrFileDataPos;
    vector<CNewsBlock> NewsBlocks;
    int m_numberOfNews;
    double AvgDispersion;

    ///< ��� ��������� ������
    // 1  : ������������� ��������.
    // 0  : ��� ���������
    // -1 : ������ ��������� ������.
    long m_lLastError;

    // ����� �������� � html ������
    long m_lVisibleHtmlLen;
};
#endif