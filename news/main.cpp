#include "tagprocess.h"

#include <fstream>

using namespace std;

/**
 * \fn  int main()
 *
 * \brief   Main entry-point for this application.
 * \algo
 * 			�������� ���������:
 * 			- ���� ����������� � ������� data. N ��������.\n\n
 * 			
 * 			- �������� �� ���� data, ���� '<' � '>', �������� �� ������� � 
 * 			������ ������ ������� � tagPosition.\n
 * 			� ���� �� ������ ���������� � modifiedData ���� <*>, 
 * 			�.� ���������� ��� ����������, ����� �������� ����,\n
 * 			� ���������� � modTagPosition ������� ���� � modifiedData.\n
 * 			<b>O(N)</b>\n\n
 * 			
 * 			- �� modifiedTagPosition ������� ��� �������� ����, 
 * 			�.� ���������� �� ����� ������� ����� (-1, -1). \n
 * 			������ �� ���� �����        * ������� �� ����� ������ ��� �������� * tolower ������ ���  * ��������� �����  * �������� ����� �� ��������� ��� �������� remDoubleTag.\n
 * 			<b>O(modifiedTagPosition.size) * O(remTag.size)                       * O(each tag size)    * O(each tag size) * Const</b>\n\n
 * 			
 * 			- �������� �� modifiedTagPosition � ���������� � clearedData ��� ��������� ����. 
 * 			����������� ���� ����� ����� ��� > 0, ����������� < 0.\n
 * 			� clearedTagPosition ���������� ������ ���� ����� � modifiedTagPosition.\n
 * 			<b>O(modifiedTagPosition.size)</b>\n\n
 * 			
 * 			- ��������� ������� [clearedData.size x clearedData.size].\n
 * 			<b>O(clearedData.size ^ 2)</b>\n\n
 * 			
 * 			- ��������� ������� �� ���������, ������� � ����� ������� � ������� ���������.\n
 * 		    <b>O(clearedData.size ^ 2 / 2)</b>\n\n
 * 		    
 * 		    - ��� ������ ��������� ������ >= MINSZ ��������� 2 ��������,\n
 * 		    <b>O(2 * substring.size)</b>\n
 * 		    ������ ������� ������� ���������,\n
 * 		    <b>O(clearedData.size * substring.size)</b>\n
 * 		    ������ � � set, ��������� ����� ������ � ������� ������� � avgLen � avgFreq.\n
 * 		    <b>O(log(freq.size))</b>\n
 * 		    ��������� ��� ��� ����������, ��� 2� �������� ����� length - 1.\n
 * 		    ���� ������� ������� ��������� < MINSZ - ���������� 0.\n
 * 		    ���� ��������� ������� ��� 2� �������� == 0 - ���������� 0.\n
 * 		    ����� ��������� ��� ��� �������� ������.\n
 * 		    
 * 		    - ������������ avgLen � avgFreq, ���������� �� set`� � vector ���,\n
 * 		    � ���� ������� ������� < avgFreq * 1.6 � ����� < avgLen * 1.5.\n
 * 		    ��������� ���� ������.\n
 * 		    <b>O(possibleTags.size * log(possibleTags.size))</b>\n\n
 * 		    
 * 		    - ���� � ����� �������, ���� 2 �������� � ���������� ����������� ������ - ����� � ������ �������.\n
 * 		    �������� ����� ����� �� clearedData.  ������ ��� getNews.\n
 * 		    <b>[O(2 * clearedData.size) + O(clearedTagPosition.size)] * possibleTags.size </b>\n\n
 * 		    
 * 		    
 * 		    
 * 			
 *
 * \author  Alexander
 * \date    7/20/2011
 *
 * \return  Exit-code for the process - 0 for success, else an error code.
 */

int main()
{
    CNewsFinder finder("news", 8);
    finder.init();
    finder.getPossibleRanges();
    finder.getNewsRange();
    finder.writeNews();

	return 0;
}