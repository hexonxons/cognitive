/*
 *  suffixtrie.cpp
 *  realization of suffix tree class.
 *  24 October 2011
 *  Hexonxonx
 */

#include <iostream>
#include <string>
#include <algorithm>

#include "../include/debug.h"
#include "suffixtrie.h"

using std::string;
using std::vector;

CTrie::CSuffixNode::CSuffixNode(int start, int end, int pathlen)
{
    this->m_start = start;
    this->m_end = end;
    this->m_pathlen = pathlen;
}

CTrie::CSuffixNode::CSuffixNode()
{
    this->m_start = -1;
    this->m_end = -1;
    this->m_pathlen = 0;
}

int CTrie::CSuffixNode::GetLength()
{
    if(m_start == -1)
        return 0;
    else 
        return m_end - m_start + 1;
}

bool CTrie::CSuffixNode::IsRoot()
{
    return m_start == -1;
}

CTrie::CTrie(string str)
{
    m_text = str;
    m_pRoot = m_nodeStorage.Add(CSuffixNode());
    m_pRoot->m_pLink = m_pRoot;
}

CTrie::~CTrie()
{
    m_nodeStorage.~CBasicDataBase();
    m_stateStorage.~CBasicDataBase();
}

void CTrie::BuildSuffixTree()
{
    CSuffixNode *u = m_pRoot;
    CSuffixNode *v = m_pRoot;
    CState *state = m_stateStorage.Add(CState());

    for(int i = 0; i < m_text.length(); ++i)
    {
        CSuffixNode *s = u->m_pLink;

        int uvlen = v->m_pathlen - u->m_pathlen;

        if(u->IsRoot() && !v->IsRoot())
        {
            --uvlen;
        }

        int j = s->m_pathlen + i;		

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
        v->m_pLink = w;

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
    int keylen = m_text.length() - j;

    for(int i = 0; i < currnode->m_children.size(); ++i)
    {
        CSuffixNode *child = currnode->m_children[i];

        int childkeylen = child->GetLength();
        int len = childkeylen < keylen ? childkeylen : keylen;
        int delta = 0;

        for(; delta < len; ++delta)
        {
            if(m_text[j + delta] != m_text[child->m_start + delta])
                break;
        }

        if(delta == 0)
        {
            if(m_text[j] < m_text[child->m_start])
            {
                int pathlen = m_text.length() - j + currnode->m_pathlen;
                CSuffixNode *node = m_nodeStorage.Add(CSuffixNode(j, m_text.length() - 1, pathlen));

                currnode->m_children.insert(currnode->m_children.begin() + i, node);
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
                int nodepathlen = child->m_pathlen - (child->GetLength() - delta);
                CSuffixNode *node = m_nodeStorage.Add(CSuffixNode(child->m_start, child->m_start + delta - 1, nodepathlen));

                int tailpathlen = (m_text.length() - (j + delta)) + nodepathlen;
                CSuffixNode *tail = m_nodeStorage.Add(CSuffixNode(j + delta, m_text.length() - 1, tailpathlen));

                child->m_start += delta;

                if(m_text[j + delta] < m_text[child->m_start])
                {
                    node->m_children.push_back(tail);
                    node->m_children.push_back(child);
                }
                else
                {
                    node->m_children.push_back(child);
                    node->m_children.push_back(tail);
                }
                currnode->m_children[i] = node;

                state->v = node;
                state->finished = true;
            }
            done = true;
            break;
        }
    }

    if(!done)
    {
        int pathlen = m_text.length() - j + currnode->m_pathlen;
        CSuffixNode *node = m_nodeStorage.Add(CSuffixNode(j, m_text.length() - 1, pathlen));

        currnode->m_children.push_back(node);
        state->v = currnode;
        state->finished = true;
    }
}

void CTrie::fastscan(CState *state, CSuffixNode *currNode, int uvlen, int j)
{		  
    for(int i = 0; i < currNode->m_children.size(); ++i)
    {
        CSuffixNode *child = currNode->m_children[i];

        if(m_text[child->m_start] == m_text[j])
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
                    int nodepathlen = child->m_pathlen - (child->GetLength() - uvlen);
                    CSuffixNode *node = m_nodeStorage.Add(CSuffixNode(child->m_start, child->m_start + uvlen - 1, nodepathlen));

                    int tailpathlen = (m_text.length() - (j + uvlen)) + nodepathlen;
                    CSuffixNode *tail = m_nodeStorage.Add(CSuffixNode(j + uvlen, m_text.length() - 1, tailpathlen));

                    child->m_start += uvlen;
                    if(m_text[j + uvlen] < m_text[child->m_start])
                    {
                        node->m_children.push_back(tail);
                        node->m_children.push_back(child);
                    }
                    else
                    {
                        node->m_children.push_back(child);
                        node->m_children.push_back(tail);
                    }

                    currNode->m_children[i] = node;

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
    for (int i = 0; i < node->m_children.size(); ++i)
    {
        int ind = 0;
        while(m_text[node->m_children[i]->m_start + ind] == data[curpos])
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
            return findStr(data, node->m_children[i], curpos);
    }
    return 0;
}

int CTrie::Find(const string &data)
{
    int count = 0;
    return findStr(data, m_pRoot, count);
}

int CTrie::walkTreeCounter(CSuffixNode *node, string curText)
{
    int counter = 0;

    if (node->m_start != -1)
    {
        curText += string(m_text, node->m_start, node->m_end - node->m_start + 1);
    }

    // если детей нет - встретилось 1 раз
    if (node->m_children.size() == 0)
    {
        m_freq.push_back(make_pair(curText, 1));
        return 1;
    }

    // иначе складываем степень всех детей
    for (int i = 0; i < node->m_children.size(); ++i)
    {
        counter += walkTreeCounter(node->m_children[i], curText);
    }
    m_freq.push_back(make_pair(curText, counter));

    return counter;
}

void CTrie::OutWalkTreeCounter()
{
    walkTreeCounter(m_pRoot, "");

#ifdef _DEBUG
    for (std::vector<std::pair<std::string, int>>::iterator it = m_freq.begin(); it != m_freq.end(); ++it)
    {
        DebugPrint("%s -- %d\n", it->first.c_str(), it->second);
    }
#endif  //_DEBUG

}