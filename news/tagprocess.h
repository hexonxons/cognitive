/**
 * \file    tagprocess.h
 *
 * \brief   ���������� �������, ����������� �� ��������.
 */

#ifndef __TAGPROCESS_H__
#define __TAGPROCESS_H__

#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <set>
#include <fstream>
#include <windows.h>
#include "utils.h"

using std::pair;
using std::string;
using std::vector;
using std::stack;
using std::set;
using std::fstream;

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
     * \fn  void CNewsFinder::removeTags(vector<string> &tagsToRemove);
     *
     * \brief   ������� ���� �� ������ src.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    ������ ����� ��� ��������.
     */

    void removeTags(vector<string> &tagsToRemove);

    /**
     * \fn  void CNewsFinder::removeTags(vector< pair<string, string> > &tagsToRemove);
     *
     * \brief   ������� ������ ���� � ���������� ����� ���� �� ������ src.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagsToRemove    ������ ��� ����� ��� ��������.
     */

    void removeTags(vector< pair<string, string> > &tagsToRemove);

    /**
     * \fn  void CNewsFinder::init(vector<pair<string, string>> &remDoubleTag,
     *      vector<string> &remTag);
     *
     * \brief   �������������.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  remDoubleTag    ������ ������� �����.
     * \param [in,out]  remTag          ������ ��������� �����.
     */

    void init(vector<pair<string, string>> &remDoubleTag, vector<string> &remTag);

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
     * \brief   �������� ��������� ��� �� ����� � ������ �� �������.
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
     * \brief   �������� ��������� � set.
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


    struct alphacpr
    {
        int operator()(CPair <CTag, string> left, CPair <CTag, string> right) const
        {
            return strcmp(left.second.c_str(), right.second.c_str()) < 0;
        }
    };

    struct tagcodecpr
    {
        int operator()(const vector<CPair<CTag, CPair<int, int>>> &left,
                        const vector<CPair<CTag, CPair<int, int>>> &right) const
        {
                return vStrCmp(left, right) < 0;
        }
    };


    /**
     * \fn  inline void CNewsFinder::LowerCase(string * pstr)
     *
     * \brief   ���������� ������ � �������� ������.
     *
     * \param [in,out]  pstr    - ���������� ������.
     */

    inline void LowerCase(string * pstr)
    {
        if(pstr->size())
            CharLower(&(*pstr->begin()));
    }

    /**
     * \fn  string CNewsFinder::getTag(pair<int, int> &tagPos, const string &src);
     *
     * \brief   �������� ��� �� ��� ������� ������-����� � �������.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param [in,out]  tagPos  ������� ������-�����.
     * \param   src             ������� � ������.
     *
     * \return  ���.
     */

    unsigned short CNewsFinder::getTagCode(const string &tag);

    CTriple<CTag, CPair<int, int>, string> getNextTag();
    /**
     * \fn  int CNewsFinder::checksum(const string &src);
     *
     * \brief   ��������� �������� �� ���������� ���������� ����������� ����� ���������� �����������.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src ������ �����.
     *
     * \return  1, ���� ���������� ����������� ����� ������ ���������� ����������� 0 �����.
     */

    int checksum(const vector<CPair<CTag, CPair<int, int>>> &src);

    /**
     * \fn  int CNewsFinder::checkWordTruePairs(const string &src);
     *
     * \brief   ��������� �������� �� ������� ������������ ���� ������� ������������ ����,
     *          ��������������� � ���� ������.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src ������ �����.
     *
     * \return  1, ���� ��� ������� ������������ ���� ���� ����������� 0, �����.
     */

    int checkWordTruePairs(const vector<CPair<CTag, CPair<int, int>>> &src);

    /**
     * \fn  int CNewsFinder::getWordCount(const string &src);
     *
     * \brief   �������� ���������� ���� � ������. ����� - &lt;*&gt;.
     *
     * \author  Alexander
     * \date    7/13/2011
     *
     * \param   src ������.
     *
     * \return  ���������� ����.
     */

    int getWordCount(const string &src);

    /**
     * \fn  int CNewsFinder::getStringFreq(const string &src, const string &str, unsigned int pos);
     *
     * \brief   �������� ������� ������� str � src.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   src �������� ������.
     * \param   str ������, ������� ������� ������� �� ����.
     * \param   pos ������� � ������� �� ���������, � ������� �������� ���������� str c src.
     *
     * \return  ������� ������� str � src.
     */

    int getStringFreq(const vector<CPair<CTag, CPair<int, int>>> &src,
                      const vector<CPair<CTag, CPair<int, int>>> &str,
                      unsigned int pos);

    /**
     * \fn  int CNewsFinder::getTagSubs(const string &src, int pos);
     *
     * \brief   �������� ��� ������������������ �����, ���������� ��� �������.
     *
     * \author  Alexander
     * \date    7/19/2011
     *
     * \param   src ������ �����, ��� ������� (� ���� � ��������) �� ����� ����� ������� ������� �
     *              ������� �������� ��������� �������/������ �������.
     * \param   pos ������� � ������� �� ���������, � ������� �������� ���������� str c src.
     *
     * \return  1/0.
     */

    int getTagSubs(const vector<CPair<CTag, CPair<int, int>>> &src, int pos);

    /**
     * \fn  string CNewsFinder::getNews(char *srcBegin, const string &newsBegin,
     *      const string &newsEnd, unsigned int &offset);
     *
     * \brief   ���������� ����� �������.
     *
     * \author  Alexander
     * \date    7/20/2011
     *
     * \param [in,out]  srcBegin    ������ ������ �����.
     * \param   newsBegin           ������ �����, � ������� ���������� �������.
     * \param   newsEnd             ������ �����, ������� ������������� �������.
     * \param [in,out]  offset      ����� � srcBegin.
     *
     * \return  ����� �������.
     */

    string CNewsFinder::getNews(vector<CPair<CTag, CPair<int, int>>> &srcBegin,
                                vector<CPair<CTag, CPair<int, int>>> &newsBegin,
                                vector<CPair<CTag, CPair<int, int>>> &newsEnd,
                                unsigned int &offset);

    void printTable();

    ///< ����������� ������ ������������������ �����.
    unsigned int m_minSz;
    ///< ����������� ������ ������� ������������������ �����.
    unsigned int m_minFreq;
    ///< ������� ����.
    fstream m_fileIn;
    ///< �������� ����.
    fstream m_fileOut;
    ///< �������, ���������� � ���� ������� ����.
    string m_fileData;

    ///< Set �� ��� <������, ������� ������� ������>.
    //   ������������� ������ �� �����������.
    //   �� ������ ����� �� �������.
    set <CPair<vector<CPair<CTag, CPair<int, int>>>, unsigned int>, ltstr> m_freq;
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
    vector<CPair<CTag, CPair<int, int>>> m_newsBegin;
    vector<CPair<CTag, CPair<int, int>>> m_newsEnd;
    set <vector<CPair<CTag, CPair<int, int>>>, tagcodecpr> m_subsArr;
    vector<CPair<CTag, CPair<int, int>>> mod;
    set <CPair <CTag, string>, alphacpr> alphabet;
    unsigned int currFileDataPos;
};
#endif