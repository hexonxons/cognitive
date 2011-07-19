#include "tagprocess.h"

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    // ������� ����
	fstream fin("news", ios::in);
    // �������� ����
	fstream fout("out", ios::out);
    // �������, ���������� � ���� ������� ����
	string data((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    // ���������� ������� �������� �����
	string modifiedData;
    // ���������� modifiedData. � clearedData ��� ��������� �����
    string clearedData;
    // ������� ���� � data
	vector< pair<int, int> > realTagPosition;
    // ������� ���� � modifiedData
	vector< pair<int, int> > modifiedTagPosition;
    // ������� ���� � modifiedData
    vector< pair<int, int> > clearedTagPosition;
    // Set �� ��� <������, ������� ������� ������>
    // ������������� ������ �� �����������
    // �� ������ ����� �� �������
    set <pair <string, int>, ltstr> freq;
    set <pair <string, int>, ltstr>::iterator setIter;
    // ������� ��� ������ ������������� �����
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
    short **table;
    // ������ ������� - tableSize X tableSize
    int tableSize = 0;
    // ������� ����� ������ � freq
    int avgLen = 0;
    // ������� ������� ������� ������
    int avgFreq = 0;
    // ������ ��������� �����/������ ��������
    vector<pair<string, int>> possibleTags;
    vector<pair<string, int>>::iterator vectorIter;
	int i = 0;
	int j = 0;
	pair <int, int> tagPosition = make_pair(-1, -1);
	pair <int, int> modTagPosition = make_pair(-1, -1);

	// �������� �� ����� �����
	for(i = 0; i < data.size(); ++i)
	{
		// ���� (��������) ����������� ���
		if (data[i] == '<')
		{
			// � �������, ���� < � js
			if (tagPosition.first != -1)
				tagPosition.first = i;
			// �����, � ����� ������� ����� ���
			tagPosition.first = i;
			modTagPosition.first = j;
			// ���� �� ������ �� �������/����������� ������ - ����������
			while (data[i] != ' ' && data[i] != '>')
			{
				modifiedData += data[i];
				++j;
				++i;				
			}
		}
		// ���� ����������� ������ - ����������
		if (data[i] == '>')
		{
			modifiedData += data[i];
			++j;
			tagPosition.second = i;
			modTagPosition.second = j;
			realTagPosition.push_back(tagPosition);
			modifiedTagPosition.push_back(modTagPosition);
			tagPosition.first = -1;
			tagPosition.second = -1;
		}
	}

    // ������ �� �������� �����
	vector<pair<string, string>> remDoubleTag;
    vector<string> remTag;

    remDoubleTag.push_back(make_pair("<script>", "</script>"));
    remDoubleTag.push_back(make_pair("<noscript>", "</noscript>"));
    remDoubleTag.push_back(make_pair("<form>", "</form>"));
    remDoubleTag.push_back(make_pair("<iframe>", "</iframe>"));
    remDoubleTag.push_back(make_pair("<ul>", "</ul>"));
    remDoubleTag.push_back(make_pair("<span>", "</span>"));
    remDoubleTag.push_back(make_pair("<p>", "</p>"));

    remTag.push_back("<!doctype>");
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
    remTag.push_back("<p>");
    remTag.push_back("</p>");

    removeTags(&modifiedTagPosition, modifiedData, remTag);
    removeTags(&modifiedTagPosition, modifiedData, remDoubleTag);
   
    // ���������� clearedData
    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (modifiedTagPosition[i].first != -1)
        {
            string tag = getTag(modifiedTagPosition[i], modifiedData);
            int ch = 0;
            if (tag[1] != '/')
                // <tag[1]>           -- tag[1] + 47
                // <tag[1]tag[2]..>   -- tag[1] + tag[2]
                // VV Overflow 256    -> "ch > 0"
                ch = tag[1] + tag[2] + 128;
            else
                // </tag[2]>          -- tag[2] + 47
                // </tag[2][tag[3]..> -- tag[2] + tag[3]
                // VV Overflow 128    -> "ch < 0"
                ch = 128 + (tag[2] + tag[3]) % 128; 
            clearedData += ch;
            clearedTagPosition.push_back(modifiedTagPosition[i]);
        }
    }

    // ������ ������ �������
    tableSize = clearedData.size();
    // �������� ������ �� �������
    table = new short *[tableSize];
    for (i = 0; i < tableSize; ++i)
        table[i] = new short[tableSize];

    // ��������� �������
    for (i = 0; i < tableSize; ++i)
    {
        for (j = 0; j < tableSize; ++j)
        {
            if (clearedData[i] == clearedData[j])
            {
                table[i][j] = 1;
            }
            else
                table[i][j] = 0;
        }
    }

    // ������ � table �� ����������
    // diag - ����� ���������
    // diag = 0 - ������� ���������, ��������� 
    int diag = 1;
    while(diag != tableSize)
    {
        for (i = 0; i < tableSize - diag; ++i)
        {
            if (table[i][i + diag] != 0)
            {
                // ��������� �������
                string str;
                int beg = i;
                while (i < tableSize - diag && table[i][i + diag] != 0)
                {
                    str += clearedData[i];
                    ++i;
                }
                // � ��������� ����������� ���� �������(��� � ��������) ���� ������� ��� ������ �������
                getTagSubs(freq, str, clearedData, table, tableSize, beg + diag, avgLen, avgFreq);
            }
        }
        // ������ ��������� ���������
        ++diag;
    }
    // ��������� ������� �����/������� �����
    avgLen = avgLen / freq.size();
    avgFreq = avgFreq / freq.size();

    for(setIter = freq.begin(); setIter != freq.end(); ++setIter)
    {
        if (setIter->second >= 8 && setIter->first.size() < avgLen * 1.5 && setIter->second < avgFreq * 1.6)
            possibleTags.push_back(*setIter);
    }
    // ��������� ����
    sort(possibleTags.begin(), possibleTags.end(), pred());


    // ���� �� �������� � clearedTagPosition ������� � realTagPosition
    // ���������� ������ ����� � ������ �������� ������ ���� ���������
    vectorIter = possibleTags.end();
    --vectorIter;
    while (vectorIter->second != (vectorIter - 1)->second)
    {
        --vectorIter;
    }
    string newsBegin;
    string newsEnd;

    // ���� �������, � ������� ���������� �������
    int begin = strstr(clearedData.c_str(), vectorIter->first.c_str()) - clearedData.c_str();
    newsBegin = vectorIter->first;
    while (!strstr(vectorIter->first.c_str(), (vectorIter - 1)->first.c_str()) == NULL)
    {
        --vectorIter;
    }
    // �������, �� ������� ������� �������������
    int end = strstr(clearedData.c_str(), (vectorIter - 1)->first.c_str()) - clearedData.c_str();
    newsEnd = (vectorIter - 1)->first;
    // ���� � possibleTags �� ���������� ������ � �����
    if (begin > end)
    {
        int t = begin;
        string ts = newsBegin;
        begin = end;
        newsBegin = newsEnd;
        end  = t;
        newsEnd = ts;
    }
    // ����� ����, � �������� ������� ���������� � �������������, � modifiedTagPosition
    int rbegin;
    int rend;
    
    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (clearedTagPosition[begin].first == modifiedTagPosition[i].first)
            rbegin = i;
        if (clearedTagPosition[end].second == modifiedTagPosition[i].second)
            rend = i;
    }
    char *strBegin = (char *)clearedData.c_str();
    int offset = 0;
    // �������� ������� ������� � ������� �
    while (strlen(strBegin) > offset)
    {
        string res = getNews(data, strBegin, newsBegin, newsEnd, clearedTagPosition,
            modifiedTagPosition, realTagPosition, offset);
        if (res.empty())
            break;
        fout << res;
    }

	return 0;
}