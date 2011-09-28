#pragma once
#include <vector>
#include <windows.h>
#include <functional>

class CSuffixTrie
{
public:
    CSuffixTrie(void);
    ~CSuffixTrie(void);

    void Init(std::string data);
    void BuildTrie();
    bool Present(std::string word);
private:

    struct link
    {
        // Начало слова
        int nWordStart;
        // Конец слова
        int nWordEnd;
        // Номер вершины, в которую ведет ссылка
        int nLinkTo;
        // Буква, с которой начинается слово
        UCHAR letter;
        // Слово
        std::string word;
        // Количество встреч
        UINT count;

        link(void);
        link(int _start, int _end, int _to, UCHAR _let, UINT _count, std::string _text);
    };

    struct node
    {
        // Массив ссылок
        std::vector<link> links;
        // Родитель текущего нода
        int suffix;

        // Функция поиска в массиве ссылок
        std::vector<link>::iterator findByLetter(UCHAR);

        struct cmp :public std::binary_function <link, UCHAR, bool>
        {
            bool operator () (first_argument_type, second_argument_type) const;
        };

        node(void);
    };

    // Функция создания новой вершины
    int createNewNode();
    // Функция создания новой ссылки
    void createLink(int from, int start, int end, int to);
    // Функция перевода числа в букву из слова
    unsigned char iToUc(int i);

    std::pair<int, int> update(int vertex, int start, int end);
    std::pair<int, int> canonize(int vertexv, int start, int end);
    std::pair<bool, int> CSuffixTrie::testAndSplit(int vertex, int start, int end, unsigned char c);
    // Функция задания родителя нода
    int &getSuffix(int v);
    // Само дерево
    std::vector<node> tree;
    // Корень дерева
    int root;
    // Виртуальная вершина - родитель корня
    int dummy;
    // Строка для построения суффиксного дерева
    std::string text;
};
