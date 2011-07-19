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
			// ;TODO1 �������... < a >? 
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

    // ���������� set �� ��� (������, ������� ������� ���� ������)
    // set - ������������� �������� ���������
    set <pair <string, int>, ltstr> freq;
    set <pair <string, int>, ltstr>::iterator setIter;

    //////////////////////////////////////////////////////////////////////////
    int sz = clearedData.size();
    short **table;
    table = new short *[sz];
    for (i = 0; i < sz; ++i)
        table[i] = new short[sz];
    for (i = 0; i < sz; ++i)
    {
        for (j = 0; j < sz; ++j)
        {
            if (clearedData[i] == clearedData[j])
            {
                table[i][j] = 1;
            }
            else
                table[i][j] = 0;
        }
    }

    int avgLen = 0;
    int avgFreq = 0;
    // �������� �������
    int diag = 1;
    while(diag != sz)
    {
        for (i = 0; i < sz - diag; ++i)
        {
            if (table[i][i + diag] != 0)
            {
                int temp = i;
                string str;
                int beg = i;
                while (i < sz - diag && table[i][i + diag] != 0)
                {
                    str += clearedData[i];
                    ++i;
                }
                getTagSubs(freq, str, clearedData, table, sz, temp + diag, avgLen, avgFreq);
            }
        }
        ++diag;
    }
    
    //////////////////////////////////////////////////////////////////////////
    avgLen = avgLen / freq.size();
    avgFreq = avgFreq / freq.size();

    set<pair<string, int>, ltstr>::iterator it;
    vector<pair<string, int>> temp;
    int sum = 0;
    // ������������ �� set � ������
    // ;TODO5 ��. ;TODO3
    for(it = freq.begin(); it != freq.end(); it++ )
    {
        if (it->second >= 8 && it->first.size() < avgLen * 1.5 && it->second < avgFreq * 1.5)
        {
            temp.push_back(*it);
            sum += it->second;
        }
    }
    // ��������� ����
    sort(temp.begin(), temp.end(), pred());


    // ���� �� �������� � clearedTagPosition ������� � realTagPosition
    // ��������� ������� ����� ������������� � ������ ���������� �����.
    // ������������ � BestString, ��� ��� ������� ������� ������� ������
    // ;TODO6 �� ;TODO2
    vector<pair<string, int>>::iterator vit = temp.end();
    --vit;
    while (vit->second != (vit - 1)->second)
    {
        --vit;
    }
    int begin = strstr(clearedData.c_str(), vit->first.c_str()) - clearedData.c_str();
    string back;
   
    for (;;--vit)
    {
        if (strstr(vit->first.c_str(), (vit - 1)->first.c_str()) == NULL)
        {
            back = (vit - 1)->first;
            break;
        }
    }
    int end = strstr(clearedData.c_str(), back.c_str()) - clearedData.c_str();

    if (begin > end)
    {
        int t = begin;
        begin = end;
        end  = t;
    }
    int rbegin;
    int rend;
    
    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (clearedTagPosition[begin].first == modifiedTagPosition[i].first)
            rbegin = i;
        if (clearedTagPosition[end].second == modifiedTagPosition[i].second)
            rend = i;
    }

    // �������� ������� ������� � ������� �
    string res(data, realTagPosition[rbegin].first, 
               realTagPosition[rend].first - realTagPosition[rbegin].first);
    fout << res;
	return 0;
}