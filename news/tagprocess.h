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

using std::pair;
using std::string;
using std::vector;
using std::stack;

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
 * \fn  string getTag(int tagNum, const string &src);
 *
 * \brief   ��������� ���� �� ��� ����������� ������
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   tagNum  ���������� ����� ����.
 * \param   src     ������� � ������.
 *
 * \return  ���.
 */
string getTag(int tagNum, const string &src);

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
 * \fn  int getWordFreq(const string &src, const string &word);
 *
 * \brief   ��������� ���������� ������ word � src. 
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     ������ ����.
 * \param   word    �����.
 *
 * \return  ������� ������� word � src.
 */
int getWordFreq(const string &src, const string &word);

/**
 * \fn  string getWordString(const string &src, int begin, int end);
 *
 * \brief   �������� ������ �� ����, � begin`�� �� end`�� �����
 * 			����� - <*>. ��������� ���� - 1..n
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     ������.
 * \param   begin   ������� � src, � ������� ��������� �����.
 * \param   end     ������� � src, �� ������� ��������� �����.
 *
 * \return  ������� �� ���� c � begin`�� �� end`��, ��� �� ����������, ����
 * 			end > ���������� ����.
 * 			NULL, ���� begin > ���������� ����.
 */
string getWordString(const string &src, int begin, int end);

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
int checksum(const string &src, int flag);

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
int checkWordTruePairs(const string &src, int flag);
/**
 * \fn  int wordSubPtr(const string &src, const string &word);
 *
 * \brief   ���������� ���������� ����� � ������, �������������� ������
 * 			������� ������������������ word
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     ������.
 * \param   word    ������������������ �����-����, ��������� src.
 *
 * \return  ���������� �����.
 */
int wordSubPtr(const string &src, const string &word);

/**
 * \fn  int wordSubPtr(const string &src, const string &word, int offset);
 *
 * \brief   ���������� ���������� ����� � ������ �� ������� �� offset 
 * 			���������, �������������� ������ ������� ������������������
 * 			word
 *
 * \author  Alexander
 * \date    7/13/2011
 *
 * \param   src     ������.
 * \param   word    ������������������ �����-����, ��������� src.
 * \param   offset  ����� � src.
 *
 * \return  ���������� �����.
 */
int wordSubPtr(const string &src, const string &word, int offset);

#endif