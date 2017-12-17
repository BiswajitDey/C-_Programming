#include <iostream>
#include <unordered_map>
#include <string>
#include "InAndOutMatrix.h"
#include "hashing.h"
#include <cmath>
#include <queue>
#include "BooPHF.h"
#include "utils.h"

using namespace std;

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

/** Dynamic size multiplier */
extern int DYN_SIZE;

string get_kmer_string(string kMer, char nucleotide);

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
  char *kmer;
  level_node *parent;
  char par;
  int height=0;
} level_node;

vector<tree_node *> forest_vector;
unordered_map<long, tree_node *> node_pointer;
queue<level_node> level_nodes;


unordered_map<unsigned long, string> root_map;
unsigned char *parent_array;
unsigned long *parent_hash;


/**
 * Create internal nodes
 */
void create_non_root_node(level_node *parent_node, unsigned long mph, string kmer, struct inAndOutMatrix *matrix, char c, unsigned long phash ) {

  if(parent_array[mph]=='X'){

    level_node lnode;
    lnode.hash_value = mph;
    lnode.kmer= (char *) malloc (kmer.length()*sizeof(char));
    const char *temps  = kmer.c_str();
    strcpy(lnode.kmer, temps);
    lnode.parent = parent_node;
    lnode.par = c;
    parent_array[mph]=c;
    parent_hash[mph] = phash;
    lnode.height=parent_node->height+1;
    level_nodes.push(lnode);
  } else {
    matrix->inMatrix[mph][4] = '1';
  }
}

/**
 * Process root and its connected components
 */
void process_root(level_node *root,
          string kmer,
          unordered_map<string, bool> &nodes,
          struct inAndOutMatrix *matrix,
          boophf_t *mph_bf)
{
  int h = 0;
  const int MAX_HEIGHT = ceil(3 * kmer.length() * 0.6);
  //Process root
  root->kmer= (char *) malloc (kmer.length()*sizeof(char));
  const char *temps  = kmer.c_str();

  strcpy(root->kmer, temps);
  root->parent = NULL;
  root->height = 0;
  unsigned long mph, mph_main;
  unsigned long mph_root = get_dmph(root->kmer);
  root->hash_value = mph_root;

  if( matrix->inMatrix[mph_root][4] == '1' ) {
    free(root);
    return;
  } 

  parent_array[root->hash_value] = 'E';
  root_map[root->hash_value]=kmer;
  parent_hash[root->hash_value] = ULONG_MAX;
  level_node lroot;
  lroot.kmer = root->kmer;
  lroot.parent = NULL;
  lroot.hash_value = mph_root;
  lroot.height=0;
  lroot.par='E';
  level_nodes.push(lroot);

  string temp_prefix[4];
  string temp_suffix[4];
  string kmer_prefix;
  string kmer_suffix;
  unsigned long temp_mph_prefix;
  unsigned long temp_mph_suffix;

  level_node temp_node;
 h=0;
 char first,last;
  while(level_nodes.empty() == false) {
    temp_node = level_nodes.front();
    level_nodes.pop();
    first = temp_node.kmer[0];
    last = temp_node.kmer[31];
    h=temp_node.height;
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
    kmer_prefix.pop_back();
    kmer_suffix.erase(0, 1);

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
	  temp_mph_prefix = get_dmph(temp_prefix[i]);
	  create_non_root_node(&temp_node, temp_mph_prefix, temp_prefix[i], matrix,last+32, mph_main);
	}
	
	if(matrix->outMatrix[mph_main][i] == '1') {
	  temp_mph_suffix = get_dmph(temp_suffix[i]);
	  create_non_root_node(&temp_node, temp_mph_suffix, temp_suffix[i], matrix,first,mph_main);
	}
	
      }
      matrix->inMatrix[mph_main][4] = '1';
    } // outer if
    
  } // first while  
}

/**
 * Insert a node in forest
    @params kMer: kmer to be inserted
    @params matrix: in and out matrix
 */
bool insert_forest(string kMer, struct inAndOutMatrix *matrix)
{
  if( !is_node_existing(kMer) ){
    unsigned long hash_value = generate_dynamic_hash(kMer);
    root_map[hash_value] = kMer;
    parent_array[hash_value]='E';
    insert_into_matrix(matrix, kMer);
    return true;
    
  } else {
    return false;
  }
  
}

/**
 RETURN PARENT KMER OF STRING
 @params kMer: the child kMer
 @params nucleotide: the additional nucleotide in parent kMer
 */
