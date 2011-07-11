#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>
#include <time.h>

using namespace std;

struct pred
{
    bool operator()(pair<string, int> left, pair<string, int> right)
    {
        if(left.second != right.second)
            return left.second < right.second;
        else
            return left.first.length() > right.first.length();
    }
};


// получение тега по его позиции (начало-конец строки)
string getTag(pair<int, int> &tagPos, const string &src)
{
	string ret(src, tagPos.first, tagPos.second - tagPos.first);
	return ret;
}

// получение тега по его номеру
string getTag(int tagPos, const string &src)
{
    int i = 0;
    int cnt = 0;
    string ret;
    for (i = 0; i < src.size() && cnt != tagPos + 1; ++i)
    {
        if (src[i] == '<')
            ++cnt;
    }
    ret = src[i - 1];

    while (src[i] != '>')
    {
        ret += src[i++];
    }
    ret += '>';
    return ret;
}

// удаление тега из строки
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

// удаление парного тега с содержимым
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
                    if ((*tagsPos)[i].first == -1)
                        continue;

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

// получить набор слов с позиции begin до позиции end
// слова - <*>
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

// получить количество слов в строке
// слова - <*>
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
    if (cnt > getWordCount(src) / 2 + 1)
    {
        return 0;
    }
    return 1;
}

// проверяет, что каждому закрывающему тегу соответствует открывающий
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
            if (st.empty())
                return 0;
            string temp = st.top();
            if (strncmp(temp.c_str() + 1, str.c_str() + 2, 10))
            {
                while (strncmp(temp.c_str() + 1, str.c_str() + 2, 10))
                {
                    st.pop();
                    if (st.empty())
                        return 0;
                    temp = st.top();
                }
            }
            st.pop();
        }
    }
    return 1;
}

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
	int i = 0;
	int j = 0;
	pair <int, int> tagPosition = make_pair(-1, -1);
	pair <int, int> modTagPosition = make_pair(-1, -1);

	// проходим по всему файлу
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
    //remDoubleTag.push_back(make_pair("<p>", "</p>"));

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
        }

    }
    //fout << "\n";

    time_t start;
    time_t finish;
    
    start = clock();
    int step = getWordCount(clearedData);
    while (step > 8)
    {
        for (i = getWordCount(clearedData) / step; i - step > 0; --i)
        {
             string ret = getWord(clearedData, i - step, i);
             if (ret[1] == '/')
                 continue;
             if (checksum(ret) && checkWordTruePairs(ret))
             {
                 int f = getWordFreq(clearedData, ret);
                 if (f < 5)
                     continue;
                 freq.insert(make_pair(ret, 4 * f + 2 * getWordCount(ret)));
                 //i -= getWordCount(ret);
             }
        }
        --step;
    }
    finish = clock();
    
    set<pair<string, int>>::iterator it;
    vector<pair<string, int>> temp;
    for(it = freq.begin(); it != freq.end(); it++ )
    {
        temp.push_back(*it);
        //fout << it->first << " " << it->second << '\n';
    }
    sort(temp.begin(), temp.end(), pred());
    vector<pair<string, int>>::iterator itt;
    for (itt = temp.begin(); itt != temp.end() - 1; ++itt)
    {
        if (!strstr(temp.back().first.c_str(), itt->first.c_str()))
        {
            fout << itt->first << " " << itt->second << '\n';
        }
    }
    fout << temp.back().first << " " << temp.back().second << '\n';

    fout << (finish - start) / CLOCKS_PER_SEC;
    //fout << modifiedData;*/

	return 0;
}