#include "SuffixTrie.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    //std::fstream fileIn("news", std::ios::in);
    //std::string str = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    //str.push_back((char)(16));
    //CSuffixTrie tree(str);
    CSuffixTrie tree("aabaabaab*");
    tree.buildSuffixTree();

    cout << tree.find("aab");
    cout << tree.find("baab");
    cout << tree.find("<META NAME=\"Description\" CONTENT=\"Все о Linux на русском языке\">");
    cout << tree.find("<div");
    cout << tree.find("aabaa");
    cout << tree.find("<li><a");

    return 0;
}