#pragma once
#include <vector>

class CSuffixTrie
{
public:
    CSuffixTrie(std::string _text)
    {
        this->text = _text;
        root = NULL;
    }
    ~CSuffixTrie();

    struct SuffixNode
    {
        SuffixNode(std::string text,int start, int end, int pathlen)
        {
            this->text = text;
            this->start = start;
            this->end = end;
            this->pathlen = pathlen;
        }

        SuffixNode(std::string text)
        {
            this->text = text;
            this->start = -1;
            this->end = -1;
            this->pathlen = 0;
        }

        int getLength()
        {
            if(start == -1) 
                return 0;
            else 
                return end - start + 1;
        }

        std::string getString()
        {
            if(start != -1)
                return this->text.substr(start, end + 1);
            else
                return "";
        }

        bool isRoot()
        {
            return start == -1;
        }

        std::string text;
        std::vector<SuffixNode *> children;
        SuffixNode *link;
        int start;
        int end;
        int pathlen;
    };


    struct State
    {
        SuffixNode *u; //parent(head)
        SuffixNode *w; //s(head[i-1])
        SuffixNode *v; //head[i-1]
        int j; //the global index of text starting from 0 to text.length()
        bool finished; //is this suffix insertion finished?
    };

    SuffixNode *root;
    std::string text;

    void buildSuffixTree()
    {
        if(root == NULL)
        {
            root = new SuffixNode(text);
            root->link = root; //link to itself
        }

        SuffixNode *u = root;
        SuffixNode *v = root;
        State *state = new State;		

        for(int i = 0; i < text.length(); ++i)
        {
            //process each suffix

            SuffixNode *s = u->link;

            int uvLen = v->pathlen - u->pathlen;

            if(u->isRoot() && !v->isRoot())
            {
                uvLen--;
            }
            int j = s->pathlen + i;		

            //init state
            state->u = s;
            state->w = s; //if uvLen = 0
            state->v = s;
            state->j = j;
            state->finished = false;

            //execute fast scan
            if(uvLen > 0) 
            {
                fastscan(state, s, uvLen, j);
            }

            //establish the suffix link with v
            SuffixNode *w = state->w;
            v->link = w;

            //execute slow scan
            if(!state->finished)
            {
                j = state->j;
                state->u = w; //w must be an internal node when state->finished=false, then it must have a suffix link, so u can be updated.
                slowscan(state, w, j);
            }		

            u = state->u;
            v = state->v;
        }
    }

