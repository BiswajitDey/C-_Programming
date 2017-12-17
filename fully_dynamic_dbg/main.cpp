#include <iostream>
#include <algorithm>
#include "hashing.h"
#include <unordered_map>
#include "InAndOutMatrix.h"
#include "deBruijnFileOperations.h"
#include "forest.h"
#include "BooPHF.h"
#include "utils.h"

using namespace std;

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

boophf_t *min_perf_hash;
extern unsigned MAX_NODE;

extern int DYN_SIZE;

#define K_MER_SIZE 32

/**
 * Point of entry for Dynamic De Bruijn Graph
 */
int main(int argc, char *argv[])
{
  string sequence;
  unordered_map<string, bool> nodes;
  vector<unsigned long> rabin_hash_values;
  unordered_map<string, long> mph_result;
  vector<string> kmers;
  uint32_t max = 0;
  uint32_t min = 9999999;
  boophf_t *mph_bf;
  int start_s, stop_s, start_ss, stop_ss;
  
  //Read fastq file and create de Bruijn graph
  start_s = clock();
  start_ss = start_s;

  cout << "\n--------------------------- TESTING FULLY DBG ---------------------\n";
  cout << "Reading fastq file : " << argv[1] << endl;
  nodes = read_fastq_file(argv[1], K_MER_SIZE);
  //storeDeBruijnToFile(argv[2], nodes);
  cout << "Count of nodes : " << nodes.size() << endl;
  stop_s = clock();  
  cout << "\n## Time (read and create DBG): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n";

  MAX_NODE = nodes.size();

  DYN_SIZE = 1;
  
  // Rabin karp Hashing
  start_s = clock();
  cout << "Starting string hash!" << endl;
  rabin_hash_values = rabin_karp(nodes);
  cout << "String Hash : hash size : " << rabin_hash_values.size() << endl;
  stop_s = clock();
  cout << "\n## Time (string hashing): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n";

  // Minimum Perfect hashing
  start_s = clock();
  mph_bf  = minimum_perfect_hash(rabin_hash_values);
  stop_s = clock();
  cout << "\n## Time (minimum perfect hashing): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n";

  //vector<unsigned long>().swap(rabin_hash_values);
  
  min_perf_hash = mph_bf;
 
  // Creating In and Out matrix
  start_s = clock();
  cout << "Creating in and out matrix" << endl;
  struct inAndOutMatrix matrix = createInAndOutMatrix(nodes, mph_bf);
  stop_s = clock();
  cout << "\n## Time (in and out matrix creation): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n";

  // Creating forest
  start_s = clock();
  cout << "Creating forest!" << endl;
  create_forest(nodes, &matrix, mph_bf);
  stop_s = clock();
  cout << "\n## Time (forest creation): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n";
  stop_ss = stop_s;

  cout << "\n## Time (total time): " << (stop_ss-start_ss)/double(CLOCKS_PER_SEC)*1000 << "ms\n";
  
  cout << "Forest created!" << endl;

  cout << "\n-----------------------------------------------------------------------------\n";
  
  return 0;

}



