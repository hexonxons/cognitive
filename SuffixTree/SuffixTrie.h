#ifndef __SUFFIXTRIE_H__
#define __SUFFIXTRIE_H__

#include <vector>

class CTrie
{
public:

    struct CSuffixNode
    {
        CSuffixNode(int start, int end, int pathlen);
        CSuffixNode();

        int GetLength();
        bool IsRoot();

        // дети текущего нода
        std::vector<CSuffixNode *> children;
        // суффиксная ссылка
        CSuffixNode *slink;
        // начало суффикса
        int start;
        // конец суффикса
        int end;
        // глубина всего суффикса
        int pathlen;
    };

    struct CState
    {
        CSuffixNode *u;
        CSuffixNode *w;
        CSuffixNode *v; 
        int j;
        bool finished;
    };

    
    CTrie(std::string str);
    void BuildSuffixTree();
    void OutWalkTreeCounter();
    int Find(const std::string &);

private:

    CTrie();
    void slowscan(CState *state, CSuffixNode *currNode, int j);
    void fastscan(CState *state, CSuffixNode *currNode, int uvLen, int j);

    int walkTreeCounter(CSuffixNode *node, std::string curText);
    int findStr(const std::string &data, CSuffixNode *node, int &curPos);

    CSuffixNode *root;
    std::string text;
    std::vector<std::pair<std::string, int>> freq;
};

#endif  //__SUFFIXTRIE_H__
