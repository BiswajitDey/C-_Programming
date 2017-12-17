#include <iostream>
#include <string>
#include "hashing.h"
#include <limits.h>
#include "forest.h"

using namespace std;

extern boophf_t *min_perf_hash;
unsigned long MAX_NODE;
unordered_map<string, unsigned long> dynamic_hash;

int DYN_SIZE;

/**
 * Generate dynamic hash for new nodes
 */
unsigned long generate_dynamic_hash(string kmer)
{
  // cout << "\nDH : MAX_NODE : " << MAX_NODE; 
  if(search_forest(kmer, min_perf_hash) == false) {
    dynamic_hash[kmer] = MAX_NODE + 1;
    MAX_NODE++;
  }
  return dynamic_hash[kmer];
}

/**
 * Get dynamic minimum perfect hash
 */
unsigned long get_dmph(string kmer)
{
  unsigned long rk_hash = rabin_karp_single_entry(kmer);

  if (dynamic_hash.find(kmer) != dynamic_hash.end()) {
    return dynamic_hash[kmer];
  } else if(min_perf_hash->lookup(rk_hash) != ULLONG_MAX) {
    return min_perf_hash->lookup(rk_hash);
  } else {
    return -1;
  }  
}


string get_modified_kmer(string kmer, char c) {
  string result = kmer;
  switch(c) {
  case 'A':
    result = "A" + result;
    result.pop_back();
    break;
  case 'C':
    result = "C" + result;
    result.pop_back();
    break;
  case 'G':
    result = "G" + result;
    result.pop_back();
    break;
  case 'T':
    result = "T" + result;
    result.pop_back();
    break;
  case 'a':
    result = result + 'A';
    result.erase(0,1);
    break;
  case 'c':
    result = result + 'C';
    result.erase(0,1);
    break;
  case 'g':
    result = result + 'G';
    result.erase(0,1);
    break;
  case 't':
    result = result + 'T';
    result.erase(0,1);
    break;
  }

  return result;

}


/**
 * Check if node exists in forest
 */
bool is_node_existing(string kmer)
{
  return search_forest(kmer, min_perf_hash);
}

int get_int_from_char(char c)
{
  if(c=='A')
    return 0;
  else if(c=='C')
    return 1;
  else if(c=='G')
    return 2;
  else if(c=='T')
    return 3;
  else
    return -1;
}

char get_char_from_int(int i) {
  if(i==0)
    return 'A';
  else if(i==1)
    return 'C';
  else if(i==2)
    return 'G';
  else if(i==3)
    return 'T';
  else
    return 'S';
}

/*
string get_kmer(int i, unordered_map<string, long> mph_result){
  for(auto j=mph_result.begin();j!=mph_result.end();j++)
    {
      if(j->second==i)
	return j->first;

    }
}
*/

/*
bool validate_in_out(struct inAndOutMatrix matrix, unordered_map<string, long> mph_result)
{
  int total = 0;
  int length = sizeof(matrix.inMatrix)/sizeof(matrix.inMatrix[0]);
  cout<<length;
  for(int i=0;i<length;i++) {
    for(int j=0;j<4;j++) {
      if(matrix.inMatrix[i][j]==1)
	{
	  char c = getChar(j);
	  string kMer = getK_Mer(i,mph_result);
	  string temp = c + kMer.substr(0,K_MER_SIZE-1);
	  auto val = mph_result.find(temp);
	  int ind= val->second;
	  int ind1 = getValue1(kMer.at(K_MER_SIZE-1));
	  int val1 = matrix.outMatrix[ind][ind1];
	  if(val1 != 1) {
	    cout<<"THERE IS AN ERROR: "<< ++total;
	  }
	}
    }
  }
}

*/
  /*
  for(int i=0;i<3*nodes.size();i++)
    {
        for(int j=0;j<5;j++)
        {
            cout<<"InMatrix ["<<i<<"]["<<j<<"]"<<matrix.inMatrix[i][j];
            cout<<"OutMatrix ["<<i<<"]["<<j<<"]"<<matrix.inMatrix[i][j]<<"\n";

        }

    }*/


  //dynamic_perfect_hash(rabin_karp_result);
  /*
  cout << "Checking hash values!" << endl;
  unordered_map<uint32_t, bool> temph;
  int dupc = 0;
  for(int i = 0; i < kmers.size(); i++) {
    uint32_t temp_rk_hash = rabin_karp_single_entry(kmers[i]);
    if(Lookup(temp_rk_hash) != 1) {
      cout << "\nError in dph";
    }
    uint32_t b = HashFunction(temp_rk_hash, Hash.k_parameter, PRIME, Hash.size);

    //uint32_t b = GetPH(temp_rk_hash);
    //uint32_t c = HashFunction(temp_rk_hash, Hash.sub_table[b].k_parameter, PRIME, Hash.sub_table[b].space);
    //cout << "\nMPH : " << b;
    if(temph.find(b) != temph.end() ) {
      dupc++;
    }
    temph[b] = true;
    if(b > max) {
      //cout << "test";
      max = b;
    }
    if(b < min) {
      //cout << "test";
      min = b;
    }
  }
  cout << "Finished checking hash values!" << endl;
  cout << "\nMAX : " << max << " Min : " << min <<  " dup : " << dupc << "\n";

  */

char get_parent_char(int in_out, int char_loc) {
  char result;
  if(in_out == 0) {
    result = get_char_from_int(char_loc);
    result = result + 32;
  } else {
    result = get_char_from_int(char_loc);

  }
  return result;
}
