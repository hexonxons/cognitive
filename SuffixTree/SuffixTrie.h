#pragma once
#include<vector>
#include <limits>

class CSuffixTrie
{
public:
    CSuffixTrie(void);
    ~CSuffixTrie(void);
    void Init(std::string data);
    void BuildTrie();
private:

    struct link
    {
      int start;
      int end;
      int to;
      unsigned char let;
      link();
      link(int _start, int _end, int _to, unsigned char _let);
    };

    struct node
    {
      std::vector<link> links;
      int suffix;

      node();
    };

    struct pred
    {
      bool operator()(link left, link right) const;
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
    const int inf;
    std::string text;
};
