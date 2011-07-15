#include "tagprocess.h"

#include <iostream>
#include <set>
#include <fstream>

using namespace std;

#define MINSZ 2

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
    // измененная modifiedData. В clearedData нет удаленных тегов
    string clearedData;
    // позиция тега в data
	vector< pair<int, int> > realTagPosition;
    // Позиция тега в modifiedData
	vector< pair<int, int> > modifiedTagPosition;
    // Позиция тега в modifiedData
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
			// ;TODO1 пробелы... < a >? 
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
   
    // составляем clearedData
    // ;TODO2 убрать этот костыль и сделать одно: clearedData или 
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

    // составляем set из пар (строка, частота встречи этой строки)
    // set - повторяющиеся элементы удаляются
    // ;TODO3 Убрать этот костыль и составить нормальное составление
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

    // получаем строчку
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
                // если её размер меньше MINSZ тегов - не нужна.
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
                // если больше MINSZ - выбираем все подпоследовательности длины >= MINSZ символов
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

/*    // Шаг. Последовательности какой длины мы ищем.
    // ;TODO4 Поставить шаг не с количества слов, а с меньшего числа
    // последовательности длиной в всю строку встретятся немного раз.
    int step = getWordCount(clearedData);

    // минимум длина = MINSZ
    while (step > MINSZ)
    {
        for (i = getWordCount(clearedData) / step; i - step >= 0; --i)
        {
            // получаем строку из слов
            string ret = getWordString(clearedData, i - step, i);
            // если она начинается с закрывающего тега - отбрасываем
            if (ret[1] == '/')
                continue;
            // выполняем проверки
            if (checksum(ret) && checkWordTruePairs(ret))
            {
                // получаем частоту встречи нашей фразы в clearedData
                int f = getWordFreq(clearedData, ret);
                if (f < MINSZ)
                    continue;
                // ~эвристика, хехе
                // Вставляем в set пар
                freq.insert(make_pair(ret, 1 * f + 30 * getWordCount(ret)));
            }
        }
        // уменьшаем длину фразы.
        --step;
    }
    */
    
    set<pair<string, int>, ltstr>::iterator it;
    vector<pair<string, int>> temp;
    int sum = 0;
    // перекидываем из set в массив
    // ;TODO5 см. ;TODO3
    for(it = freq.begin(); it != freq.end(); it++ )
    {
        if (it->second >= 8)
        {
            temp.push_back(*it);
            sum += it->second;
        }
    }
    // сортируем пары
    sort(temp.begin(), temp.end(), pred());
    // Считаем, что последняя пара в массиве - блок, с которого начинается
    // новость
    string BestString = temp.back().first;
   

    // Ищем по позициям в clearedTagPosition позиции в realTagPosition
    // следующяя новость будет располагаться с начала следующего блока.
    // совпадающего с BestString, так как считаем новости идущими подряд
    // ;TODO6 см ;TODO2
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
    // Получаем строчку новости и выводим её
    string res(data, realTagPosition[rbegin].first, 
               realTagPosition[rend].first - realTagPosition[rbegin].first);
    fout << res;
	return 0;
}