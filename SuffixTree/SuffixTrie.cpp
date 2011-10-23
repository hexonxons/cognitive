#include "debug.h"
#include "suffixtrie.h"

#include <iostream>
#include <string>
#include <algorithm>

using std::string;
using std::vector;

CTrie::CSuffixNode::CSuffixNode(int start, int end, int pathlen)
{
    this->start = start;
    this->end = end;
    this->pathlen = pathlen;
}

CTrie::CSuffixNode::CSuffixNode()
{
    this->start = -1;
    this->end = -1;
    this->pathlen = 0;
}

int CTrie::CSuffixNode::GetLength()
{
    if(start == -1)
        return 0;
    else 
        return end - start + 1;
}

bool CTrie::CSuffixNode::IsRoot()
{
    return start == -1;
}

CTrie::CTrie(string str)
{
    text = str;
    root = new CSuffixNode();
    root->slink = root;
}

void CTrie::BuildSuffixTree()
{
    CSuffixNode *u = root;
    CSuffixNode *v = root;
    CState *state = new CState();		

    for(int i = 0; i < text.length(); i++)
    {
        CSuffixNode *s = u->slink;

        int uvlen = v->pathlen - u->pathlen;

        if(u->IsRoot() && !v->IsRoot())
        {
            --uvlen;
        }

        int j = s->pathlen + i;		

        state->u = s;
        state->w = s;
        state->v = s;
        state->j = j;
        state->finished = false;

        if(uvlen > 0)
        {
            fastscan(state, s, uvlen, j);
        }

        CSuffixNode *w = state->w;
        v->slink = w;

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

void CTrie::slowscan(CState *state, CSuffixNode *currnode, int j)
{
    bool done = false;
    int keylen = text.length() - j;

    for(int i = 0; i < currnode->children.size(); ++i)
    {
        CSuffixNode *child = currnode->children[i];

        int childkeylen = child->GetLength();
        int len = childkeylen < keylen ? childkeylen : keylen;
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
                int pathlen = text.length() - j + currnode->pathlen;
                CSuffixNode *node = new CSuffixNode(j, text.length() - 1, pathlen);

                currnode->children.insert(currnode->children.begin() + i, node);
                state->v = currnode;
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
                if(keylen > childkeylen)
                { 
                    state->u = child;
                    j += childkeylen;
                    state->j = j;
                    slowscan(state, child, j);
                }
            }
            else
            {
                int nodepathlen = child->pathlen - (child->GetLength() - delta);
                CSuffixNode *node = new CSuffixNode(child->start, child->start + delta - 1, nodepathlen);

                int tailpathlen = (text.length() - (j + delta)) + nodepathlen;
                CSuffixNode *tail = new CSuffixNode(j + delta, text.length() - 1, tailpathlen);

                child->start += delta;

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
                currnode->children[i] = node;

                state->v = node;
                state->finished = true;
            }
            done = true;
            break;
        }
    }

    if(!done)
    {
        int pathlen = text.length() - j + currnode->pathlen;
        CSuffixNode *node = new CSuffixNode(j, text.length() - 1, pathlen);

        currnode->children.push_back(node);
        state->v = currnode;
        state->finished = true;
    }
}

void CTrie::fastscan(CState *state, CSuffixNode *currNode, int uvlen, int j)
{		  
    for(int i = 0; i < currNode->children.size(); ++i)
    {
        CSuffixNode *child = currNode->children[i];

        if(text[child->start] == text[j])
        {
            int len = child->GetLength();

            if(uvlen == len)
            {
                state->u = child;
                state->w = child;
                state->j = j + len;
                break;
            }
            else 
            {
                if(uvlen < len)
                {
                    int nodepathlen = child->pathlen - (child->GetLength() - uvlen);
                    CSuffixNode *node = new CSuffixNode(child->start, child->start + uvlen - 1, nodepathlen);

                    int tailpathlen = (text.length() - (j + uvlen)) + nodepathlen;
                    CSuffixNode *tail = new CSuffixNode(j + uvlen, text.length() - 1, tailpathlen);

                    child->start += uvlen;
                    if(text[j + uvlen] < text[child->start])
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
                    uvlen -= len;
                    state->u = child;
                    j += len;
                    state->j = j;

                    fastscan(state, child, uvlen, j);
                }
                break;
            }
        }
    }
}

int CTrie::findStr(const string &data, CSuffixNode *node, int &curpos)
{
    bool flag = false;
    for (int i = 0; i < node->children.size(); ++i)
    {
        int ind = 0;
        while(text[node->children[i]->start + ind] == data[curpos])
        {
            ++curpos;
            ++ind;
            flag = true;
            if (curpos == data.length())
            {
                return 1;
            }
        }
        if (flag)
            return findStr(data, node->children[i], curpos);
    }
    return 0;
}

int CTrie::Find(const string &data)
{
    int count = 0;
    return findStr(data, root, count);
}

int CTrie::walkTreeCounter(CSuffixNode *node, string curText)
{
    int counter = 0;

    if (node->start != -1)
    {
        curText += string(text, node->start, node->end - node->start + 1);
    }

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

#ifdef _DEBUG
    for (std::vector<std::pair<std::string, int>>::iterator it = freq.begin(); it != freq.end(); ++it)
    {
        DebugPrint("%s -- %d\n", it->first.c_str(), it->second);
    }
#endif  //_DEBUG

}