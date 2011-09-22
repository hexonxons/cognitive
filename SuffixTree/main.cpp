#include "SuffixTrie.h"

int main()
{
    CSuffixTrie tree;
    tree.Init("abracadabra");
    tree.BuildTrie();
    return 0;
}