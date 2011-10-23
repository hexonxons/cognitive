#include "SuffixTrie.h"
#include <algorithm>

using namespace std;

CSuffixNode::CSuffixNode(string text, int start, int end, int pathlen)
{
    this->text = text.substr(start, end - start + 1);
    this->start = start;
    this->end = end;
    this->pathlen = pathlen;
}

CSuffixNode::CSuffixNode(string text)
{
    this->text = "";
    this->start = -1;
    this->end = -1;
    this->pathlen = 0;
}

int CSuffixNode::getLength()
{
    if(start == -1)
        return 0;
    else 
        return end - start + 1;
}

string CSuffixNode::getString(string text)
{
    if(start != -1)
    {
        return text.substr(start, end - start + 1);
    }
    else
    {
        return "";
    }
}

bool CSuffixNode::isRoot()
{
    return start == -1;
}

CTrie::CTrie(string str)
{
    text = str;
    root = new CSuffixNode(str);
    root->link = root;
}

void CTrie::buildSuffixTree()
{
    CSuffixNode *u = root;
    CSuffixNode *v = root;
    CState *state = new CState();		

    for(int i = 0; i < text.length(); i++)
    {
        std::string insertVal = text.substr(i, text.size() - i);

        CSuffixNode *s = u->link;

        int uvLen = v->pathlen - u->pathlen;

        if(u->isRoot() && !v->isRoot())
        {
            --uvLen;
        }
        int j = s->pathlen + i;		

        state->u = s;
        state->w = s;
        state->v = s;
        state->j = j;
        state->finished = false;

        if(uvLen > 0)
        {
            fastscan(state, s, uvLen, j);
        }

        CSuffixNode *w = state->w;
        v->link = w;

        if(!state->finished)
        {
            j = state->j;
            state->u = w;
            slowscan(state, w, j);
        }		

        u = state->u;
        v = state->v;
    }
}

void CTrie::slowscan(CState *state, CSuffixNode *currNode, int j)
{
    std::string insertVal = text.substr(j, text.size() - j);

    bool done = false;
    int keyLen = text.length() - j;
    for(int i = 0; i < currNode->children.size(); ++i)
    {
        CSuffixNode *child = currNode->children[i];

        int childKeyLen = child->getLength();
        int len = childKeyLen < keyLen ? childKeyLen : keyLen;
        int delta = 0;
        for(; delta < len; delta++)
        {
            if(text[j + delta] != text[child->start + delta])
                break;
        }

        if(delta == 0)
        {
            if(text[j] < text[child->start])
            {
                int pathlen = text.length() - j + currNode->pathlen;
                CSuffixNode *node = new CSuffixNode(text, j, text.length() - 1, pathlen);
                currNode->children.insert(currNode->children.begin() + i, node);
                state->v = currNode;
                state->finished = true;
                done = true;
                break;
            }
            else
                continue;
        }
        else
        {
            if(delta == len)
            {
                if(keyLen > childKeyLen)
                { 
                    state->u = child;
                    j += childKeyLen;
                    state->j = j;
                    slowscan(state, child, j);
                }
            }
            else
            {
                int nodepathlen = child->pathlen - (child->getLength() - delta);
                CSuffixNode *node = new CSuffixNode(text, child->start, child->start + delta - 1, nodepathlen);

                int tailpathlen = (text.length() - (j + delta)) + nodepathlen;
                CSuffixNode *tail = new CSuffixNode(text, j + delta, text.length() - 1, tailpathlen);

                child->start += delta;
                child->text = child->getString(text);

                if(text[j + delta] < text[child->start])
                {
                    node->children.push_back(tail);
                    node->children.push_back(child);
                }
                else
                {
                    node->children.push_back(child);
                    node->children.push_back(tail);
                }
                currNode->children[i] = node;

                state->v = node;
                state->finished = true;
            }
            done = true;
            break;
        }
    }
    if(!done)
    {
        int pathlen = text.length() - j + currNode->pathlen;
        CSuffixNode *node = new CSuffixNode(text, j, text.length() - 1, pathlen);
        currNode->children.push_back(node);
        state->v = currNode;
        state->finished = true;
    }
}

void CTrie::fastscan(CState *state, CSuffixNode *currNode, int uvLen, int j)
{		  
    std::string insertVal = text.substr(j, text.size() - j);

    for(int i = 0; i < currNode->children.size(); ++i)
    {
        CSuffixNode *child = currNode->children[i];

        if(text[child->start] == text[j])
        {
            int len = child->getLength();
            if(uvLen == len)
            {
                state->u = child;
                state->w = child;
                state->j = j + len;
                break;
            }
            else 
            {
                if(uvLen < len)
                {
                    int nodepathlen = child->pathlen - (child->getLength() - uvLen);
                    CSuffixNode *node = new CSuffixNode(text, child->start, child->start + uvLen - 1, nodepathlen);

                    int tailpathlen = (text.length() - (j + uvLen)) + nodepathlen;
                    CSuffixNode *tail = new CSuffixNode(text, j + uvLen, text.length() - 1, tailpathlen);

                    child->start += uvLen;
                    child->text = child->getString(text);
                    if(text[j + uvLen] < text[child->start])
                    {
                        node->children.push_back(tail);
                        node->children.push_back(child);
                    }
                    else
                    {
                        node->children.push_back(child);
                        node->children.push_back(tail);
                    }

                    currNode->children[i] = node;

                    state->w = node;
                    state->finished = true;
                    state->v = node;					

                }
                else
                {
                    uvLen -= len;
                    state->u = child;
                    j += len;
                    state->j = j;

                    fastscan(state, child, uvLen, j);
                }
                break;
            }
        }
    }
}

int CTrie::findStr(std::string data, CSuffixNode *node, int &curPos)
{
    bool flag = false;
    for (int i = 0; i < node->children.size(); ++i)
    {
        int ind = 0;
        while(node->children[i]->text[ind] == data[curPos])
        {
            ++curPos;
            ++ind;
            flag = true;
            if (curPos == data.length())
            {
                return 1;
            }
        }
        if (flag)
            return findStr(data, node->children[i], curPos);
    }
    return 0;
}

int CTrie::find(std::string data)
{
    int count = 0;
    return findStr(data, root, count);
}

int CTrie::walkTreeCounter(CSuffixNode *node, string curText)
{
    int counter = 0;

    string data(node->text);
    //reverse(data.begin(), data.end());
    curText += data;
    // если детей нет - встретилось 1 раз
    if (node->children.size() == 0)
    {
        freq.push_back(make_pair(curText, 1));
        return 1;
    }
    // иначе складываем степень всех детей
    for (int i = 0; i < node->children.size(); ++i)
    {
        counter += walkTreeCounter(node->children[i], curText);
    }
    freq.push_back(make_pair(curText, counter));

    return counter;
}

void CTrie::OutWalkTreeCounter()
{
    walkTreeCounter(root, "");
    for (std::vector<std::pair<std::string, int>>::iterator it = freq.begin(); it != freq.end(); ++it)
    {
        cout << it->first << " -- " << it->second << "\n";
    }
}