    //slow scan until head(=state->v) is found
    void slowscan(State *state, SuffixNode *currNode, int j)
    {
        bool done = false;
        int keyLen = text.length() - j;
        for(int i=0;i<currNode->children.size();i++)
        {
            SuffixNode *child = currNode->children[i];

            //use min(child->key.length, key.length)
            int childKeyLen = child->getLength();
            int len = childKeyLen<keyLen?childKeyLen:keyLen;
            int delta = 0;
            for(;delta<len;delta++){
                if(text[j+delta] != text[child->start+delta]){
                    break;
                }
            }
            if(delta==0){//this child doesn't match	any character with the new key
                //order keys by lexi-order
                if(text[j] < text[child->start])
                {
                    //e.g. child="e" (currNode="abc")
                    //	   abc                     abc
                    //    /  \    =========>      / | \
                    //   e    f   insert "c^"    c^ e  f
                    int pathlen = text.length() - j + currNode->pathlen;
                    SuffixNode *node = new SuffixNode(text,j,text.length()-1,pathlen);
                    currNode->children.insert(currNode->children.begin() + i,node);
                    //state->u = currNode; //currNode is already registered as state->u, so commented out
                    state->v = currNode;
                    state->finished = true;
                    done = true;
                    break;
                }
                else
                { //key.charAt(0)>child->key.charAt(0)
                    //don't forget to add the largest new key after iterating all children
                    continue;
                }
            }
            else
            {//current child's key partially matches with the new key
                if(delta==len){
                    if(keyLen>childKeyLen){ //suffix tree with ^ ending can't have other two cases
                        //e.g. child="ab"
                        //	   ab                      ab
                        //    /  \    ==========>     / | \
                        //   e    f   insert "abc^"  c^ e  f
                        //recursion
                        state->u = child;
                        j += childKeyLen;
                        state->j = j;
                        slowscan(state,child,j);
                    }
                }
                else
                {//0<delta<len 

                    //e.g. child="abc"
                    //	   abc                     ab
                    //    /  \     ==========>     / \
                    //   e    f   insert "abd^"   c  d^
                    //                           /  \
                    //                          e    f
                    //insert the new node: ab
                    int nodepathlen = child->pathlen
                        - (child->getLength()-delta);
                    SuffixNode *node = new SuffixNode(text,child->start,child->start + delta - 1,nodepathlen);
                    //node->children = new LinkedList<SuffixNode>();

                    int tailpathlen = (text.length() - (j + delta)) + nodepathlen;
                    SuffixNode *tail = new SuffixNode(text, j+delta,text.length()-1,tailpathlen);

                    //update child node: c
                    child->start += delta;
                    if(text[j+delta]<text[child->start]){
                        node->children.push_back(tail);
                        node->children.push_back(child);
                    }else{
                        node->children.push_back(child);
                        node->children.push_back(tail);
                    }
                    //update parent
                    currNode->children[i] = node;

                    //state->u = currNode; //currNode is already registered as state->u, so commented out
                    state->v = node;
                    state->finished = true;
                }
                done = true;
                break;
            }
        }
        if(!done){
            int pathlen = text.length() - j + currNode->pathlen;
            SuffixNode *node = new SuffixNode(text,j,text.length()-1,pathlen);
            currNode->children.push_back(node);
            //state->u = currNode; //currNode is already registered as state->u, so commented out
            state->v = currNode;
            state->finished = true;
        }
    }
    //fast scan until w is found
    void fastscan(State *state,SuffixNode *currNode,int uvLen,int j)
    {		  

        for(int i=0;i<currNode->children.size();i++){
            SuffixNode *child = currNode->children[i];

            if(text[child->start] == text[j]){
                int len = child->getLength();
                if(uvLen==len){
                    //then we find w
                    //uvLen = 0;
                    //need slow scan after this child
                    state->u = child;
                    state->w = child;
                    state->j = j+len;
                }else if(uvLen<len){
                    //branching	and cut child short
                    //e.g. child="abc",uvLen = 2
                    //	   abc                          ab
                    //    /  \    ================>     / \
                    //   e    f   suffix part: "abd^"  c   d^
                    //                                /  \
                    //                               e    f				

                    //insert the new node: ab; child is now c
                    int nodepathlen = child->pathlen
                        - (child->getLength()-uvLen);
                    SuffixNode *node = new SuffixNode(text,
                        child->start,child->start + uvLen - 1,nodepathlen);

                    int tailpathlen = (text.length() - (j + uvLen)) + nodepathlen;
                    SuffixNode *tail = new SuffixNode(text,
                        j+uvLen,text.length()-1,tailpathlen);

                    //update child node: c
                    child->start += uvLen;
                    if(text[j+uvLen]<text[child->start]){
                        node->children.push_back(tail);
                        node->children.push_back(child);
                    }else{
                        node->children.push_back(child);
                        node->children.push_back(tail);
                    }

                    //update parent
                    currNode->children[i] = node;

                    //uvLen = 0;
                    //state->u = currNode; //currNode is already registered as state->u, so commented out
                    state->w = node;
                    state->finished = true;
                    state->v = node;					

                }else{//uvLen>len
                    //e.g. child="abc", uvLen = 4
                    //	   abc
                    //    /  \    ================>
                    //   e    f   suffix part: "abcdefg^"
                    //
                    //
                    //jump to next node
                    uvLen -= len;
                    state->u = child;
                    j += len;
                    state->j = j;
                    fastscan(state,child,uvLen,j);
                }
                break;
            }
        }
    }
};
