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
    // aabababadvc* - crash
    // aaadvc* - crash
    // aaba <-
    //string str = "aababaaabadvc*";
    string str = "aabaaabaaba*";

    CSuffixTrie tree(str);
    tree.buildSuffixTree();
    
    for (int i = 0; i < str.length(); ++i)
    {
        for (int j = i + 1; j <= str.length(); ++j)
        {
            string fnd = str.substr(i, j - i);
            if (fnd == "babadvc*")
            {
                int a = 0;
            }
            cout << fnd << " -- " << tree.find(fnd) << "\n";
        }
    }

    tree.OutWalkTree();

    cout << tree.find("baab");
    /*cout << tree.find("baab");
    cout << tree.find("<META NAME=\"Description\" CONTENT=\"Все о Linux на русском языке\">");
    cout << tree.find("<div");
    cout << tree.find("aabaa");
    cout << tree.find("<li><a");*/

    return 0;
}