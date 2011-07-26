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

    struct CTag
    {
        CTag(short _Val1, char _Val2);
        CTag();
        short tag;
        char isClose;
    };

    template <class _Ty1, class _Ty2> struct CPair
    {
        CPair()
        {
            first = _Ty1();
            second = _Ty2();
        }
        CPair(_Ty1 _Val1, _Ty2 _Val2)
        {
            first = _Val1;
            second = _Val2;
        }
        _Ty1 first;
        _Ty2 second;
    };

    template <class _Ty1, class _Ty2, class _Ty3> struct CTriple
    {
        CTriple()
        {
            first = _Ty1();
            second = _Ty2();
            third = _Ty3();
        }
        CTriple(_Ty1 _Val1, _Ty2 _Val2, _Ty3 _Val3)
        {
            first = _Val1;
            second = _Val2;
            third = _Val3;
        }
        _Ty1 first;
        _Ty2 second;
        _Ty3 third;
    };

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
        bool operator()(pair<string, int> left, pair<string, int> right) const
        {
            if(left.second != right.second)
                return left.second < right.second;
            else
                return left.first.length() < right.first.length();
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
        bool operator()(pair<string, int> left, pair<string, int> right) const
        {
            return strcmp(left.first.c_str(), right.first.c_str()) < 0;
        }
    };

    struct alphacpr
    {
        bool operator()(CPair <CTag, string> left, CPair <CTag, string> right) const
        {
            return strcmp(left.second.c_str(), right.second.c_str()) < 0;
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

    int checksum(const string &src);

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

    int checkWordTruePairs(const string &src);

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

    int getStringFreq(const string &src, const string &str, unsigned int pos);

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

    int getTagSubs(const string &src, int pos);

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

    string getNews(char *srcBegin, const string &newsBegin, const string &newsEnd, unsigned int &offset);

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
    ///< ���������� ������� �������� �����.
    string m_modifiedData;
    ///< ���������� modifiedData. � clearedData ��� ��������� �����.
    string m_clearedData;
    ///< ������� ���� � data
    vector< pair<int, int> > m_realTagPosition;
    ///< ������� ���� � modifiedData.
    vector< pair<int, int> > m_modifiedTagPosition;
    ///< ������� ���� � modifiedData.
    vector< pair<int, int> > m_clearedTagPosition;

    ///< Set �� ��� <������, ������� ������� ������>.
    //   ������������� ������ �� �����������.
    //   �� ������ ����� �� �������.
    set <pair <string, unsigned int>, CNewsFinder::ltstr> m_freq;
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
    vector<pair<string, unsigned int>> possibleTags;
    pair <int, int> modTagPosition;
    string m_newsBegin;
    string m_newsEnd;
    set <string> m_subsArr;
    vector<CPair<CTag, CPair<int, int>>> mod;
    set <CPair <CTag, string>, alphacpr> alphabet;
    unsigned int currFileDataPos;
};
#endif