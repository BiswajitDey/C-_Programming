#include <iostream>
#include <unordered_map>
#include <string>
#include "InAndOutMatrix.h"
#include "hashing.h"
#include "dph.h"
#include <cmath>
#include <queue>
#include "BooPHF.h"
#include "utils.h"

extern HashTable Hash;
extern uint32_t PRIME;

using namespace std;

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

/**
 * Tree Node
 */
typedef struct tree_node {
  unsigned long hash_value;
  tree_node *parent;
  char *kmer_str;
} tree_node;

/**
 * Queue for storing nodes at a given level of tree
 */
typedef struct level_node {
  unsigned long hash_value;
  string kmer;
} level_node;

vector<tree_node *> forest_vector;
unordered_map<long, tree_node *> node_pointer;
queue<level_node> level_nodes;


unordered_map<unsigned long, string> root_map;
unsigned char *parent_array;


/**
 * Create internal nodes
 */
void create_non_root_node(unsigned long mph, string kmer, struct inAndOutMatrix *matrix , char c) {

  if(kmer == "GAGAAGAAAAGAGTGGCAATTATAGTATCCAG") {
    cout << "\nFound : " << parent_array[mph];
  }
  
  if(parent_array[mph] == 'X'){
    parent_array[mph] = c;
    level_node lnode;
    lnode.hash_value = mph;
    lnode.kmer = kmer;
    level_nodes.push(lnode);
  } else {
    matrix->inMatrix[mph][4] = '1';
  }
}

/**
 * Process root and its connected components
 */
