#ifndef FOREST_H
#define FOREST_H

#include "InAndOutMatrix.h"
#include <unordered_map>
#include <string>
#include "BooPHF.h"

using namespace std;

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

void create_forest(unordered_map<string, bool> &nodes, struct inAndOutMatrix *matrix, boophf_t *mph_bf);
bool search_forest(string kmer, boophf_t *mph_bf);
bool insert_forest(string kMer, struct inAndOutMatrix *matrix);
bool deleteNode(struct inAndOutMatrix *matrix, string kMer, boophf_t *mph_bf);

#endif
