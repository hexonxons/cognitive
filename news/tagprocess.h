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

using std::pair;
using std::string;
using std::vector;
using std::stack;
using std::set;

// magick...
#define MINSZ 6

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

/**
 * \fn  string getTag(pair<int, int> &tagPos, const string &src);
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
string getTag(pair<int, int> &tagPos, const string &src);

/**
 * \fn  void removeTags(vector< pair<int, int> > *tagsPos, 
 * 		const string &src, vector<string> &tagsToRemove);
 *
 * \brief   ������� ���� �� ������ src.
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param [in,out]  tagsPos         ������ ��� ������-����� �����.
 * \param   src                     ������ � ������.
 * \param [in,out]  tagsToRemove    ������ ����� ��� ��������.
 */
void removeTags(vector< pair<int, int> > *tagsPos, const string &src, 
                vector<string> &tagsToRemove);

/**
 * \fn  void removeTags(vector< pair<int, int> > *tagsPos,
 * 		const string &src, vector< pair<string, string> > &tagsToRemove);
 *
 * \brief   ������� ������ ���� � ���������� ����� ���� �� ������ src.
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param [in,out]  tagsPos         ������ ��� ������-����� �����.
 * \param   src                     ������ � ������.
 * \param [in,out]  tagsToRemove    ������ ��� ����� ��� ��������.
 */
void removeTags(vector< pair<int, int> > *tagsPos, const string &src, 
                vector< pair<string, string> > &tagsToRemove);


/**
 * \fn  int getWordCount(const string &src);
 *
 * \brief   �������� ���������� ���� � ������.
 * 			����� - <*>.
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
 * \fn  int checksum(const string &src);
 *
 * \brief   ��������� �������� �� ���������� ���������� ����������� �����
 * 			���������� �����������.
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src ������ �����.
 *
 * \return  1, ���� ���������� ����������� ����� ������ ����������
 * 			�����������
 * 			0 �����
 */
int checksum(const string &src);

/**
 * \fn  int checkWordTruePairs(const string &src);
 *
 * \brief   ��������� �������� �� ������� ������������ ���� �������
 * 			������������ ����, ��������������� � ���� ������.
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src ������ �����.
 *
 * \return  1, ���� ��� ������� ������������ ���� ���� �����������
 * 			0, �����
 */
int checkWordTruePairs(const string &src);

/**
 * \fn  int getStringFreq(const string &src, const string &str, short **table,
 *      unsigned int tableSz, int pos);
 *
 * \brief   �������� ������� ������� str � src.
 *
 * \author  Alexander
 * \date    7/19/2011
 *
 * \param   src             �������� ������.
 * \param   str             ������, ������� ������� ������� �� ����.
 * \param [in,out]  table   ������� �� ������� ���� ����������.
 * \param   tableSz         ������ �������.
 * \param   pos             ������� � ������� �� ���������, � ������� �������� ���������� str c
 *                          src.
 *
 * \return  ������� ������� str � src.
 */

int getStringFreq(const string &src, const string &str, short **table, 
                  unsigned int tableSz, int pos);

/**
 * \fn  int getTagSubs(set <pair <string, int>, ltstr> &freq, const string &src,
 *      const string &dataString, short **table, unsigned int tableSz, int pos, int &avgLen,
 *      int &avgFreq);
 *
 * \brief   �������� ��� ������������������ �����, ���������� ��� �������.
 *
 * \author  Alexander
 * \date    7/19/2011
 *
 * \param [in,out]  freq        Set ��� ������ - ������� ������� ������.
 * \param   src                 ������ �����, ��� ������� (� ���� � ��������) �� ����� �����
 *                              ������� ������� � ������� �������� ��������� �������/������
 *                              �������.
 * \param   dataString          �������� ������.
 * \param [in,out]  table       ������� �� ������� ���� ����������.
 * \param   tableSz             ������ �������.
 * \param   pos                 ������� � ������� �� ���������, � ������� �������� ���������� str
 *                              c src.
 * \param [in,out]  avgLen      ������� ����� ������������������.
 * \param [in,out]  avgFreq     ������� ������� ������������������.
 *
 * \return  1/0.
 */

int getTagSubs(set <pair <string, int>, ltstr> &freq, const string &src, const string &dataString,
               short **table, unsigned int tableSz, int pos, int &avgLen, int &avgFreq);

/**
 * \fn  string getNews(const string &data, char *srcBegin, const string &newsBegin,
 *      const string &newsEnd, vector< pair<int, int> > &clearedTagPosition,
 *      vector< pair<int, int> > &modifiedTagPosition, vector< pair<int, int> > &realTagPosition,
 *      int &offset);
 *
 * \brief   ���������� ����� �������.
 *
 * \author  Alexander
 * \date    7/20/2011
 *
 * \param   data                        ������, ���������� ���� ����.
 * \param [in,out]  srcBegin            ������ ������ �����
 * \param   newsBegin                   ������ �����, � ������� ���������� �������.
 * \param   newsEnd                     ������ �����, ������� ������������� �������.
 * \param [in,out]  clearedTagPosition  ������� ����� � clearedData.
 * \param [in,out]  modifiedTagPosition ������� ����� � modifiedData.
 * \param [in,out]  realTagPosition     ������� ����� � data/
 * \param [in,out]  offset              ����� � srcBegin.
 *
 * \return  ����� �������.
 */

string getNews(const string &data, char *srcBegin, const string &newsBegin, const string &newsEnd,
               vector< pair<int, int> > &clearedTagPosition, vector< pair<int, int> > &modifiedTagPosition,
               vector< pair<int, int> > &realTagPosition, int &offset);
#endif