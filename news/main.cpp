#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

string getTag(pair<int, int> &tagPos, const string &src)
{
	string ret(src, tagPos.first, tagPos.second - tagPos.first);
	return ret;
}

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

int getWordCount(const string &src, const string &word)
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

int main()
{
	fstream fin("news", ios::in);
	fstream fout("out", ios::out);
	string data((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	string modifiedData;
	vector< pair<int, int> > realTagPosition;
	vector< pair<int, int> > modifiedTagPosition;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int strSz = data.size();
	pair <int, int> tagPosition = make_pair(-1, -1);
	pair <int, int> modTagPosition = make_pair(-1, -1);

	// проходим по всему файлу
	for(i = 0; i < strSz; ++i)
	{
		/*if (data[i] ==  10)
		{
			if (j > 0 && modifiedData[j - 1] != 10)
			{
				modifiedData += data[i];
				++j;
			}
		}*/
		// если (возможно) открывающий тег
		if (data[i] == '<')
		{
			// к примеру, знак < в js
			if (tagPosition.first != -1)
				tagPosition.first = i;
			// знаем, с какой позиции лежит тег
			tagPosition.first = i;
			modTagPosition.first = j;
			// пока не дойдем до пробела/закрывающей скобки - записываем
			// ! пробелы... < a >? 
			while (data[i] != ' ' && data[i] != '>')
			{
				modifiedData += data[i];
				++j;
				++i;				
			}
		}
		// если закрывающая скобка - записываем
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

	vector< pair<string, string> > remDoubleTag;
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
   
    set<string> alphabet;

    for(i = 1; i < modifiedTagPosition.size(); ++i)
    {
        if (modifiedTagPosition[i].first != -1)
        {
            string tag = getTag(modifiedTagPosition[i], modifiedData);
            alphabet.insert(tag);
            fout << tag;// << '\n';
        }

    }
    
    /*vector< pair<string, int> > freq;
    for (i = 0; i < modifiedData.size(); ++i)
    {
        for (j = 0; i + j < modifiedData.size(); ++j)
        {
            string ret(modifiedData, i, i + j);
            fout << ret;
        }
    }*/
    //fout << modifiedData;

	return 0;
}