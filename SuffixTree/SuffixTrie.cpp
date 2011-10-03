#include "SuffixTrie.h"
#include <string>

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
        return text.substr(start, end);
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

bool CSuffixTrie::findStr(std::string data, CSuffixNode *node, int &curPos)
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
                return true;
        }
        if (flag)
            return findStr(data, node->children[i], curPos);
    }
    return false;
}

bool CSuffixTrie::find(std::string data)
{
    int count = 0;
    return findStr(data, root, count);
}

void CSuffixTrie::buildSuffixTree()
{
    if(root == NULL)
    {
        root = new CSuffixNode(text);
        // Ссылка на самого себя
        root->link = root;
    }

    CSuffixNode *u = root;
    CSuffixNode *v = root;
    CState *state = new CState();		

    // Обрабатываем каждый суффикс
    for(int i = 0; i < text.length(); ++i)
    {
        CSuffixNode *s = u->link;

        int uvLen = v->pathlen - u->pathlen;

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
            fastscan(state, s, uvLen, j);
        }

        //establish the suffix link with v
        CSuffixNode *w = state->link;
        v->link = w;

        //execute slow scan
        if(!state->finished)
        {
            j = state->j;
            state->parent = w; //w must be an internal node when state->finished=false, then it must have a suffix link, so u can be updated.
            slowscan(state, w, j);
        }		

        u = state->parent;
        v = state->prefix;
    }
}

void CSuffixTrie::slowscan(CState *state, CSuffixNode *currNode, int j)
{
    bool done = false;
    int keyLen = text.length() - j;
    for(int i = 0; i < currNode->children.size(); ++i)
    {
        CSuffixNode *child = currNode->children[i];

        //use min(child->key.length, key.length)
        int childKeyLen = child->getLength();
        int len = childKeyLen < keyLen ? childKeyLen:keyLen;
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
                CSuffixNode *node = new CSuffixNode(text,j,text.length()-1,pathlen);
                currNode->children.insert(currNode->children.begin() + i,node);
                //state->u = currNode; //currNode is already registered as state->u, so commented out
                state->prefix = currNode;
                state->finished = true;
                done = true;
                break;
            }
            else
            { 
                //key.charAt(0)>child->key.charAt(0)
                //don't forget to add the largest new key after iterating all children
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
                    state->parent = child;
                    j += childKeyLen;
                    state->j = j;
                    slowscan(state,child,j);
                }
            }
            else
            {
                //0<delta<len
                //e.g. child="abc"
                //	   abc                     ab
                //    /  \     ==========>     / \
                //   e    f   insert "abd^"   c  d^
                //                           /  \
                //                          e    f
                //insert the new node: ab
                int nodepathlen = child->pathlen - (child->getLength() - delta);
                CSuffixNode *node = new CSuffixNode(text, child->start, child->start + delta - 1, nodepathlen);
                //node->children = new LinkedList<CSuffixNode>();
                node->cnt = child->cnt + 1;

                int tailpathlen = (text.length() - (j + delta)) + nodepathlen;
                CSuffixNode *tail = new CSuffixNode(text, j + delta, text.length() - 1, tailpathlen);

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

                //state->u = currNode; //currNode is already registered as state->u, so commented out
                state->prefix = node;
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
        //state->u = currNode; //currNode is already registered as state->u, so commented out
        state->prefix = currNode;
        state->finished = true;
    }
}


void CSuffixTrie::fastscan(CState *state,CSuffixNode *currNode,int uvLen,int j)
{		  
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
                state->parent = child;
                state->link = child;
                state->j = j+len;
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
                    int nodepathlen = child->pathlen - (child->getLength()-uvLen);
                    CSuffixNode *node = new CSuffixNode(text, child->start, child->start + uvLen - 1, nodepathlen);

                    int tailpathlen = (text.length() - (j + uvLen)) + nodepathlen;
                    CSuffixNode *tail = new CSuffixNode(text, j + uvLen, text.length() - 1, tailpathlen);
                    node->cnt = child->cnt + 1;
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

                    //uvLen = 0;
                    //state->u = currNode; //currNode is already registered as state->u, so commented out
                    state->link = node;
                    state->finished = true;
                    state->prefix = node;					

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
                    uvLen -= len;
                    state->parent = child;
                    j += len;
                    state->j = j;
                    fastscan(state,child,uvLen,j);
                }
            }
            break;
        }
    }
}

//#########################################################################################