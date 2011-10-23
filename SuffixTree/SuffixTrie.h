#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct CSuffixNode
{
    string text;
    vector<CSuffixNode *> children;
    CSuffixNode *link;
    int start;
    int end;
    int pathlen;

    CSuffixNode(string str, int start, int end, int pathlen);
    CSuffixNode(string str);
    int getLength();
    string getString(string text);
    bool isRoot();
};

struct CState
{
    CSuffixNode *u;
    CSuffixNode *w;
    CSuffixNode *v; 
    int j;
    bool finished;
};


class CTrie
{
public:
    void buildSuffixTree();
    CTrie(string str);
    void slowscan(CState *state, CSuffixNode *currNode, int j);
    void fastscan(CState *state,CSuffixNode *currNode,int uvLen,int j);
    void OutWalkTreeCounter();
    int find(std::string);
private:
    int walkTreeCounter(CSuffixNode *node, std::string curText);
    int findStr(std::string data, CSuffixNode *node, int &curPos);
    CSuffixNode *root;
    string text;
    std::vector<std::pair<std::string, int>> freq;
};