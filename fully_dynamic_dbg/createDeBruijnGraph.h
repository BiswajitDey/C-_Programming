#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
void createNodesOfDeBruijn(string sequence, int k,unordered_map<string,bool> &nodes);
//unordered_map<string,vector<string>> createAdjacencyListOfDeBruijn(int k,unordered_map<string,vector<string>> nodes);
