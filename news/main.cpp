#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>

using namespace std;

// ��������� ���� �� ��� ������� (������-����� ������)
string getTag(pair<int, int> &tagPos, const string &src)
{
	string ret(src, tagPos.first, tagPos.second - tagPos.first);
	return ret;
}

// ��������� ���� �� ��� ������
string getTag(int tagPos, const string &src)
{
    int i = 0;
    int cnt = 0;
    string ret;
    for (i = 0; i < src.size() && cnt != tagPos; ++i)
    {
        if (src[i] == '<')
            ++cnt;
    }

    while (src[cnt] != '>')
    {
        ret += src[cnt++];
    }
    ret += '>';
    return ret;
}

// �������� ���� �� ������
void removeTags(vector< pair<int, int> > *tagsPos, const string &src, vector<string> &tagsToRemove)
{
	unsigned int i = 0;
	unsigned int j = 0;
	for (i = 0; i < tagsPos->size(); ++i)
	{
        if ((*tagsPos)[i].first == -1)
            continue;
		string tag = getTag((*tagsPos)[i], src);
        transform(tag.begin(), tag.end(), tag.begin(), tolower);
		for (j = 0; j < tagsToRemove.size(); ++j)
		{
			if (!strncmp(tag.c_str(), tagsToRemove[j].c_str(), tagsToRemove[j].size() - 1))
				(*tagsPos)[i] = make_pair(-1, -1);
		}
	}
}

// �������� ������� ���� � ����������
void removeTags(vector< pair<int, int> > *tagsPos, const string &src, 
				vector< pair<string, string> > &tagsToRemove)
{
	unsigned int i = 0;
	unsigned int j = 0;
	for (i = 0; i < tagsPos->size(); ++i)
	{
        if ((*tagsPos)[i].first == -1)
            continue;
		string tag = getTag((*tagsPos)[i], src);
        transform(tag.begin(), tag.end(), tag.begin(), tolower);
		for (j = 0; j < tagsToRemove.size(); ++j)
		{
			if (!strncmp(tag.c_str(), tagsToRemove[j].first.c_str(),
									  tagsToRemove[j].first.size() - 1))
            {
				(*tagsPos)[i] = make_pair(-1, -1);
                while (strncmp(tag.c_str(), tagsToRemove[j].second.c_str(),
                                            tagsToRemove[j].second.size() - 1))
                {
                    ++i;
                    tag = getTag((*tagsPos)[i], src);
                    (*tagsPos)[i] = make_pair(-1, -1);
                }
            }

		}
	}
}

int getWordFreq(const string &src, const string &word)
{
    int count = 0;
    int i = 0;
    for(i = 0; i < src.size(); ++i)
    {
        if (!strncmp(src.c_str() + i, word.c_str(), word.size()))
            ++count;
    }
    return count;
}

// �������� ����� ���� � ������� begin �� ������� end
// ����� - <*>
string getWord(const string &src, int begin, int end)
{
    int i = 0;
    int cnt = 0;
    string ret;
    for (i = 0; i < src.size(); ++i)
    {
        if (src[i] == '<')
            ++cnt;
        if (cnt == begin + 1)
        {
            while (cnt != end + 1 && i < src.size())
            {
                ret += src[i];
                if (src[i] == '>')
                    ++cnt;
                ++i;
                
            }
            return ret;
        }
    }
    return NULL;
}

// �������� ���������� ���� � ������
// ����� - <*>
int getWordCount(const string &src)
{
    int cnt = 0;
    int i = 0;

    for (i = 0; i < src.size(); ++i)
    {
        if (src[i] == '>')
            ++cnt;
    }
    return cnt;
}

// in process
int checksum(const string &src)
{
    int i = 0;
    int cnt = 0;

    for (i = 0; i < src.size(); ++i)
    {
        if (src[i] == '/')
            ++cnt;
    }
    if (cnt > getWordCount(src) / 2)
    {
        return 0;
    }
    return 1;
}

int checkWordTruePairs(const string &src)
{
    int i;
    stack<string> st;

    for(i = 0; i < getWordCount(src); ++i)
    {
        string str = getTag(i, src);
        if (strncmp(str.c_str(), "</", 2))
            st.push(str);
        else
        {
            string temp = st.top();
            if (strncmp(temp.c_str() + 1, str.c_str() + 2, 10));
            {
                if (!strncmp(str.c_str(), "</a", 3) || !strncmp(str.c_str(), "</span", 6))
                    return 0;
            }
            st.pop();
        }
    }
    return 1;
}

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
    // ������� ���� � data
	vector< pair<int, int> > realTagPosition;
    // ������� ���� � modifiedData
	vector< pair<int, int> > modifiedTagPosition;
	unsigned int i = 0;
	unsigned int j = 0;
	pair <int, int> tagPosition = make_pair(-1, -1);
	pair <int, int> modTagPosition = make_pair(-1, -1);

	// �������� �� ����� �����
	for(i = 0; i < data.size(); ++i)
	{
		/*if (data[i] ==  10)
		{
			if (j > 0 && modifiedData[j - 1] != 10)
			{
				modifiedData += data[i];
				++j;
			}
		}*/
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
			// ! �������... < a >? 
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
    remDoubleTag.push_back(make_pair("<script>", "</script>"));
    remDoubleTag.push_back(make_pair("<noscript>", "</noscript>"));
    remDoubleTag.push_back(make_pair("<form>", "</form>"));
    remDoubleTag.push_back(make_pair("<iframe>", "</iframe>"));

    removeTags(&modifiedTagPosition, modifiedData, remTag);
    removeTags(&modifiedTagPosition, modifiedData, remDoubleTag);
   
    // in process
    set<string> alphabet;
    set<pair<string, int>> freq;
    string clearedData;
    for(i = 1; i < modifiedTagPosition.size(); ++i)
    {
        if (modifiedTagPosition[i].first != -1)
        {
            string tag = getTag(modifiedTagPosition[i], modifiedData);
            alphabet.insert(tag);
            //fout << tag;// << '\n';
            clearedData += tag;
        }

    }
    
    int step = 10;
    while (step != getWordCount(clearedData))
    {
        for (i = 0; i + step < getWordCount(clearedData); ++i)
        {
             string ret = getWord(clearedData, i, i + step);
             if (ret[1] == '/')
                 continue;
             if (checksum(ret) && checkWordTruePairs(ret))
             {
                 int f = getWordFreq(clearedData, ret);
                 if (f < 5)
                     continue;
                 freq.insert(make_pair(ret, f));
             }
        }
        ++step;
    }
    
    set<pair<string, int>>::iterator it;
    for(it = freq.begin(); it != freq.end(); it++ )
    {
        fout << it->first << " " << it->second << '\n';
    }
    //fout << modifiedData;

	return 0;
}