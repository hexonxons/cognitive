#include "SuffixTrie.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    CSuffixTrie tree("aabaabaab*");
    tree.buildSuffixTree();
    //std::fstream fileIn("news", std::ios::in);
    //std::string str = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    
    cout << tree.find("aab");
    cout << tree.find("dsfhdfhdfhsdfhsfdhsdfhsdfhsfdhsfdhsdf");
    cout << tree.find("baab");
    cout << tree.find("aba");
    cout << tree.find("aabaa");
    cout << tree.find("aabaabaaf");

    return 0;
}