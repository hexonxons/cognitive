/*
 *  suffixtrie.h
 *  suffixtrie.cpp prototypes.
 *  24 October 2011
 *  Hexonxonx
 */

#ifndef __SUFFIXTRIE_H__
#define __SUFFIXTRIE_H__

#pragma warning( disable : 4018)

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

private:
    struct ltstr
    {
        bool operator()(const vector<pair<int, int>> &left, const vector<pair<int, int>> &right) const
        {
            return left.size() > right.size();
        }
    };

public:

    typedef int (*Compare)(const Y &pElem1, const Y &pElem2);

    struct CSuffixDescription
    {
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
        // суффиксна€ ссылка
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
        buildSuffixTree();
    }

    virtual ~CTrie()
    {
        m_nodeStorage.~CBasicDataBase();
        m_stateStorage.~CBasicDataBase();
    }

    vector< vector<pair<int, int>>> GetRanges(int minLength, int minFreq)
    {
        buidFreqArrays(m_pRoot, 0);
        getPossibleRanges(m_pRoot, minLength, minFreq);
        sort(neededSubstrings.begin(), neededSubstrings.end(), ltstr());
        return neededSubstrings;
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

    void buildSuffixTree()
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

    void buidFreqArrays(CSuffixNode *node, int size)
    {
        int offset = size;

        // если не root
        if (node->m_start != -1)
            size +=  node->m_end + 1 - node->m_start;

        // если детей нет
        if (node->m_children.size() == 0)
        {
            // начало - конец подстроки
            node->m_description.m_wordset.push_back(make_pair(node->m_start - offset, node->m_end));
            node->m_description.m_realwordset.push_back(make_pair(node->m_start - offset, node->m_end));
            return;
        }

        // иначе проходим по всем дет€м
        for (int i = 0; i < node->m_children.size(); ++i)
        {
            buidFreqArrays(node->m_children[i], size);
            // записываем начала - концы подстрок в реальной строке
            for (std::vector<std::pair<int, int>>::iterator it = node->m_children[i]->m_description.m_wordset.begin();
                                                            it != node->m_children[i]->m_description.m_wordset.end(); 
                                                            ++it)
            {
                node->m_description.m_wordset.push_back(make_pair(it->first, it->second + node->m_children[i]->m_start - node->m_children[i]->m_end - 1));
            }
        }
        // сортируем пары начал - концов
        sort(node->m_description.m_wordset.begin(), node->m_description.m_wordset.end(), pred());

        // выкидываем пересечени€
        std::vector<std::pair<int, int>>::iterator it = node->m_description.m_wordset.begin();
        node->m_description.m_realwordset.push_back(*it);
        ++it;
        while(it != node->m_description.m_wordset.end())
        {
            if (!(it->first >= node->m_description.m_realwordset.back().first && it->first <= node->m_description.m_realwordset.back().second))
                node->m_description.m_realwordset.push_back(*it);
            ++it;
        }
    }

    void getPossibleRanges(CSuffixNode *node, int minWordLength, int minWordFreq)
    {
        // если частота встречи текущего нода меньше минимальной, то просматривать детей нет смысла
        if (node->m_description.m_realwordset.size() < minWordFreq)
            return;
        
        // если длина подстроки больше необходимой, то сохран€ем подстроку
        if (node->m_description.m_realwordset[0].second - node->m_description.m_realwordset[0].first + 1 >= minWordLength)
        {
            neededSubstrings.push_back(node->m_description.m_realwordset);
        }

        // запускаем по дет€м
        for (int i = 0; i < node->m_children.size(); ++i)
        {
            getPossibleRanges(node->m_children[i], minWordLength, minWordFreq);
        }
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

    // текст, дл€ которого строитс€ суффиксное дерево
    T m_text;
    int m_textLength;
    Compare m_compareFunction;
    vector< vector<pair<int, int>>> neededSubstrings;
};

#endif  //__SUFFIXTRIE_H__