void process_root(string kmer,
		  unordered_map<string, bool> &nodes,
		  struct inAndOutMatrix *matrix,
		  boophf_t *mph_bf)
{
  int h = 0;
  const int MAX_HEIGHT = ceil(3 * kmer.length() * 0.6);

  unsigned long mph_main;
  
  //Process root
  unsigned long mph_root = get_dmph(kmer);

  if( matrix->inMatrix[mph_root][4] == '1' ) {
    cout << "Node already processed : " << kmer << endl;
    return;
  } else {
    cout << "Processing new root : " << kmer << endl;
  }

  cout << "\n\n";

  for(auto i = nodes.begin(); i != nodes.end(); i++) {
    cout << "\nNode : " << i->first << " hash : " << get_dmph(i->first);
  }
  cout << "\n\n";
  
  for(int i = 0; i < nodes.size(); i++) {
    for(int j = 0 ; j < 4; j++) {
      cout << matrix->inMatrix[i][j] << "\t";
    }
    cout << "\n";
  }


  cout << "\n\n";
  
  for(int i = 0; i < nodes.size(); i++) {
    for(int j = 0 ; j < 4; j++) {
      cout << matrix->outMatrix[i][j] << "\t";
    }
    cout << "\n";
  }

  root_map[mph_root] = kmer;

  level_node lroot;
  lroot.kmer = kmer;
  lroot.hash_value = mph_root;
  parent_array[mph_root] = 'E';
  level_nodes.push(lroot);

  string temp_prefix[4];
  string temp_suffix[4];
  string kmer_prefix;
  string kmer_suffix;
  unsigned long temp_mph_prefix;
  unsigned long temp_mph_suffix;

  level_node temp_node;
  unsigned long temp_mph;
  string temp_str;
  char c;
  char first_c;
  char last_c;
  
  while(level_nodes.empty() == false) {
    h = 0;
    temp_node = level_nodes.front();
    level_nodes.pop();
    temp_mph = temp_node.hash_value;
    temp_str = temp_node.kmer;
    if( matrix->inMatrix[temp_mph][4] == '1' ) {
      continue;
    }
    
    while(parent_array[temp_mph] != 'E') {
      cout << "\nString : "  << temp_str << "parent array : " << parent_array[temp_mph];
      sleep(1);
      temp_str = get_modified_kmer(temp_str, parent_array[temp_mph]);
      //cout << "\nModified String : "  << temp_str << "parent array : " << parent_array[temp_mph];
      temp_mph = get_dmph(temp_str);
      h++;
    }
    cout << "\nONE ROUND";
    
    if(h >= MAX_HEIGHT) {
      matrix->inMatrix[temp_node.hash_value][4] = '1';
      while(level_nodes.empty() == false) {
    temp_node = level_nodes.front();
    level_nodes.pop();
    matrix->inMatrix[temp_node.hash_value][4] = '1';
      }
      queue<level_node> empty;
      swap(level_nodes, empty);
      break;
    }

    mph_main = temp_node.hash_value;

    kmer_prefix = temp_node.kmer;
    kmer_suffix = temp_node.kmer;
    last_c = temp_node.kmer.at(kmer.length() - 1);
    kmer_prefix.pop_back();
    first_c = temp_node.kmer.at(0);
    kmer_suffix.erase(0, 1);

    cout << "\nString : " << kmer << " first_c : " << first_c << " last_c : " << last_c;
    
    temp_prefix[0] = "A" + kmer_prefix;
    temp_prefix[1] = "C" + kmer_prefix;
    temp_prefix[2] = "G" + kmer_prefix;
    temp_prefix[3] = "T" + kmer_prefix;

    temp_suffix[0] = kmer_suffix + "A";
    temp_suffix[1] = kmer_suffix + "C";
    temp_suffix[2] = kmer_suffix + "G";
    temp_suffix[3] = kmer_suffix + "T";

    //Check for connected components process only if not visited
    if( matrix->inMatrix[mph_main][4] == '0' ) {
      for(int i = 0; i < 4; i++) {
	if(matrix->inMatrix[mph_main][i] == '1') {
	  if(nodes.find(temp_prefix[i]) != nodes.end()) {
	    temp_mph_prefix = get_dmph(temp_prefix[i]);
	    c = get_parent_char(0, i);
	    create_non_root_node(temp_mph_prefix, temp_prefix[i], matrix, first_c+32);
	  }
	}
	
	if(matrix->outMatrix[mph_main][i] == '1') {
	  if(nodes.find(temp_suffix[i]) != nodes.end()) {
	    temp_mph_suffix = get_dmph(temp_suffix[i]);
	    c = get_parent_char(1, i);
	    create_non_root_node(temp_mph_suffix, temp_suffix[i], matrix, last_c);
	  }
	}
	
      }     
     
      matrix->inMatrix[mph_main][4] = '1';

    } // outer if

  } // first while

  //cout << "Height of tree : " << h << endl;

}

/**
 * Insert a node in forest
    @params kMer: kmer to be inserted
    @params matrix: in and out matrix
 */
void insert_forest(string kMer, struct inAndOutMatrix *matrix)
{
  if( !is_node_existing(kMer) ){
    root_map[hash_value] = kMer;
    parent_array[hash_value]='E';
    insert_into_matrix(matrix, kMer);

  } else {
    cout<<"\nNode already exists : " << kMer;
  }

}

/**
 Return the parent kMer string
 @params kMer: the child kMer
 @params nucleotide: the additional nucleotide in parent kMer
 */
string get_kmer_string(string kMer, char nucleotide){
    tKmer = kMer;
    string suffix, prefix;

    suffix = tKmer.substr(1);
    prefix=tKmer.substr(0,length-1);

        if(nucleotide=='a'){
        tKmer = suffix + "A";
        }
        else if(nucleotide=='a'){
        tKmer = suffix + "A";
        }
        else if(nucleotide=='c'){
            tKmer = suffix + "C";
        }
        else if(nucleotide=='g'){
            tKmer = suffix + "G";
        }
        else if(nucleotide=='t'){
            tKmer = suffix + "T";
        }
        else if(nucleotide=='A'){
            tKmer = prefix + "A";
        }
        else if(nucleotide=='C'){
            tKmer = prefix+ "C";
        }
        else if(nucleotide=='G'){
            tKmer = prefix + "G";
        }
        else if(nucleotide=='T'){
            tKmer = prefix + "T";
        }
    return tKmer;
}

