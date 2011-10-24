/**
 * \file    tagprocess.h
 *
 * \brief   ���������� �������, ����������� �� ��������.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include "utils.h"
#include "../include/debug.h"

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
     * \fn  CNewsFinder::CNewsFinder(char *filename, UINT minSize, UINT minFreq);
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

    CNewsFinder(LPCSTR lpcszInFileName, UINT unMinSize, UINT unMinFreq);

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
     * \fn  VOID CNewsFinder::init(vector<std::pair<std::string, std::string>> &remDoubleTag,
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

    VOID Init(vector<std::pair<std::string, std::string>> &remDoubleTag, vector<std::string> &remTag);

    /**
     * \fn  VOID CNewsFinder::getPossibleRanges();
     *
     * \brief   ��������� ���� ��������� ������������������� ����� ������/����� ��������.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    VOID GetPossibleRanges();

    /**
     * \fn  VOID CNewsFinder::getNewsRange();
     *
     * \brief   ��������� ����� ������/����� �������.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    VOID GetNewsRange();

    /**
     * \fn  VOID CNewsFinder::writeNews(LPCSTR lpcszOutFileName);
     *
     * \brief   ������ �������� � ����.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    VOID WriteNews(LPCSTR lpcszOutFileName);

    /**
     * \fn  ULONG CNewsFinder::dwGetlastError();
     *
     * \brief   ���������� ��� ��������� ������.
     *
     * \author  Alexander
     * \date    8/31/2011
     */

    long GetlastError();

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
        bool operator()(pair<vector<CTagDescription>, UINT> left,
                        pair<vector<CTagDescription>, UINT> right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.size() < right.first.size();
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
        bool operator()(pair<vector<CTagDescription>, UINT> left,
                        pair<vector<CTagDescription>, UINT> right) const
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
        bool operator()(const vector<CTagDescription> &left,
                        const vector<CTagDescription> &right) const
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
     * \fn  inline VOID CNewsFinder::LowerCase(string *pStr)
     *
     * \brief   ���������� ������ � �������� ������.
     *
     * \param [in,out]  pstr    - ���������� ������.
     */

    inline VOID LowerCase(std::string *pStr)
    {
        if(pStr->size())
            CharLower(&(*pStr->begin()));
    }

    /**
     * \fn  VOID CNewsFinder::removeTags(vector<std::string> &tagsToRemove);
     *
     * \brief   ������� ���� �� V������ mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    ������ ����� ��� ��������.
     */

    VOID removeTags(vector<std::string> &tagsToRemove);

    /**
     * \fn  VOID CNewsFinder::removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);
     *
     * \brief   ������� ������ ���� � ���������� ����� ���� �� V������ mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    ������ ��� ����� ��� ��������.
     */

     VOID removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);

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

    CTagDescription getNextTag();

    /**
     * \fn  int CNewsFinder::checksum(const vector<CPair<CTag, CPair<int, int>>> &src);
     *
     * \brief   ��������� �������� �� ���������� ���������� ����������� ����� ���������� �����������.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src V������ �����.
     *
     * \return  1, ���� ���������� ����������� ����� ������ ���������� ����������� 0 �����.
     */

    int checksum(const vector<CTagDescription> &src);

    /**
     * \fn  int CNewsFinder::checkWordTruePairs(const vector<CPair<CTag, CPair<int, int>>> &src);
     *
     * \brief   ��������� �������� �� ������� ������������ ���� ������� ������������ ����,
     *          ��������������� � ���� V������.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src V������ �����.
     *
     * \return  1, ���� ��� ������� ������������ ���� ���� �����������
     * 			0, �����.
     */

    int checkWordTruePairs(const vector<CTagDescription> &src);

    /**
     * \fn  int CNewsFinder::getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &str,
     *      UINT pos);
     *
     * \brief   �������� ������� ������� str � mod.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   str V������, ������� ������� ������� �� ����.
     * \param   pos ������� � ������� �� ���������, � ������� �������� ���������� str c src.
     *
     * \return  ������� ������� str � mod.
     */

    int getStringFreq(const vector<CTagDescription> &str,
                      UINT unPos);

    /**
     * \fn  int CNewsFinder::getTagSubs(const vector<CPair<CTag, CPair<int, int>>> &src, int pos);
     *
     * \brief   �������� ��� ������������������ �����, ���������� ��� �������.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   src V������ �����, ��� ������� (� ���� � ��������) �� ����� ����� ������� ������� �
     *              ������� �������� ��������� �������/������ �������.
     * \param   pos ������� � ������� �� ���������, � ������� �������� ���������� str c src.
     *
     * \return  1/0.
     */

    int getTagSubs(const vector<CTagDescription> &src, int nPos);

    /**
     * \fn  std::string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &newsBegin,
     *      vector<CPair<CTag, CPair<int, int>>> &newsEnd, UINT &offset);
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

    std::string CNewsFinder::getNews(vector<CTagDescription> &newsBegin,
                                     vector<CTagDescription> &newsEnd,
                                     UINT &unOffset);
#ifdef _DEBUG
    /**
     * \fn  VOID CNewsFinder::printTable();
     *
     * \brief   ������ �������.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    VOID printTable();

    VOID dbgPrintData(VOID);

    std::string getTagWord(vector<CTagDescription> &tagSeq);
#endif

private:
    ///< ����������� ������ ������������������ �����.
    UINT m_unMinSz;

    ///< ����������� ������ ������� ������������������ �����.
    UINT m_unMinFreq;

    ///< �������, ���������� � ���� ������� ����.
    std::string m_fileData;

    ///< Set �� ��� <V������, ������� ������� ������>.
    //   ������������� ������ �� �����������.
    //   �� ������ ����� �� �������.
    std::set <pair<vector<CTagDescription>, UINT>, ltstr> m_freq;

    ///<  ������� ��� ������ ������������� �����.
    // ��������: abcabcac
    //   a b c a b c a c
    // a * 0 0 * 0 0 * 0
    // b 0 * 0 0 * 0 0 0
    // c 0 0 * 0 0 * 0 *
    // a * 0 0 * 0 0 * 0
    // b 0 * 0 0 * 0 0 0
    // c 0 0 * 0 0 * 0 *
    // a * 0 0 * 0 0 * 0
    // c 0 0 * 0 0 * 0 *
    bool **m_pTable;

    ///< ������ ������� - tableSize X tableSize.
    UINT m_unTableSize;

    ///< ������� ����� ������ � freq.
    UINT m_unAvgLen;

    ///< ������� ������� ������� ������.
    UINT m_unAvgFreq;

    ///< ������ ��������� �����/������ ��������.
    vector<pair<vector<CTagDescription>, UINT>> possibleTags;

    ///< V������ ������ �������
    vector<CTagDescription> m_newsBegin;

    ///< V������ ����� �������
    vector<CTagDescription> m_newsEnd;

    ///< Set �� ���� ��������
    std::set <vector<CTagDescription>, tagcodecpr> m_subsArr;

    // M.A.P. ������� � ������ ������ ��������?
    ///< m_fileData, ������������ � V������ �����
    vector<CTagDescription> m_mod;

    ///< ������� ������� � m_fileData
    UINT m_unCurrFileDataPos;

    ///< ��� ��������� ������
    // 1  : ������������� ��������.
    // 0  : ��� ���������
    // -1 : ������ ��������� ������.
    long m_lLastError;

#ifdef _DEBUG
    LPTSTR *plpszTagTable;
#endif
};
#endif