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

    void Init(vector<std::pair<std::string, std::string>> &remDoubleTag, vector<std::string> &remTag);
    vector<CTagSequence> GetRanges();
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

    /**
     * \fn  void CNewsFinder::writeNews(LPCSTR lpcszOutFileName);
     *
     * \brief   ������ �������� � ����.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void WriteNews(LPCSTR lpcszOutFileName);

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
        bool operator()(pair<vector<CTagDescription>, unsigned int> left,
                        pair<vector<CTagDescription>, unsigned int> right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.size() < right.first.size();
        }
    };

    struct pred1
    {
        bool operator()(vector<pair<int, int>> left,
            vector<pair<int, int>> right) const
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
        bool operator()(pair<vector<CTagDescription>, unsigned int> left,
                        pair<vector<CTagDescription>, unsigned int> right) const
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
     * \fn  inline void CNewsFinder::LowerCase(string *pStr)
     *
     * \brief   ���������� ������ � �������� ������.
     *
     * \param [in,out]  pstr    - ���������� ������.
     */

    inline void LowerCase(std::string *pStr)
    {
        if(pStr->size())
            CharLower(&(*pStr->begin()));
    }

    /**
     * \fn  void CNewsFinder::removeTags(vector<std::string> &tagsToRemove);
     *
     * \brief   ������� ���� �� V������ mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    ������ ����� ��� ��������.
     */

    void removeTags(vector<std::string> &tagsToRemove);

    /**
     * \fn  void CNewsFinder::removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);
     *
     * \brief   ������� ������ ���� � ���������� ����� ���� �� V������ mod.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    ������ ��� ����� ��� ��������.
     */

     void removeTags(vector< std::pair<std::string, std::string> > &tagsToRemove);

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

    int checkTag(const vector<CTagDescription> &src);

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

    std::string CNewsFinder::getNews(vector<CTagDescription> &newsBegin,
                                     vector<CTagDescription> &newsEnd,
                                     unsigned int &unOffset);
#ifdef _DEBUG

    void dbgPrintData(void);

    std::string getTagWord(vector<CTagDescription> &tagSeq);
#endif

private:
    ///< ����������� ������ ������������������ �����.
    unsigned int m_minLen;

    ///< ����������� ������ ������� ������������������ �����.
    unsigned int m_minFreq;

    ///< �������, ���������� � ���� ������� ����.
    std::string m_fileData;

    ///< Set �� ��� <V������, ������� ������� ������>.
    //   ������������� ������ �� �����������.
    //   �� ������ ����� �� �������.
    std::set <pair<vector<CTagDescription>, unsigned int>, ltstr> m_freq;

    ///< ������� ����� ������ � freq.
    unsigned int m_avgLen;

    ///< ������� ������� ������� ������.
    unsigned int m_avgFreq;

    ///< ������ ��������� �����/������ ��������.
    vector<pair<vector<CTagDescription>, unsigned int>> possibleTags;

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
    unsigned int m_unCurrFileDataPos;
    vector<CTagSequence> tags;
    int m_numberOfNews;

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