string get_kmer_string(string kMer, char nucleotide){
  string tKmer = kMer;
  string suffix, prefix;
  int length = kMer.length();
  suffix = tKmer.substr(1);
  prefix=tKmer.substr(0,length-1);
  
  if(nucleotide=='A'){
    tKmer = "A"+prefix;
  }
  else if(nucleotide=='C'){
    tKmer = "C" + prefix;
  }
  else if(nucleotide=='G'){
    tKmer = "G" + prefix;
  }
  else if(nucleotide=='T'){
    tKmer = "T" + prefix;
  }
  else if(nucleotide=='a'){
    tKmer = suffix + "A";
  }
  else if(nucleotide=='c'){
    tKmer = suffix+ "C";
  }
  else if(nucleotide=='g'){
    tKmer = suffix + "G";
  }
  else if(nucleotide=='t'){
    tKmer = suffix + "T";
  }
  return tKmer;
}

/**
   SEARCH A KMER
   @params kmer : kmer to be searched
   @params mph_bf : minimum perfect hash array
*/
bool search_forest(string kmer, boophf_t *mph_bf)
{
  string tKmer = kmer;
  unsigned long mph = get_dmph(kmer);
  int length = kmer.length();
  if(mph == -1) {
    return false;
  }
  string prefix,suffix;
  char nucleotide;
  string oldKmer;
  unsigned long pHash;
  while(parent_array[get_dmph(tKmer)] != 'E' && parent_array[get_dmph(tKmer)] != 'X'){
    pHash = get_dmph(tKmer);
    nucleotide = parent_array[pHash];
    pHash = parent_hash[pHash]; 
    tKmer = get_kmer_string(tKmer, nucleotide);
    
    if(oldKmer == tKmer || get_dmph(tKmer)!= pHash )
      return false;
    else oldKmer = tKmer;
  }
  mph = get_dmph(tKmer);
  if(root_map.find(mph) == root_map.end()) {
    return false;
  } else {
    string t = root_map[mph];
    if(t != tKmer) {
      return false;
    }    
    return true;
  }
}

/**
    DELETE A GIVEN NODE
    @params matrix: In and Out Matrix
    @params kMer : kMer to be deleted
    @params mph_bf: minimum perfect hash
*/
bool deleteNode(struct inAndOutMatrix *matrix, string kMer, boophf_t *mph_bf){
  
  if(!is_node_existing(kMer)) {
    return false;
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
  
  parent_array[hashValue]='X';
  parent_hash[hashValue]=ULONG_MAX;
  root_map.erase(hashValue);
  char nucleotide;int vals;
  string tKmer;
  for(string listNode : inListOfNodes) {
    if(listNode != "") {
      hashOfElem = get_dmph(listNode);
      nucleotide = parent_array[hashOfElem];
      tKmer = get_kmer_string(listNode,nucleotide);
      if(listNode == tKmer && parent_hash[get_dmph(listNode)] == hashValue) {
        root_map[hashOfElem]=listNode;
        parent_array[hashOfElem] = 'E';
	parent_hash[hashOfElem] = ULONG_MAX;
      }
      matrix->outMatrix[hashOfElem][get_int_from_char(endChar)] = '0';
    }
  }

  for(string listNode : outListOfNodes) {
    if(listNode != "") {
        hashOfElem = get_dmph(listNode);
        nucleotide = parent_array[hashOfElem];
        tKmer = get_kmer_string(listNode,nucleotide);
        if(listNode == tKmer && parent_hash[get_dmph(listNode)] == hashValue) {
            root_map[hashOfElem]=listNode;
            parent_array[hashOfElem] = 'E';
	    parent_hash[hashOfElem] = ULONG_MAX;
        }
        matrix->inMatrix[hashOfElem][get_int_from_char(startChar)] = '0';
    }
  }
  return true;
}


/**
 * Entry point for creation of forest
 */
void create_forest(unordered_map<string, bool> &nodes,
           struct inAndOutMatrix *matrix,
           boophf_t *mph_bf) {
  
  int count = 0;
  parent_array = new unsigned char[DYN_SIZE*nodes.size()];
  parent_hash = new unsigned long[DYN_SIZE*nodes.size()];
  for(int i=0;i<DYN_SIZE*nodes.size();i++)
    {
      parent_array[i] = 'X';
      parent_hash[i] = ULLONG_MAX;
  }
  for(auto i = nodes.begin(); i != nodes.end(); i++) {
    level_node *root  = (level_node *) malloc(sizeof(level_node));
    process_root(root, i->first, nodes, matrix, mph_bf);
    count++;
  }

  cout << "\nRoot map size : " << root_map.size() << endl;
}
