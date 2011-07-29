#include "tagprocess.h"

#include <fstream>

using namespace std;

/**
 * \fn  int main()
 *
 * \brief   Main entry-point for this application.
 * \algo
 * 			�������� ���������:
 * 			- ���� ����������� � ������� m_fileData. N ��������.\n\n
 * 			
 * 			- m_fileData ����������� � ������ �������.\n
 * 			<b>O(N)</b>\n\n
 * 			
 * 			- ���������� �� ���� < head >\n\n
 * 			
 * 			- ��������� �� ���� < /head >. ������ ������ ���������� ��� � m_VtagFileData � � m_alphabet. ����� ����� - M.\n
 * 			<b>O(MlogM) + O(N)\n\n
 * 			 
 * 			- ������� �������� ���� � �������� ���� <����������� ���, ����������� ���> �����, �� ����, ��� ����� ����.\n\n
 * 			<b>O(M * NumOfTagsToDel)</b>\n\n
 * 			
 * 			- ���������� �������.\n
 * 			<b>O(M ^ 2)</b>\n\n
 * 			
 * 			- �������� �� ������� � ���������� ������ ��� < ������������������ �����, ������� ������� >.\n
 * 			<b>O[(M - m_minSz - 1) * (M - m_minSz) * (M + m_minSz - 1)]</b>\n\n
 * 			
 * 			- ��������� ��� ������������������ �����, ������� ������ �������� � ������.\n\n
 * 			
 * 			- ��������� �� ������� �������. ���������� ������������������, ������� ������ ����������� �� ������� �������� ������/����.\n\n
 * 			
 * 			- �������� ������������������ ������/����� �������.\n\n
 *
 * \author  Alexander
 * \date    7/20/2011
 *
 * \return  Exit-code for the process - 0 for success, else an error code.
 */


int main()
{
    // ������� �������� �����
    vector<pair<string, string>> remDoubleTag;
    vector<string> remTag;
    CNewsFinder finder("news", 8, 10);
    remDoubleTag.push_back(make_pair("<script>", "</script>"));
    remDoubleTag.push_back(make_pair("<noscript>", "</noscript>"));
    remDoubleTag.push_back(make_pair("<form>", "</form>"));
    remDoubleTag.push_back(make_pair("<iframe>", "</iframe>"));
    //remDoubleTag.push_back(make_pair("<ul>", "</ul>"));
    //remDoubleTag.push_back(make_pair("<span>", "</span>"));
    remDoubleTag.push_back(make_pair("<noindex> ", "</noindex> "));
    remDoubleTag.push_back(make_pair("<style>", "</style>"));

    
    remTag.push_back("<html>");
    remTag.push_back("</html>");
    remTag.push_back("<head>");
    remTag.push_back("</head>");
    remTag.push_back("<body>");
    remTag.push_back("</body>");
    remTag.push_back("<link>");
    remTag.push_back("<title>");
    remTag.push_back("</title>");
    remTag.push_back("<meta>");
    remTag.push_back("</meta>");
    remTag.push_back("<base>");
    remTag.push_back("<!-->");
    remTag.push_back("<li>");
    remTag.push_back("</li>");
    //remTag.push_back("<p>");
    //remTag.push_back("</p>");
    //remTag.push_back("<ul>");
    //remTag.push_back("</ul>");
    //remTag.push_back("<span>");
    remTag.push_back("<!-->");

    finder.init(remDoubleTag, remTag);
    finder.getPossibleRanges();
    finder.getNewsRange();
    finder.writeNews();

	return 0;
}