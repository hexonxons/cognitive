#include "SuffixTrie.h"
#include <iostream>
#include <fstream>

int main()
{
    CSuffixTrie tree;
    //std::fstream fileIn("news", std::ios::in);
    //std::string str = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    std::string str = "aabaabaab*";
    tree.Init(str);
    tree.BuildTrie();
    std::cout << tree.Present("<link rel=\"search\" title=\"Search L.O.R.\" href=\"/search.jsp\">");
    std::cout << tree.Present("baab");
    std::cout << tree.Present("braca");
    std::cout << tree.Present("aaaa");
    std::cout << tree.Present("baabab");
    std::cout << tree.Present("aaaaabaaaaaaabaaaaaaaaabcbadsfgahgshfh");
    return 0;
}