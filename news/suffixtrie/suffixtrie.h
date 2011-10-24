/*
 *  suffixtrie.h
 *  suffixtrie.cpp prototypes.
 *  24 October 2011
 *  Hexonxonx
 */

#ifndef __SUFFIXTRIE_H__
#define __SUFFIXTRIE_H__

#include <vector>

#include "../include/memalloc.h"

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
        std::vector<CSuffixNode *> m_children;
        // суффиксная ссылка
        CSuffixNode *m_pLink;
        // начало суффикса
        int m_start;
        // конец суффикса
        int m_end;
        // глубина всего суффикса
        int m_pathlen;
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
    ~CTrie();
    void BuildSuffixTree();
    void OutWalkTreeCounter();
    int Find(const std::string &str);

private:

    CTrie();
    void slowscan(CState *state, CSuffixNode *currNode, int j);
    void fastscan(CState *state, CSuffixNode *currNode, int uvLen, int j);

    int walkTreeCounter(CSuffixNode *node, std::string curText);
    int findStr(const std::string &data, CSuffixNode *node, int &curPos);

    CSuffixNode *m_pRoot;
    CBasicDataBase<CSuffixNode> m_nodeStorage;
    CBasicDataBase<CState> m_stateStorage;

    std::string m_text;
    std::vector<std::pair<std::string, int>> m_freq;
};

#endif  //__SUFFIXTRIE_H__
