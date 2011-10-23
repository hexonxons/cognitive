#include "SuffixTrie.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    std::fstream fileIn("news", std::ios::in);
    std::string str = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    str.push_back('*');
    CTrie tree(str);
    tree.buildSuffixTree();
    tree.OutWalkTreeCounter();
    return 0;
}