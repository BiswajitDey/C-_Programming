#ifndef INOUT_H
#define INOUT_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "BooPHF.h"


using namespace std;
typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;


struct inAndOutMatrix{
    unsigned char **inMatrix;
    unsigned char **outMatrix;
};

struct inAndOutMatrix createInAndOutMatrix(unordered_map<string, bool> &nodes, boophf_t *bf);
struct inAndOutMatrix createInAndOutMatrix2(unordered_map<string, bool> nodes, unordered_map<string, long> hash);
void insert_into_matrix(struct inAndOutMatrix *matrix, string kMer);
#endif
