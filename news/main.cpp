#include "tagprocess.h"

#include <iostream>
#include <set>
#include <fstream>

using namespace std;

int main()
{
    // Входной файл
	fstream fin("news", ios::in);
    // Выходной файл
	fstream fout("out", ios::out);
    // Строчка, содержащая в себе входной файл
	string data((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    // измененная строчка входного файла
	string modifiedData;
    // позиция тега в data
	vector< pair<int, int> > realTagPosition;
    // Позиция тега в modifiedData
	vector< pair<int, int> > modifiedTagPosition;
    vector< pair<int, int> > clearedTagPosition;
	int i = 0;
	int j = 0;
	pair <int, int> tagPosition = make_pair(-1, -1);
	pair <int, int> modTagPosition = make_pair(-1, -1);

	// проходим по всему файлу
	for(i = 0; i < data.size(); ++i)
	{
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

    // чистка от ненужных тегов
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
            clearedTagPosition.push_back(modifiedTagPosition[i]);
        }

    }

    int step = getWordCount(clearedData);
    while (step > 8)
    {
        for (i = getWordCount(clearedData) / step; i - step >= 0; --i)
        {
             string ret = getWordString(clearedData, i - step, i);
             if (ret[1] == '/')
                 continue;
             if (checksum(ret) && checkWordTruePairs(ret))
             {
                 int f = getWordFreq(clearedData, ret);
                 if (f < 8)
                     continue;
                 freq.insert(make_pair(ret, 1 * f + 30 * getWordCount(ret)));
             }
        }
        --step;
    }
    
    set<pair<string, int>>::iterator it;
    vector<pair<string, int>> temp;
    for(it = freq.begin(); it != freq.end(); it++ )
    {
        temp.push_back(*it);
    }
    sort(temp.begin(), temp.end(), pred());

    string BestString = temp.back().first;


    const char *ptr = strstr(clearedData.c_str(), temp.back().first.c_str());

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
    string res(data, realTagPosition[rbegin].first, realTagPosition[rend].first - realTagPosition[rbegin].first);
    fout << res;
	return 0;
}