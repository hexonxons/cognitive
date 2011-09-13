/**
 * \file    tagprocess.h
 *
 * \brief   ���������� �������, ����������� �� ��������.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include "utils.h"

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <windows.h>

using std::vector;

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

    CNewsFinder(char *filename, unsigned int minSize, unsigned int minFreq);

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

    void init(vector<std::pair<std::string, std::string>> &remDoubleTag, vector<std::string> &remTag);

    /**
     * \fn  void CNewsFinder::getPossibleRanges();
     *
     * \brief   ��������� ���� ��������� ������������������� ����� ������/����� ��������.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void getPossibleRanges();

    /**
     * \fn  void CNewsFinder::getNewsRange();
     *
     * \brief   ��������� ����� ������/����� �������.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void getNewsRange();

    /**
     * \fn  void CNewsFinder::writeNews();
     *
     * \brief   ������ �������� � ����.
     *
     * \author  Alexander
     * \date    7/20/2011
     */

    void writeNews();

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
        bool operator()(CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> left,
                        CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> right) const
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
        int operator()(CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> left,
                       CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int> right) const
        {
            return vStrCmp(left.first, right.first) < 0;
        }
    };

    /**
     * \struct  alphacpr
     *
     * \brief   �������� ��������� �� ������, ��������������� CTag. 
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    struct alphacpr
    {
        int operator()(CPair <CTag, std::string> left, CPair <CTag, std::string> right) const
        {
            return strcmp(left.second.c_str(), right.second.c_str()) < 0;
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
        int operator()(const vector<CPair<CTag, CPair<int, int>>> &left,
                        const vector<CPair<CTag, CPair<int, int>>> &right) const
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
     * \fn  inline void CNewsFinder::LowerCase(string * pstr)
     *
     * \brief   ���������� ������ � �������� ������.
     *
     * \param [in,out]  pstr    - ���������� ������.
     */

    inline void LowerCase(std::string * pstr)
    {
        if(pstr->size())
            CharLower(&(*pstr->begin()));
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

    CTriple<CTag, CPair<int, int>, std::string> getNextTag();

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

    int checksum(const vector<CPair<CTag, CPair<int, int>>> &src);

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

    int checkWordTruePairs(const vector<CPair<CTag, CPair<int, int>>> &src);

    /**
     * \fn  int CNewsFinder::getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &str,
     *      unsigned int pos);
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

    int getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &str,
                      unsigned int pos);

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

    int getTagSubs(const vector<CPair<CTag, CPair<int, int>>> &src, int pos);

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

    std::string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &newsBegin,
                                     vector<CPair<CTag, CPair<int, int>>> &newsEnd,
                                     unsigned int &offset);

    /**
     * \fn  void CNewsFinder::printTable();
     *
     * \brief   ������ �������.
     *
     * \author  Alexander
     * \date    7/28/2011
     */

    void printTable();

private:
    ///< ����������� ������ ������������������ �����.
    unsigned int m_minSz;

    ///< ����������� ������ ������� ������������������ �����.
    unsigned int m_minFreq;

    ///< ������� ����.
    std::fstream m_fileIn;

    ///< �������� ����.
    std::fstream m_fileOut;

    ///< �������, ���������� � ���� ������� ����.
    std::string m_fileData;

    ///< Set �� ��� <V������, ������� ������� ������>.
    //   ������������� ������ �� �����������.
    //   �� ������ ����� �� �������.
    std::set <CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>, ltstr> m_freq;

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
    short **m_pTable;

    ///< ������ ������� - tableSize X tableSize.
    unsigned int m_tableSize;

    ///< ������� ����� ������ � freq.
    unsigned int m_avgLen;

    ///< ������� ������� ������� ������.
    unsigned int m_avgFreq;

    ///< ������ ��������� �����/������ ��������.
    vector<CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>> possibleTags;

    ///< V������ ������ �������
    vector<CPair<CTag, CPair<int, int>>> m_newsBegin;

    ///< V������ ����� �������
    vector<CPair<CTag, CPair<int, int>>> m_newsEnd;

    ///< Set �� ���� ��������
    std::set <vector<CPair<CTag, CPair<int, int>>>, tagcodecpr> m_subsArr;

    ///< m_fileData, ������������ � V������ �����
    vector<CPair<CTag, CPair<int, int>>> m_VtagFileData;

    ///< �������
    std::set <CPair <CTag, std::string>, alphacpr> m_alphabet;

    ///< ������� ������� � m_fileData
    unsigned int currFileDataPos;

};
#endif