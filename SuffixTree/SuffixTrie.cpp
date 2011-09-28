#include "SuffixTrie.h"
#include <string>
#include <algorithm>

using namespace std;

CSuffixTrie::link::link()
{
    nLinkTo = -1;
    nWordStart = -1;
    nWordEnd = -1;
    letter = 255;
    count = 0;
}

CSuffixTrie::link::link(int _start, int _end, int _to, UCHAR _let, UINT _count, string _text)
{
    nWordStart = _start;
    nWordEnd = _end;
    nLinkTo = _to;
    letter = _let;
    count = _count;
    if (_start < _end && _start >= 0 && _end >= 0)
        word = _text.substr(_start, _end - _start);
}

CSuffixTrie::node::node()
{
    suffix = -1;
}

bool CSuffixTrie::node::cmp::operator () (first_argument_type CLink, second_argument_type letter) const
{
    return (CLink.letter == letter);
}

std::vector<CSuffixTrie::link>::iterator CSuffixTrie::node::findByLetter(UCHAR letter)
{
    if (links.size() == 0)
        return links.end();
    return find_if(links.begin(), links.end(), bind2nd(cmp(), letter));
}

CSuffixTrie::CSuffixTrie(void)
{
}

CSuffixTrie::~CSuffixTrie(void)
{
}

int CSuffixTrie::createNewNode()
{
    tree.push_back(node());
    return tree.size() - 1;
}

unsigned char CSuffixTrie::iToUc(int i)
{
    return (i < 0) ? (- i - 1) : text[i];
}

void CSuffixTrie::createLink(int from, int start, int end, int to)
{
    std::vector<link>::iterator cur = tree[from].findByLetter(iToUc(start));
    if (cur == tree[from].links.end())
    {
        tree[from].links.push_back(link(start, end, to, iToUc(start), 1, text));
        getSuffix(to) = from;
    }
    else
    {
        getSuffix(to) = from;
        cur->letter = iToUc(start);
        cur->nWordStart = start;
        cur->nWordEnd = end;
        cur->word = text.substr(start, end - start);
        cur->nLinkTo =  to;
        cur->count++;
    }
}

int &CSuffixTrie::getSuffix(int vertex)
{
    return tree[vertex].suffix;
}

void CSuffixTrie::Init(string data)
{
    text = data;
    tree.clear();
    dummy = createNewNode();
    root = createNewNode();

    getSuffix(root) = dummy;
    for(int i = 0; i < 256; ++i)
        createLink(dummy, -i - 1, -i, root);
}

void CSuffixTrie::BuildTrie()
{
    pair<int, int> activePoint = make_pair(root, 0);
    for(UINT i = 0; i < text.length(); ++i)
    {
        char a = text[i];
        activePoint = update(activePoint.first, activePoint.second, i);
        activePoint = canonize(activePoint.first, activePoint.second, i + 1);
    }
}

pair<int, int> CSuffixTrie::canonize(int vertex, int start, int end)
{
    if(end <= start)
    {
        return make_pair(vertex, start);
    } 
    else 
    {
        std::vector<link>::iterator cur = tree[vertex].findByLetter(iToUc(start));
        while(end - start >= cur->nWordEnd - cur->nWordStart)
        {   
            start += cur->nWordEnd - cur->nWordStart;
            vertex = cur->nLinkTo;
            if(end > start)
            {
                cur = tree[vertex].findByLetter(iToUc(start));
            }
        }
        return make_pair(vertex, start);
    }
}

pair<int, int> CSuffixTrie::update(int vertex, int start, int end)
{
    std::vector<link>::iterator cur = tree[vertex].findByLetter(iToUc(start));
    pair<bool, int> splitRes;

    splitRes = testAndSplit(vertex, start, end, iToUc(end));
    while(!splitRes.first)
    {
        createLink(splitRes.second, end, text.length(), createNewNode());

        pair<int, int> newPoint = canonize(getSuffix(vertex), start, end);
        vertex = newPoint.first;
        start = newPoint.second;
        splitRes = testAndSplit(vertex, start, end, iToUc(end));
    }
    return make_pair(vertex, start);
}

pair<bool, int> CSuffixTrie::testAndSplit(int vertex, int start, int end, unsigned char c)
{
    if(end <= start)
    {
        pair <bool, int> reti;
        std::vector<link>::iterator cur = tree[vertex].findByLetter(c);
        if (cur == tree[vertex].links.end())
            reti.first = 0;
        else
        {
            reti.first = cur->nLinkTo != -1;
            cur->count++;
        }
        reti.second = vertex;
        return reti;
    }
    else
    {
        std::vector<link>::iterator cur = tree[vertex].findByLetter(iToUc(start));
        if(c == iToUc(cur->nWordStart + end - start))
        {
            cur->count++;
            return make_pair(true, vertex);
        }

        int curEnd = cur->nWordEnd;
        int curStart = cur->nWordStart;
        int curLinkTo = cur->nLinkTo;

        int middle = createNewNode();
        getSuffix(middle) = vertex;
        createLink(vertex, curStart, curStart + end - start, middle);
        getSuffix(curLinkTo) = middle;
        createLink(middle, curStart + end - start, curEnd, curLinkTo);
        return make_pair(false, middle);
    }
}

bool CSuffixTrie::Present(string word)
{
    int vertex = root;
    int start = 0;
    int end = 0;
    for(UINT i = 0; i < word.length(); ++i) 
    {
        if(end == start)
        {
            std::vector<link>::iterator cur = tree[vertex].findByLetter((UCHAR)word[i]);
            if(cur == tree[vertex].links.end() ||  cur->nLinkTo == -1)
                return false;
            start = cur->nWordStart;
            end = start + 1;
        }
        else
        {
            if(word[i] != iToUc(end))
                return false;
            ++end;
        }

        std::vector<link>::iterator cur = tree[vertex].findByLetter(iToUc(start));
        if(end == cur->nWordEnd)
        {
            std::vector<link>::iterator rcur = tree[vertex].findByLetter(iToUc(start));
            vertex = rcur->nLinkTo;
            start= 0;
            end = 0;
        }
    }
    return true;
}