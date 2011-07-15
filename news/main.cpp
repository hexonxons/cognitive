#include "tagprocess.h"

#include <iostream>
#include <set>
#include <fstream>

using namespace std;

#define MINSZ 2

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
    // ;TODO2 ������ ���� ������� � ������� ����: clearedData ��� 
    // modifiedData
    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (modifiedTagPosition[i].first != -1)
        {
            string tag = getTag(modifiedTagPosition[i], modifiedData);
            int ch = 0;
            if (tag[1] != '/')
                ch = tag[1];
            else
                ch = '/' + tag[2]; 
            clearedData += ch;
            clearedTagPosition.push_back(modifiedTagPosition[i]);
        }
    }

    // ���������� set �� ��� (������, ������� ������� ���� ������)
    // set - ������������� �������� ���������
    // ;TODO3 ������ ���� ������� � ��������� ���������� �����������
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

    // �������� �������
    int diag = 1;
    while(diag != sz)
    {
        for (i = 0; i < sz - diag; ++i)
        {
            if (table[i][i + diag] != 0)
            {
                string str;
                int beg = i;
                while (i < sz - diag && table[i][i + diag] != 0)
                {
                    str += clearedData[i];
                    ++i;
                }
                // ���� � ������ ������ MINSZ ����� - �� �����.
                if (str.size() == MINSZ)
                {
                    setIter = freq.find(make_pair(str, 0));
                    if (setIter == freq.end())
                    {
                        if (checksum(str, 0) && checkWordTruePairs(str, 0))
                        {
                            freq.insert(make_pair(str, 1));
                        }
                    }
                    else
                        setIter->second++;
                }
                // ���� ������ MINSZ - �������� ��� ��������������������� ����� >= MINSZ ��������
                if (str.size() > MINSZ)
                {
                    int ind = 0;
                    int jnd = 0;
                    for(ind = MINSZ; ind <= str.size(); ++ind)
                    {
                        for (jnd = 0; jnd + ind <= str.size(); ++jnd)
                        {
                            string subs(str.c_str() + jnd, ind);
                            if (checksum(subs, 0) && checkWordTruePairs(subs, 0))
                            {
                                setIter = freq.find(make_pair(subs, 0));
                                if (setIter == freq.end())
                                    freq.insert(make_pair(subs, 1));
                                else
                                    setIter->second++;
                            }
                        }
                    }
                }
            }
        }
        ++diag;
    }
    
    //////////////////////////////////////////////////////////////////////////

/*    // ���. ������������������ ����� ����� �� ����.
    // ;TODO4 ��������� ��� �� � ���������� ����, � � �������� �����
    // ������������������ ������ � ��� ������ ���������� ������� ���.
    int step = getWordCount(clearedData);

    // ������� ����� = MINSZ
    while (step > MINSZ)
    {
        for (i = getWordCount(clearedData) / step; i - step >= 0; --i)
        {
            // �������� ������ �� ����
            string ret = getWordString(clearedData, i - step, i);
            // ���� ��� ���������� � ������������ ���� - �����������
            if (ret[1] == '/')
                continue;
            // ��������� ��������
            if (checksum(ret) && checkWordTruePairs(ret))
            {
                // �������� ������� ������� ����� ����� � clearedData
                int f = getWordFreq(clearedData, ret);
                if (f < MINSZ)
                    continue;
                // ~���������, ����
                // ��������� � set ���
                freq.insert(make_pair(ret, 1 * f + 30 * getWordCount(ret)));
            }
        }
        // ��������� ����� �����.
        --step;
    }
    */
    
    set<pair<string, int>, ltstr>::iterator it;
    vector<pair<string, int>> temp;
    int sum = 0;
    // ������������ �� set � ������
    // ;TODO5 ��. ;TODO3
    for(it = freq.begin(); it != freq.end(); it++ )
    {
        if (it->second >= 8)
        {
            temp.push_back(*it);
            sum += it->second;
        }
    }
    // ��������� ����
    sort(temp.begin(), temp.end(), pred());
    // �������, ��� ��������� ���� � ������� - ����, � �������� ����������
    // �������
    string BestString = temp.back().first;
   

    // ���� �� �������� � clearedTagPosition ������� � realTagPosition
    // ��������� ������� ����� ������������� � ������ ���������� �����.
    // ������������ � BestString, ��� ��� ������� ������� ������� ������
    // ;TODO6 �� ;TODO2
    int begin = wordSubPtr(clearedData, temp.back().first);
    const char *Rptr = strstr(clearedData.c_str(), temp.back().first.c_str());
    int end = wordSubPtr(Rptr, temp.back().first, 1) + begin;

    int rbegin;
    int rend;
    
    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (clearedTagPosition[begin].first == modifiedTagPosition[i].first)
            rbegin = i;
    }

    for(i = 0; i < modifiedTagPosition.size(); ++i)
    {
        if (clearedTagPosition[end].second == modifiedTagPosition[i].second)
            rend = i;
    }
    // �������� ������� ������� � ������� �
    string res(data, realTagPosition[rbegin].first, 
               realTagPosition[rend].first - realTagPosition[rbegin].first);
    fout << res;
	return 0;
}