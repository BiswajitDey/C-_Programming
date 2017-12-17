#include "createDeBruijnGraph.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

void storeDeBruijnToFile(string filePath, unordered_map<string,bool> &nodes);

/**
 * Read from an input fastq file and return the nodes hash map
 */
unordered_map<string, bool> read_fastq_file(string filePath, int kMerSize)
{
  unordered_map<string,bool> nodes;
  nodes.reserve(160000);
  const char *file = filePath.c_str();
  string sequence;
  long seq_c = 0;
  int counter = 0;
  string line;
  int lineNumber = 0;
  ifstream myfile (file);
  if( myfile.is_open() ) {
    while ( getline (myfile,line) ) {
      lineNumber++;
      if((lineNumber-2) % 4 == 0) {
	if(counter<1) {
	  sequence = sequence + line;
	} else {
	  sequence = sequence.substr(sequence.length()+1-kMerSize) + line;
	}
	createNodesOfDeBruijn(sequence,kMerSize,nodes);
	counter++;
      }
    }
    myfile.close();
  }
  return nodes;
}



/**
 * Store DebruijnGraph To File
 */
void storeDeBruijnToFile(string filePath, unordered_map<string,bool> &nodes)
{
  ofstream deBruijn;
  const char *file = filePath.c_str();
  deBruijn.open (file);
  string matchList[4];
  string temp;
  for(auto val = nodes.begin(); val != nodes.end(); val++){
    temp = val->first.substr(1);
    matchList[0] = temp+"A";matchList[1] = temp+"C";matchList[2] = temp+"G";matchList[3] = temp+"T";
    deBruijn<< val->first<<"->[";
    for(int i=0;i<4;i++){
      if(nodes.find(matchList[i])!=nodes.end())
	deBruijn<<matchList[i]<<",";
    }
    deBruijn<<"]\n";
  }
  deBruijn.close();  
}
