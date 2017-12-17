#ifndef HASHING_H
#define HASHING_H
/* Hashing algorithm - Rabin Karp + Minimum Perfect Hashing */
#include <unordered_map>
#include <vector>
#include <string>
#include "BooPHF.h"

using namespace std;

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

vector<unsigned long> rabin_karp(unordered_map<string, bool> &kmers);
boophf_t * minimum_perfect_hash(vector<unsigned long> rabin_karp_result);
unsigned long rabin_karp_single_entry(string kmer);
unordered_map<string, long> minimum_perfect_hash2(unordered_map<string, uint32_t> rabin_karp_result);
void dynamic_perfect_hash(unordered_map<string, uint32_t> rabin_karp_result);

unordered_map<string, long> minimum_perfect_hash2(unordered_map<string, uint32_t> rabin_karp_result);
void printprime();

#endif
