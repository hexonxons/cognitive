#include "SuffixTrie.h"
#include <iostream>
#include <fstream>

int main()
{
    CSuffixTrie tree("aabaabaab*");
    tree.buildSuffixTree();
    //std::fstream fileIn("news", std::ios::in);
    //std::string str = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    return 0;
}