/**
 * Search a kmer in forest
    @params kmer : kmer to be searched
    @params mph_bf : minimum perfect hash array
 */
bool search_forest(string kmer, boophf_t *mph_bf)
{
    string tKmer=kmer;
    int length = kmer.length();
    if(mph == -1) {
      return false;
    }
    string prefix,suffix;
    char nucleotide;
    while(parent_array[get_dmph(tKmer)])!='E'{
        nucleotide = parent_array[get_dmph(kmer)]);
        tKmer = get_kmer_string(tKmer, nucleotide);
    }
    if(root_map.find(tKmer)==root_map.end())
        return false;
    else
        return true;
}

/**
    DELETE A GIVEN NODE
    @params matrix: In and Out Matrix
    @params kMer : kMer to be deleted
    @params mph_bf: minimum perfect hash
*/
void deleteNode(struct inAndOutMatrix *matrix, string kMer, boophf_t *mph_bf){

  if(!is_node_existing(kMer)) {
    cout << "\nNode does not exist";
    return;
  }

  unsigned long hashValue = get_dmph(kMer);
  char startChar = kMer[0];
  char endChar = kMer[kMer.length()-1];
  string temp = kMer.substr(1);
  string temp1 = kMer.substr(0,kMer.length()-1);
  string inListOfNodes[4] = {"","","",""};
  string outListOfNodes[4] = {"","","",""};// = {temp+"A",temp+"C",temp+"G",temp+"T","A"+temp1,"C"+temp1,"G"+temp1,"T"+temp1};
  uint32_t hashOfElem;
  int ind = 0;int i;

  for(i = 0; i < 4; i++) {
    if(matrix->inMatrix[hashValue][i] == '1') {
      inListOfNodes[ind++] = get_char_from_int(i)+temp1;
    }
    matrix->inMatrix[hashValue][i] = 'e';
  }

  ind = 0;

  for(i = 0; i < 4; i++) {
    if(matrix->outMatrix[hashValue][i] == '1') {
      outListOfNodes[ind++] = temp + get_char_from_int(i);
    }
    matrix->outMatrix[hashValue][i] = 'e';
  }
    char nucleotide;int vals;
    string tKmer;
  for(string listNode : inListOfNodes) {
    if(listNode != "") {
      hashOfElem = get_dmph(listNode);
      nucleotide = parent_array[get_dmph(listNode)];
      tKmer = get_kmer_string(listNode);
      if(listNode == tKmer) {
        root_map[hashOfElem]=listNode;
        parent_array[get_dmph(listNode)] = 'E';
      }
      matrix->outMatrix[hashOfElem][get_int_from_char(endChar)] = '0';
    }
  }

  for(string listNode : outListOfNodes) {
    if(listNode != "") {
        hashOfElem = get_dmph(listNode);
        nucleotide = parent_array[get_dmph(listNode)];
        tKmer = get_kmer_string(listNode);
        if(listNode == tKmer) {
            root_map[hashOfElem]=listNode;
            parent_array[get_dmph(listNode)] = 'E';
        }
        matrix->inMatrix[hashOfElem][get_int_from_char(startChar)] = '0';
    }
  }

}


/**
 * Entry point for creation of forest
 */
void create_forest(unordered_map<string, bool> &nodes,
           struct inAndOutMatrix *matrix,
           boophf_t *mph_bf) {

  int count = 0;

  parent_array = new unsigned char[3*nodes.size()];

  for(int i = 0; i < 3*nodes.size(); i++) {
    parent_array[i] = 'X';
  }
  
  for(auto i = nodes.begin(); i != nodes.end(); i++) {
   
    process_root(i->first, nodes, matrix, mph_bf);
   
    count++;
  }

}
