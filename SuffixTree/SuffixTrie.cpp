#include "SuffixTrie.h"
#include <string>
#include <iostream>

using namespace std;

//################################### CSuffixNode ##########################################

CSuffixTrie::CSuffixNode::CSuffixNode(string &text, int start, int end, int pathlen)
{
    this->start = start;
    this->end = end;
    this->pathlen = pathlen;
    this->cnt = 1;
    curtext = getString(text);
}

CSuffixTrie::CSuffixNode::CSuffixNode(string &text)
{
    this->start = -1;
    this->end = -1;
    this->pathlen = 0;
    this->cnt = 1;
}

int CSuffixTrie::CSuffixNode::getLength()
{
    if(start == -1) 
        return 0;
    else 
        return end - start + 1;
}

string CSuffixTrie::CSuffixNode::getString(string &text)
{
    if(start != -1)
    {
        return text.substr(start, end - start + 1);
    }
    else
        return "";
}

bool CSuffixTrie::CSuffixNode::isRoot()
{
    return start == -1;
}

//################################### CState ##############################################

CSuffixTrie::CState::CState()
{
    parent = NULL;
    link = NULL;
    prefix = NULL;
    transLink = NULL;
}

//################################### CSuffixTrie #########################################

CSuffixTrie::CSuffixTrie(string _text)
{
    text = _text;
    root = NULL;
}

CSuffixTrie::~CSuffixTrie()
{
    delete root;
}

int CSuffixTrie::findStr(std::string data, CSuffixNode *node, int &curPos)
{
    bool flag = false;
    for (int i = 0; i < node->children.size(); ++i)
    {
        int ind = 0;
        while(node->children[i]->curtext[ind] == data[curPos])
        {
            ++curPos;
            ++ind;
            flag = true;
            if (curPos == data.length())
            {
                return node->children[i]->cnt;
            }
        }
        if (flag)
            return findStr(data, node->children[i], curPos);
    }
    return false;
}

void CSuffixTrie::walkTree(CSuffixTrie::CSuffixNode *node, int len)
{
    for (int i = 0; i < node->children.size(); ++i)
    {
        int curLen = len;
        CSuffixNode *curNode = node->children[i];
        curLen += curNode->curtext.length();
        if (curLen != curNode->pathlen)
        {
            std::cout << "Wrong pathlen on string " << (LPCTSTR)curNode->curtext.c_str() << "\n";
        }
        walkTree(curNode, curLen);
    }
}

void CSuffixTrie::OutWalkTree()
{
    walkTree(root, 0);
}

int CSuffixTrie::find(std::string data)
{
    int count = 0;
    return findStr(data, root, count);
}

void CSuffixTrie::buildSuffixTree()
{
    if(root == NULL)
    {
        root = new CSuffixNode(text);
        // —сылка на самого себ€
        root->link = root;
    }

    CSuffixNode *u = root;
    CSuffixNode *v = root;
    CState *state = new CState();		

    // ќбрабатываем каждый суффикс
    for(int i = 0; i < text.length(); ++i)
    {
        std::string insertVal = text.substr(i, text.size() - i);

        // суффиксна€ ссылка на предыдущий суффикс
        CSuffixNode *s = u->link;

        int uvLen;
        if (state->transLink != NULL)
        {
            uvLen = state->transLink->pathlen - u->pathlen;
        }
        else
            uvLen = v->pathlen - u->pathlen;
        // >>>
        bool isFastScanned = false;

        if(u->isRoot() && !v->isRoot())
        {
            uvLen--;
        }
        int j = s->pathlen + i;		

        //init state
        state->parent = s;
        state->link = s;    //if uvLen = 0
        state->prefix = s;
        state->j = j;
        state->finished = false;

        //execute fast scan
        if(uvLen > 0) 
        {
            if (s->pathlen != 0)
            {
                s->cnt++;
                s->start = i;
                s->end = i;
                s->curtext = s->getString(text);
            }
            fastscan(state, s, uvLen, j, i);
            isFastScanned = true;
        }

        //establish the suffix link with v
        // v - последн€€ вершина не нод
        CSuffixNode *w = state->link;
        v->link = w;

        //execute slow scan
        if(!state->finished)
        {
            j = state->j;
            state->parent = w; //w must be an internal node when state->finished=false, then it must have a suffix link, so u can be updated.
            // сдвиг индекса j на глубину w
            if (w->pathlen != 0)
                w->cnt++;
            slowscan(state, w, j, i);
        }		

        u = state->parent;
        v = state->prefix;
    }
}

