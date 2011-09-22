#include "SuffixTrie.h"
#include <string>

using namespace std;

CSuffixTrie::link::link()
{
    to = -1;
    start = -1;
    end = -1;
    let = 0;
}

CSuffixTrie::link::link(int _start, int _end, int _to, unsigned char _let)
{
    start = _start;
    end = _end;
    to = _to;
    let = _let;
}

CSuffixTrie::node::node()
{
    //links.assign(256, link());
    suffix = -1;
}

CSuffixTrie::CSuffixTrie(void)
:   inf(std::numeric_limits<int>::max())
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
    tree[from].links[iToUc(start)] = link(start, end, to, iToUc(start));
}

int &CSuffixTrie::getSuffix(int v)
{
    return tree[v].suffix;
}

void CSuffixTrie::Init(string data)
{
    text = data;
    tree.clear();
    dummy = createNewNode();
    root = createNewNode();

    getSuffix(root) = dummy;
    //for(int i = 0; i < 256; ++i)
       // createLink(dummy, -i - 1, -i, root);
}

void CSuffixTrie::BuildTrie()
{
    pair<int, int> activePoint = make_pair(root, 0);
    for(int i = 0; i < text.length(); ++i)
    {
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
        link cur = tree[vertex].links[iToUc(start)];
        while(end - start >= cur.end - cur.start)
        {   
            start += cur.end - cur.start;
            vertex = cur.to;
            if(end > start)
                cur = tree[vertex].links[iToUc(start)];
        }
        return make_pair(vertex, start);
    }
}

pair<int, int> CSuffixTrie::update(int vertex, int start, int end)
{
    link cur = tree[vertex].links[iToUc(start)];
    pair<bool, int> splitRes;
    int oldR = root;

    splitRes = testAndSplit(vertex, start, end, iToUc(end));
    while(!splitRes.first)
    {
        // Add a new branch
        createLink(splitRes.second, end, text.length(), createNewNode());

        // Create a suffix link from the prev. branching vertex
        if(oldR != root)
            getSuffix(oldR) = splitRes.second;
        oldR = splitRes.second;

        // Go to the next vertex (in the final set of STrie(T_end))
        pair<int, int> newPoint = canonize(getSuffix(vertex), start, end);
        vertex = newPoint.first;
        start = newPoint.second;
        splitRes = testAndSplit(vertex, start, end, iToUc(end));
    }
    if(oldR != root)
        getSuffix(oldR) = splitRes.second;
    return make_pair(vertex, start);
}

pair<bool, int> CSuffixTrie::testAndSplit(int vertex, int start, int end, unsigned char c)
{
    if(end <= start)
    {
        return make_pair(tree[vertex].links[c].to != -1, vertex);
    }
    else
    {
        link cur = tree[vertex].links[iToUc(start)];
        if(c == iToUc(cur.start + end - start))
            return make_pair(true, vertex);

        int middle = createNewNode();
        createLink(vertex, cur.start, cur.start + end - start, middle);
        createLink(middle, cur.start + end - start, cur.end, cur.to);
        return make_pair(false, middle);
    }
}
