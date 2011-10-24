/*
 *  main.cpp
 *  main function for testing suffix tree class.
 *  24 October 2011
 *  Hexonxonx
 */

#include <iostream>
#include <fstream>
#include <string>

#include "suffixtrie.h"


using namespace std;

int main()
{
    std::fstream fileIn("news", std::ios::in);
    std::string str = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    str.push_back('*');
    CTrie tree(str);
    tree.BuildSuffixTree();

    for (int i = 0; i < str.length(); ++i)
    {
        for (int j = i + 1; j <= str.length(); ++j)
        {
            string fnd = str.substr(i, j - i);
            cout << fnd << " -- " << tree.Find(fnd) << "\n";
        }
    }

    cout << "\n\nFreq section\n\n";

    tree.OutWalkTreeCounter();
    return 0;
}