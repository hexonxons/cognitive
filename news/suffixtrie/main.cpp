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

int compare(const char &left, const char &right)
{
    if (left == right)
        return 0;
    if (left > right)
        return 1;
    else
        return -1;
}

int main()
{
   // std::fstream fileIn("news", std::ios::in);
   // std::string str = std::string((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    std::string str = "ababababab";
    str.push_back('*');
    CTrie<std::string, char> tree(str, str.length(), compare);

    //for (int i = 0; i < str.length(); ++i)
    //{
    //    for (int j = i + 1; j <= str.length(); ++j)
    //    {
    //        string fnd = str.substr(i, j - i);
    //        cout << fnd << " -- " << tree.Find(fnd) << "\n";
    //    }
    //}

    //cout << "\n\nFreq section\n\n";

    tree.GetRanges(2, 2);
    //tree.PrintSubstrings();
    return 0;
}