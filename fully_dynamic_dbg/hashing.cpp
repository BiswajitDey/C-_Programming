#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include "hashing.h"
#include "BooPHF.h"
#include <cmath>

#define NO_OF_THREADS 10

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

unsigned long random_prime;

bool is_prime(unsigned long  number) {
  int i = 0;
  for(i = 2; i < sqrt(number); i++) {
    if(number % i == 0) {
      return false;
    }
  }
  return true;
}

unsigned long get_random_prime() {
  unsigned long random = rand();

  while(is_prime(random) == false) {
    random = rand();
  }
  return random;
}

unsigned long rk_hash(string key, int length) {

  unsigned long hash = 0;
  int d = 4;

  for (int i = 0; i < length; i++) {
    hash = hash + ((unsigned long)pow(d, 32 - 1 - i) * key[i]) % random_prime;
  }
  
  return hash;

}

unsigned long rk_hash1(string key, int length) {

  unsigned long hash = 0;
  int d = 4;

  for (int i = 0; i < length; i++) {
    hash = (d * hash + key[i]) % random_prime;
  }

  return hash;
}

/**
 * Using string hash of C++
 */
vector<unsigned long> perform_rabin_karp(unordered_map<string, bool> &kmers)
{
  unordered_map<unsigned long, unsigned long> temp_hash_values;
  vector<unsigned long> rabin_hash_values;
  std::hash<string> str_hash;

  int index = 0;

  random_prime = get_random_prime();

  unordered_map<string, bool>::iterator it = kmers.begin();

  while (index < kmers.size()) {

    unsigned long temp_hash = str_hash(it->first);

    if (temp_hash_values.find(temp_hash) != temp_hash_values.end()) {
      index = 0;
      it = kmers.begin();
      cout << "\nHash collision C++ string hash!";
      temp_hash_values.clear();
      rabin_hash_values.clear();
      random_prime = get_random_prime();
 
    } else {
      temp_hash_values[temp_hash] = temp_hash;
      rabin_hash_values.push_back(temp_hash);
      index++;
      it++;
    }

  }
  temp_hash_values.clear();
  return rabin_hash_values;
}

/**
 * Internally using string hash of C++
 */
unsigned long rabin_karp_single_entry(string kmer)
{
  std::hash<string> str_hash;
  
  unsigned long temp_hash = str_hash(kmer);
  return temp_hash;
}


/**
 * Rabin karp internallly uses string hash
 */
vector<unsigned long> rabin_karp(unordered_map<string, bool> &kmers)
{
  srand(time(0));

  vector<unsigned long> rabin_karp_result;

  rabin_karp_result = perform_rabin_karp(kmers);
  return rabin_karp_result;
}

/**
 * Minimum perfect hashing
 */
boophf_t * minimum_perfect_hash(vector<unsigned long> rabin_karp_result)
{
  boophf_t *bphf = NULL;
  int no_of_elements = rabin_karp_result.size();
  u_int64_t *data = (u_int64_t *) calloc(no_of_elements, sizeof(u_int64_t));
  u_int64_t *result = (u_int64_t *)calloc(no_of_elements, sizeof(u_int64_t));

  int index = 0;

  for(auto i = 0; i < rabin_karp_result.size(); i++) {
    data[index] = rabin_karp_result.at(i);
    index++;
  }

  auto data_iterator = boomphf::range(static_cast<const u_int64_t*>(data), static_cast<const u_int64_t*>(data+no_of_elements));

  double gammaFactor = 1.0;

  bphf = new boomphf::mphf<u_int64_t,hasher_t>(no_of_elements,data_iterator,NO_OF_THREADS,gammaFactor);

  return bphf;
}

