#pragma once
#include <vector>
#include <windows.h>
#include "debug.h"

class CSuffixTrie
{
public:
    CSuffixTrie(std::string _text);
    ~CSuffixTrie();
    void buildSuffixTree();
    int find(std::string);

    struct CSuffixNode
    {
        CSuffixNode(std::string &text, int start, int end, int pathlen);
        CSuffixNode(std::string &text);
        int getLength();
        std::string getString(std::string &text);
        bool isRoot();

        std::string curtext;
        std::vector<CSuffixNode *> children;
        CSuffixNode *link;
        int start;
        int end;
        int pathlen;
        int cnt;
    };

    void walkTree(CSuffixNode *node, int len);
    void OutWalkTree(void);

    struct CState
    {
        CState();

        CSuffixNode *parent;     // parent(head)
        CSuffixNode *link;       // s(head[i - 1])
        CSuffixNode *prefix;     // head[i - 1]
        CSuffixNode *transLink;
        int j;                   // the global index of text starting from 0 to text.length()
        bool finished;           // is this suffix insertion finished?
    };

private:
    CSuffixNode *root;
    std::string text;

    //slow scan until head( = state->v) is found
    void slowscan(CState *state, CSuffixNode *currNode, int j, int insSubStrStart);
    //fast scan until w is found
    void fastscan(CState *state,CSuffixNode *currNode,int uvLen,int j, int insSubStrStart);
    int findStr(std::string data, CSuffixNode *node, int &curPos);
};
