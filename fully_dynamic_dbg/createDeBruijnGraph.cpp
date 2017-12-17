#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;
long count_total = 0;

/**
 * Create List of nodes of debruijng from the read sequence Time complexity: O(2N)
 * @param sequence - Sequence : sequence read
 * @param k - length of k-mer
 *
*/
void createNodesOfDeBruijn(string sequence, int k, unordered_map<string,bool> &nodes){
  //int start_s=clock();
  int i = 0;
  string temp;
  string first_kmer;
  first_kmer = sequence.substr(0,k);
  nodes[first_kmer] = true;
  i++;
  while(i + k <= sequence.length()){
    //cout << first_kmer << "\n";
    first_kmer.erase(0,1);
    temp = first_kmer + sequence[i + k - 1];
    first_kmer = temp;

    if(temp == "TTTTGGTGTCAGCAATAACAATGCTAACACCA") {
      cout << "\nBAZINGA!!!\n";
    }
    // cout << "KMER : " << temp << endl;
    
    if(temp.find("S") != string::npos){
      int ind = temp.find("S");
      string temp1 = temp;
      temp[ind] = 'G';
      temp1[ind] = 'C';
      nodes[temp] =  true;
      nodes[temp1] =  true;
    } else {     
      nodes[temp] = true;
    }
    //cout << "\nExisitng nodes : " << count_existing << "\n";
    //cout << "\nNode count : " << nodes.size() << "\n";
    count_total++;
    i++;
  }
  //    if(nodes.size() % 100000 < 2000) {
  //cout << "\nNodes.size = " << nodes.size();
  //}
  //int stop_s=clock();
  //cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms length : " << sequence.length() << "total count : " << count_total << endl;
  
  // return nodes;
}

/**
 * Create adjacency list of nodes in debruijn graph. Time complexity: O(2N)
 * @param sequence - Sequence : sequence read
 * @param k - length of k-mer
 * @param nodes - list of nodes of DeBruijn Graph
 *
*/
unordered_map<string,vector<string>> createAdjacencyListOfDeBruijn(int k,unordered_map<string,vector<string>> nodes){
    unordered_map<string, vector<string>>::iterator it = nodes.begin();
    while(it != nodes.end()){
        string temp = it->first.substr(1);
        string matchList[4] = {temp+"A",temp+"C",temp+"G",temp+"T"};
        vector<string> listNodes;
        for(int i=0;i<4;i++)
        {
            unordered_map<string,vector<string>>::const_iterator searchItem = nodes.find (matchList[i]);
            if(searchItem!=nodes.end() && matchList[i] != it->first){
                listNodes.push_back(matchList[i]);
            }
        }
        it->second = listNodes;
        it++;
    }
    return nodes;
}


