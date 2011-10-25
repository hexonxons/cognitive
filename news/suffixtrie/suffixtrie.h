/*
 *  suffixtrie.h
 *  suffixtrie.cpp prototypes.
 *  24 October 2011
 *  Hexonxonx
 */

#ifndef __SUFFIXTRIE_H__
#define __SUFFIXTRIE_H__

#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/memalloc.h"
#include "../include/debug.h"

using std::vector;
using std::make_pair;
using std::pair;

// class T - сам массив
// class Y - элемент массива
// у класса T должен существовать оператор [] и метод size()
template<class T, class Y> class CTrie
{
public:

    typedef int (*Compare)(const Y &pElem1, const Y &pElem2);

    struct CSuffixDescription
    {
#ifdef _DEBUG
        T m_suffix;
#endif
        int count;
        vector<pair<int, int>> m_wordset;
        vector<pair<int, int>> m_realwordset;
    };

    struct CSuffixNode
    {
        CSuffixNode(int start, int end, int pathlen)
        {
            this->m_start = start;
            this->m_end = end;
            this->m_pathlen = pathlen;
        }

        CSuffixNode()
        {
            this->m_start = -1;
            this->m_end = -1;
            this->m_pathlen = 0;
        }

        int GetLength()
        {
            if(m_start == -1)
                return 0;
            else 
                return m_end - m_start + 1;
        }

        bool IsRoot()
        {
            return m_start == -1;
        }


        // дети текущего нода
        vector<CSuffixNode *> m_children;
        // суффиксная ссылка
        CSuffixNode *m_pLink;
        // начало суффикса
        int m_start;
        // конец суффикса
        int m_end;
        // глубина всего суффикса
        int m_pathlen;

        CSuffixDescription m_description;
    };

    struct CState
    {
        CSuffixNode *u;
        CSuffixNode *w;
        CSuffixNode *v; 
        int j;
        bool finished;
    };

    CTrie(T str, int textLength, Compare compareFunction)
    {
        m_text = str;
        m_pRoot = m_nodeStorage.Add(CSuffixNode());
        m_pRoot->m_pLink = m_pRoot;
        m_textLength = textLength;
        m_compareFunction = compareFunction;
    }

    virtual ~CTrie()
    {
        m_nodeStorage.~CBasicDataBase();
        m_stateStorage.~CBasicDataBase();
    }

    void BuildSuffixTree()
    {
        CSuffixNode *u = m_pRoot;
        CSuffixNode *v = m_pRoot;
        CState *state = m_stateStorage.Add(CState());

        for(int i = 0; i < m_textLength; ++i)
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

    void OutWalkTreeCounter()
    {
        walkTreeCounter(m_pRoot, 0);

//#ifdef _DEBUG
//        for (vector<pair<T, int>>::iterator it = m_freq.begin(); it != m_freq.end(); ++it)
//        {
//            DebugPrint("m_freq : %s -- %d\n", it->first.c_str(), it->second);
//        }
//
//        for (vector<pair<T, int>>::iterator it = m_realfreq.begin(); it != m_realfreq.end(); ++it)
//        {
//            DebugPrint("m_realfreq : %s -- %d\n", it->first.c_str(), it->second);
//        }
//#endif  //_DEBUG

    }

    int Find(const T &str)
    {
        int count = 0;
        return findStr(str, m_pRoot, count);
    }

private:

    CTrie();

    struct pred
    {
        bool operator () (const pair<int, int> &left, const pair<int, int> &right) const
        {
            if (left.first == right.first)
            {
                return left.second < right.second;
            }
            return left.first < right.first;
        }
    };

    void slowscan(CState *state, CSuffixNode *currnode, int j)
    {
        bool done = false;
        int keylen = m_textLength - j;

        for(int i = 0; i < currnode->m_children.size(); ++i)
        {
            CSuffixNode *child = currnode->m_children[i];

            int childkeylen = child->GetLength();
            int len = childkeylen < keylen ? childkeylen : keylen;
            int delta = 0;

            for(; delta < len; ++delta)
            {
                // m_text[j + delta] == m_text[child->m_start + delta]
                if(m_compareFunction(m_text[j + delta], m_text[child->m_start + delta]) != 0)
                    break;
            }

            if(delta == 0)
            {
                // m_text[j] < m_text[child->m_start]
                if(m_compareFunction(m_text[j], m_text[child->m_start]) == -1)
                {
                    int pathlen = m_textLength - j + currnode->m_pathlen;
                    CSuffixNode *node = m_nodeStorage.Add(CSuffixNode(j, m_textLength - 1, pathlen));

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

                    int tailpathlen = (m_textLength - (j + delta)) + nodepathlen;
                    CSuffixNode *tail = m_nodeStorage.Add(CSuffixNode(j + delta, m_textLength - 1, tailpathlen));

                    child->m_start += delta;

                    // m_text[j + delta] < m_text[child->m_start]
                    if(m_compareFunction(m_text[j + delta], m_text[child->m_start]) == -1)
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
            int pathlen = m_textLength - j + currnode->m_pathlen;
            CSuffixNode *node = m_nodeStorage.Add(CSuffixNode(j, m_textLength - 1, pathlen));

            currnode->m_children.push_back(node);
            state->v = currnode;
            state->finished = true;
        }
    }

    void fastscan(CState *state, CSuffixNode *currnode, int uvlen, int j)
    {		  
        for(int i = 0; i < currnode->m_children.size(); ++i)
        {
            CSuffixNode *child = currnode->m_children[i];

            //m_text[child->m_start] == m_text[j]
            if(m_compareFunction(m_text[child->m_start], m_text[j]) == 0)
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

                        int tailpathlen = (m_textLength - (j + uvlen)) + nodepathlen;
                        CSuffixNode *tail = m_nodeStorage.Add(CSuffixNode(j + uvlen, m_textLength - 1, tailpathlen));

                        child->m_start += uvlen;

                        // m_text[j + uvlen] < m_text[child->m_start]
                        if(m_compareFunction(m_text[j + uvlen], m_text[child->m_start]) == -1)
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

    int walkTreeCounter(CSuffixNode *node, int size)
    {
        int counter = 0;
        int offset = size;

        /*if (node->m_start != -1)
        {
            curText += string(m_text, node->m_start, node->m_end - node->m_start + 1);
        }*/
        if (node->m_start != -1)
            size +=  node->m_end + 1 - node->m_start;

        // если детей нет - встретилось 1 раз
        if (node->m_children.size() == 0)
        {
            //m_freq.push_back(make_pair(curText, 1));
#ifdef _DEBUG
            //node->m_description.m_suffix = curText;
#endif
            node->m_description.count = 1;
            node->m_description.m_wordset.push_back(make_pair(node->m_start - offset, node->m_end));
           // m_realfreq.push_back(make_pair(curText, 1));
            return 1;
        }

        // иначе складываем степень всех детей
        for (int i = 0; i < node->m_children.size(); ++i)
        {
            counter += walkTreeCounter(node->m_children[i], size);
            for (std::vector<std::pair<int, int>>::iterator it = node->m_children[i]->m_description.m_wordset.begin();
                it != node->m_children[i]->m_description.m_wordset.end(); 
                ++it)
            {
                node->m_description.m_wordset.push_back(make_pair(it->first, it->second + node->m_children[i]->m_start - node->m_children[i]->m_end - 1));
            }
        }

       // m_freq.push_back(make_pair(curText, counter));
        node->m_description.count = counter;

#ifdef _DEBUG
        //node->m_description.m_suffix = curText;
#endif

        sort(node->m_description.m_wordset.begin(), node->m_description.m_wordset.end(), pred());

        std::vector<std::pair<int, int>>::iterator it = node->m_description.m_wordset.begin();
        node->m_description.m_realwordset.push_back(*it);
        ++it;
        while(it != node->m_description.m_wordset.end())
        {
            if (!(it->first >= node->m_description.m_realwordset.back().first && it->first <= node->m_description.m_realwordset.back().second))
                node->m_description.m_realwordset.push_back(*it);
            ++it;
        }
        //m_realfreq.push_back(make_pair(curText, node->m_description.m_realwordset.size()));

        return counter;
    }

    int findStr(const T &data, CSuffixNode *node, int &curpos)
    {
        bool flag = false;
        for (int i = 0; i < node->m_children.size(); ++i)
        {
            int ind = 0;
            // node->m_children[i]->m_start + ind] == data[curpos]
            while(m_compareFunction(m_text[node->m_children[i]->m_start + ind], data[curpos]) == 0)
            {
                ++curpos;
                ++ind;
                flag = true;
                if (curpos == data.size())
                {
                    return 1;
                }
            }
            if (flag)
                return findStr(data, node->m_children[i], curpos);
        }
        return 0;
    }

    //  корень дерева
    CSuffixNode *m_pRoot;
    // аллокаторы
    CBasicDataBase<CSuffixNode> m_nodeStorage;
    CBasicDataBase<CState> m_stateStorage;

    // текст, для которого строится суффиксное дерево
    T m_text;
    // массив частот встреч подстрок и частот встреч подстрок с учетом пересечения
    //vector<pair<T, int>> m_freq;
   // vector<pair<T, int>> m_realfreq;
    int m_textLength;
    Compare m_compareFunction;
};

#endif  //__SUFFIXTRIE_H__
