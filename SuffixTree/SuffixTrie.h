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
        int nWordStart;
        int nWordEnd;
        int nLinkTo;
        UCHAR letter;
        std::string word;
        UINT count;

        link(void);
        link(int _start, int _end, int _to, UCHAR _let, UINT _count, std::string _text);
    };

    struct node
    {
        std::vector<link> links;
        int suffix;

        std::vector<link>::iterator findByLetter(UCHAR);
        struct cmp :public std::binary_function <link, UCHAR, bool>
        {
            bool operator () (first_argument_type, second_argument_type) const;
        };

        node(void);
    };

    int createNewNode();
    void createLink(int from, int start, int end, int to);
    unsigned char iToUc(int i);
    std::pair<int, int> update(int vertex, int start, int end);
    std::pair<int, int> canonize(int vertexv, int start, int end);
    std::pair<bool, int> CSuffixTrie::testAndSplit(int vertex, int start, int end, unsigned char c);
    int &getSuffix(int v);
    std::vector<node> tree;
    int root;
    int dummy;
    std::string text;
};
