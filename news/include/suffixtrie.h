/*
 *  suffixtrie.h
 *  24 October 2011
 *  Hexonxonx
 */

#ifndef __SUFFIXTRIE_H__
#define __SUFFIXTRIE_H__

#pragma warning( disable : 4018)

#include <iostream>
#include <vector>
#include <algorithm>

#include "memalloc.h"
#include "debug.h"
#include <map>

using std::vector;
using std::make_pair;
using std::pair;
using std::multimap;

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
            if(left.size() == right.size())
                if(left[0].first == right[0].first)
                    return left[0].second > right[0].second;
                else
                    return left[0].first < right[0].first;

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
        buildSuffixTree();
    }

    virtual ~CTrie()
    {
        m_nodeStorage.~CBasicDataBase();
        m_stateStorage.~CBasicDataBase();
    }

    multimap<int, vector< pair<int, int> > > GetRanges(int minLength, int minFreq)
    {
        buidFreqArrays(m_pRoot, 0);
        getPossibleRanges(m_pRoot, minLength, minFreq);
        return substrings;
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

        // иначе проходим по всем детям
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

        // выкидываем пересечения
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

    // проверка на включение одного диапазона в другой
    // return: -1, если left включается в right
    //          1, если наоборот
    //          0, если нет включения
    inline int checkIncluding(pair<int, int> &left, pair<int, int> &right)
    {
        // левая включается в правую
        if (left.first >= right.first && left.second <= right.second)
        {
            return -1;
        }

        // правая включается в левую
        if (left.first <= right.first && left.second >= right.second)
        {
            return 1;
        }

        return 0;
    }

    // проверка на пары начал-концов
    // return: -1, если left дополняет right
    //          1, если наоборот
    //          0, если нет дополнения
    inline int checkPair(pair<int, int> &left, pair<int, int> &right)
    {
        // левая пара - начало, правая - конец
        if (left.first > right.second && left.first > right.first)
        {
            return -1;
        }

        // правая пара - начало, левая - конец
        if (right.first > left.second && right.first > left.first)
        {
            return 1;
        }

        return 0;
    }

    // проверка на дополнение одного диапазона другим.
    // return: -1, если left дополняет right
    //          1, если right дополняет left
    //      
    inline int checkIntersection (pair<int, int> &left, pair<int, int> &right)
    {
        if (left.first <= right.first && left.second >= right.first && left.second < right.second)
        {
            return -1;
        }

        if (right.first <= left.first && right.second >= left.first && right.second < left.second)
        {
            return 1;
        }

        return 0;
    }

    void getPossibleRanges(CSuffixNode *node, int minWordLength, int minWordFreq)
    {
        // если частота встречи текущего нода меньше минимальной, то просматривать детей нет смысла
        if (node->m_description.m_realwordset.size() < minWordFreq)
            return;

        // запускаем по детям
        for (int i = 0; i < node->m_children.size(); ++i)
        {
            getPossibleRanges(node->m_children[i], minWordLength, minWordFreq);
        }

        // если длина подстроки больше необходимой, то сохраняем подстроку
        if (node->m_description.m_realwordset[0].second - node->m_description.m_realwordset[0].first + 1 >= minWordLength)
        {       
            // получаем все последовательности длины node->m_description.m_realwordset.size()
            pair<multimap<int, vector< pair<int, int> > >::iterator, multimap<int, vector< pair<int, int> > >::iterator> range = substrings.equal_range(node->m_description.m_realwordset.size());
            if (range.first == range.second)
            {
                 substrings.insert(pair<int, vector<pair<int, int>>>(node->m_description.m_realwordset.size(), node->m_description.m_realwordset));
            }
            else
            {
                bool flag = false;
                // проходим по всем элементам длины вставляемой последовательности
                for (multimap<int, vector< pair<int, int> > >::iterator it = range.first; it != range.second; ++it)
                {
                    // уже лежащее значение
                    vector<pair<int, int>> val = it->second;
                    // то, что мы хотим добавить
                    vector<pair<int, int>> newVal = node->m_description.m_realwordset;
                    // итератор по всем элементам лежащего значения
                    vector<pair<int, int>>::iterator vit = val.begin();
                    // итератор по всем элементам добавляемого значения
                    vector<pair<int, int>>::iterator nvit = newVal.begin();
                    bool pairs = true;
                    bool intersection = true;
                    bool including = true;

                    int resultPairs = checkPair(*vit, *nvit);
                    int resultIntersection = checkIntersection(*vit, *nvit);
                    int resultIncluding = checkIncluding(*vit, *nvit);

                    vector<pair<int, int>> valuesPairs;
                    vector<pair<int, int>> valuesIntersection;
                    vector<pair<int, int>> valuesIncluding;

                    // попарно проходим по всем элементам
                    while(vit != val.end() && nvit != newVal.end() && (resultPairs + resultIntersection + resultIncluding) != 0)
                    {
                        // проверяем на пары
                        if (resultPairs != 0)
                        {
                            // если у нас значение отличное от предыдушего, то больше этой проверки не выполняем
                            if(resultPairs != checkPair(*vit, *nvit))
                                resultPairs = 0;
                            else
                            {
                                pair<int, int> value = resultPairs == 1 ? make_pair(vit->first, nvit->second) : make_pair(nvit->first, vit->second);
                                if(valuesPairs.size() != 0 && valuesPairs[valuesPairs.size() - 1].second > value.first)
                                    resultPairs = 0;
                                else
                                    valuesPairs.push_back(value);
                            }
                        }

                        if (resultIntersection != 0)
                        {
                            // если у нас значение отличное от предыдушего, то больше этой проверки не выполняем
                            if(resultIntersection != checkIntersection(*vit, *nvit))
                                resultIntersection = 0;
                            else
                            {
                                pair<int, int> value = resultIntersection == -1 ? make_pair(vit->first, nvit->second) : make_pair(nvit->first, vit->second);
                                if(valuesIntersection.size() != 0 && valuesIntersection[valuesIntersection.size() - 1].second >= value.first)
                                    resultIntersection = 0;
                                else
                                    valuesIntersection.push_back(value);
                            }
                        }

                        if (resultIncluding != 0)
                        {
                            // если у нас значение отличное от предыдушего, то больше этой проверки не выполняем
                            if(resultIncluding != checkIncluding(*vit, *nvit))
                                resultIncluding = 0;
                            else
                            {
                                // если jt включается в it, то нам не интересно это значение
                                // иначе запоминаем
                                if (resultIncluding == -1)
                                {
                                    valuesIncluding.push_back(make_pair(nvit->first, nvit->second));
                                }
                                else
                                {
                                    flag = true;
                                    resultIncluding = 0;
                                }
                            }
                        }
                        ++vit;
                        ++nvit;
                    }    

                    if (valuesPairs.size() == val.size())
                    {
                        flag = true;
                        it->second = valuesPairs;
                    }

                    if (valuesIncluding.size() == val.size())
                    {
                        flag = true;
                        it->second = valuesIncluding;
                    }

                    if (valuesIntersection.size() == val.size())
                    {
                        flag = true;
                        it->second = valuesIntersection;
                    }
                }
                if(!flag)
                {
                    substrings.insert(pair<int, vector<pair<int, int>>>(node->m_description.m_realwordset.size(), node->m_description.m_realwordset));
                }
            }
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

    // текст, для которого строится суффиксное дерево
    T m_text;
    int m_textLength;
    Compare m_compareFunction;
    multimap<int, vector< pair<int, int> > > substrings;
};

#endif  //__SUFFIXTRIE_H__