void CSuffixTrie::slowscan(CState *state, CSuffixNode *currNode, int j, int insSubStrStart)
{
    bool done = false;
    std::string insertVal = text.substr(j, text.size() - j);

    int keyLen = text.length() - j;

    for(int i = 0; i < currNode->children.size(); ++i)
    { 
        CSuffixNode *child = currNode->children[i];

        int childKeyLen = child->getLength();
        int len = childKeyLen < keyLen ? childKeyLen : keyLen;

        //  оличество совпавших букв
        int delta = 0;
        for( ;delta < len; ++delta)
        {
            if(text[j + delta] != text[child->start + delta])
            {
                break;
            }
        }
        if(delta == 0)
        {
            //this child doesn't match	any character with the new key
            //order keys by lexi-order
            if(text[j] < text[child->start])
            {
                //e.g. child="e" (currNode="abc")
                //	   abc                     abc
                //    /  \    =========>      / | \
                //   e    f   insert "c^"    c^ e  f
                int pathlen = text.length() - j + currNode->pathlen;
                CSuffixNode *node = new CSuffixNode(text, j, text.length() - 1, pathlen);
                currNode->children.insert(currNode->children.begin() + i, node);
                state->prefix = currNode;
                state->finished = true;
                done = true;
                break;
            }
            else
            { 
                continue;
            }
        }
        else
        {
            //current child's key partially matches with the new key
            if(delta == len)
            {
                if(keyLen > childKeyLen)
                { 
                    //suffix tree with ^ ending can't have other two cases
                    //e.g. child="ab"
                    //	   ab                      ab
                    //    /  \    ==========>     / | \
                    //   e    f   insert "abc^"  c^ e  f
                    //recursion
                    
                    // провер€ем у детей, нет ли пересечени€
                    // например aabaaab*
                    // при вставке aab*  a -> a -> ab*
                    //                    / \->baaab*
                    // проверим в этом ноде   / наличие подстроки ab*
                    /*bool flag = false;
                    for (int i = 0; i < child->children.size(); ++i)
                    {
                        if (text.substr(child->children[i]->start, child->children[i]->end - child->children[i]->start + 1) == text.substr(j, text.size() - j))
                        {
                            flag = true;
                            break;
                        }
                    }*/
                    state->parent = child;
                    if (child->start < insSubStrStart)
                        child->cnt++;
                    child->start = j;
                    child->end = j + delta - 1;
                    child->curtext = child->getString(text);
                    j += childKeyLen;
                    state->j = j;
                    
                    slowscan(state, child, j, insSubStrStart);
                }
            }
            else
            {
                //0 < delta < len
                //e.g. child="abc"
                //	   abc                     ab
                //    /  \     ==========>     / \
                //   e    f   insert "abd^"   c  d^
                //                           /  \
                //                          e    f
                //insert the new node: ab
                int nodepathlen = -1;
                CSuffixNode *node = NULL;
                int tailpathlen = -1;
                CSuffixNode *tail = NULL;
                // ќбработка пересечений текста.
                // Ќапример:
                // ¬ строчке "aabaabaab*" последовательтность "abaab" встречаетс€ 1 раз, а не два.
                // 
                // (x) - количество повторов подстроки
                // [x] - pathlen
                // 
                //          ѕосле вставки "aabaab*" дерево будет выгл€деть не так:
                // 
                //              a(2)                                    a(3)[1]                               currNode
                //            /     \            ==========>          /        \           =========          /      \
                //    baabaab*(1)  abaabaab*(1)   вставл€ем          baabaab*(1)[9]   abaab(1)[6]                  ....    node
                //                                aabaab*                       /     \                            /    \
                //                                                         *(1)[7]   aab*(1)[10]                tail    child
                //                                        
                //          ј так:
                //                                                    
                //              a(2)                                    a(3)[1]                               currNode
                //            /     \            ==========>          /        \           =========          /      \
                //    baabaab*(1)  abaabaab*(1)   вставл€ем          baabaab*(1)[9]   aba(2)[4]                    ....    node
                //                                aabaab*                         |                                  |
                //                                                             ab(1)[6]                           transNode
                //                                                              /     \                            /    \
                //                                                         *(1)[7]   aab*(1)[10]                tail    child
                //                               
                // TXT :Ќе жаль реального, жаль иллюзорного...
                // 
                if( j >= child->start && j <= child->start + delta - 1)
                {

                    int transNodePathLen = -1;
                    CSuffixNode *transNode = NULL;
                    //            была глубина      -  длина куска слова сейчас   + конец куска получившегос€ слова  
                    nodepathlen = child->pathlen - child->getLength() + (j - child->start);
                    node = new CSuffixNode(text, child->start, j - 1, nodepathlen);
                    node->cnt = child->cnt + 1;
                    //                 глубина куска верхнего уровн€ + длина нового куска
                    transNodePathLen = nodepathlen + (delta - child->start);
                    transNode = new CSuffixNode(text, j, j + delta - child->start - 1, transNodePathLen);
                    if (transNodePathLen < 0)
                    {
                        int c = 0;
                    }
                    // ???
                    //transNode->link = node;
                    
                    tailpathlen = transNodePathLen + (text.length() - delta - j);
                    tail = new CSuffixNode(text, j + delta, text.length() - 1, tailpathlen);

                    node->children.push_back(transNode);

                    //update child node: c
                    child->start += delta;
                    child->curtext = child->getString(text);
                    //child->pathlen -= child->start - j; 

                    if(text[j + delta] < text[child->start])
                    {
                        transNode->children.push_back(tail);
                        transNode->children.push_back(child);
                    }
                    else
                    {
                        transNode->children.push_back(child);
                        transNode->children.push_back(tail);
                    }

                    //update parent
                    currNode->children[i] = node;

                    state->prefix = node;
                    state->finished = true;
                    state->transLink = transNode;
                }
                else
                {
                    nodepathlen = child->pathlen - (child->getLength() - delta);
                    //node = new CSuffixNode(text, child->start, child->start + delta - 1, nodepathlen);
                    node = new CSuffixNode(text, j, j + delta - 1, nodepathlen);
                    node->cnt = child->cnt + 1;

                    tailpathlen = (text.length() - (j + delta)) + nodepathlen;
                    tail = new CSuffixNode(text, j + delta, text.length() - 1, tailpathlen);    

                    //update child node: c
                    child->start += delta;
                    child->curtext = child->getString(text);

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

                    //update parent
                    currNode->children[i] = node;

                    state->prefix = node;
                    state->finished = true;
                }
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
        state->prefix = currNode;
        state->finished = true;
    }
}


void CSuffixTrie::fastscan(CState *state, CSuffixNode *currNode, int uvLen, int j, int insSubStrStart)
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
                //then we find w
                //uvLen = 0;
                //need slow scan after this child
                child->start = j;
                child->end = j + len - 1;
                child->curtext = child->getString(text);
                state->parent = child;
                state->link = child;
                state->j = j + len;
            }
            else 
            {
                if(uvLen < len)
                {
                    //branching	and cut child short
                    //e.g. child="abc",uvLen = 2
                    //	   abc                          ab
                    //    /  \    ================>     / \
                    //   e    f   suffix part: "abd^"  c   d^
                    //                                /  \
                    //                               e    f				

                    //insert the new node: ab; child is now c
                    int nodepathlen = -1;
                    CSuffixNode *node = NULL;
                    int tailpathlen = -1;
                    CSuffixNode *tail = NULL;

                    if( j >= child->start && j <= child->start + uvLen - 1)
                    {

                        int transNodePathLen = -1;
                        CSuffixNode *transNode = NULL;
                        //            была глубина      -  длина куска слова сейчас   + конец куска получившегос€ слова  
                        nodepathlen = child->pathlen - child->getLength() + (j - child->start);
                        node = new CSuffixNode(text, child->start, j - 1, nodepathlen);
                        node->cnt = child->cnt + 1;
                        //                 глубина куска верхнего уровн€ + длина нового куска
                        transNodePathLen = nodepathlen + (uvLen - child->start);
                        transNode = new CSuffixNode(text, j, j + uvLen - child->start - 1, transNodePathLen);
                        // ???
                        //transNode->link = node;
                        if (transNodePathLen < 0)
                        {
                            int c = 0;
                        }
                        tailpathlen = transNodePathLen + (text.length() - uvLen - j);
                        tail = new CSuffixNode(text, j + uvLen, text.length() - 1, tailpathlen);

                        node->children.push_back(transNode);

                        //update child node: c
                        child->start += uvLen;
                        child->curtext = child->getString(text);
                        //child->pathlen -= child->start - j; 

                        if(text[j + uvLen] < text[child->start])
                        {
                            transNode->children.push_back(tail);
                            transNode->children.push_back(child);
                        }
                        else
                        {
                            transNode->children.push_back(child);
                            transNode->children.push_back(tail);
                        }

                        //update parent
                        currNode->children[i] = node;

                        state->link = node;
                        state->finished = true;
                        state->prefix = node;
                        state->transLink->link = transNode;
                        state->transLink = NULL;
                    }
                    else
                    {
                        nodepathlen = child->pathlen - (child->getLength() - uvLen);
                        node = new CSuffixNode(text, child->start, child->start + uvLen - 1, nodepathlen);
                        node->cnt = child->cnt + 1;

                        tailpathlen = (text.length() - (j + uvLen)) + nodepathlen;
                        tail = new CSuffixNode(text, j + uvLen, text.length() - 1, tailpathlen);    

                        //update child node: c
                        child->start += uvLen;
                        child->curtext = child->getString(text);

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


                        //update parent
                        currNode->children[i] = node;

                        state->link = node;
                        state->finished = true;
                        state->prefix = node;		
                    }
			

                }
                else
                {
                    //uvLen>len
                    //e.g. child="abc", uvLen = 4
                    //	   abc
                    //    /  \    ================>
                    //   e    f   suffix part: "abcdefg^"
                    //
                    //
                    //jump to next node
                    
                    // провер€ем у детей, нет ли пересечени€
                    // например aabaaab*
                    // при вставке aab*  a -> a -> ab*
                    //                    / \->baaab*
                    // проверим в этом ноде   / наличие подстроки ab*
      /*              bool flag = false;
                    for (int i = 0; i < child->children.size(); ++i)
                    {
                        if (text.substr(child->children[i]->start, child->children[i]->end - child->children[i]->start + 1) == text.substr(j, text.size() - j))
                        {
                            flag = true;
                            break;
                        }
                    }*/
                    if (child->start < insSubStrStart)
                        child->cnt++;
                    uvLen -= len;
                    state->parent = child;
                    j += len;
                    state->j = j;
                  

                    fastscan(state, child, uvLen, j, insSubStrStart);
                }
            }
            break;
        }
    }
}

//#########################################################################################