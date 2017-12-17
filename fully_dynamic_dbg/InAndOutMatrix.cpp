#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>
#include "InAndOutMatrix.h"
#include "hashing.h"
#include "BooPHF.h"
#include "utils.h"

extern long MAX_NODE;
extern int DYN_SIZE;

using namespace std;
typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

/**
    Return the hash value of any given kMer
    @param: kMer: the kMer to be hashed
    return: hashed value of kMer
*/
long getHashedValue(string kMer, boophf_t *mph_bf){
    long rHash = rabin_karp_single_entry(kMer);
    long mph =  mph_bf->lookup(rHash);
    return mph;
}

/**
    Create In and Out Matrix of the DeBruijn Graph
    @param: nodes: List of all nodes of DeBruijnGraph
    return: struct of InMatrix and OutMatrix
*/
struct inAndOutMatrix createInAndOutMatrix(unordered_map<string, bool> &nodes, boophf_t *mph_bf){
    struct inAndOutMatrix matrix;
    matrix.inMatrix = new unsigned char *[DYN_SIZE*nodes.size()];

    matrix.outMatrix = new unsigned char *[DYN_SIZE*nodes.size()];
    for(int i = 0; i < DYN_SIZE*nodes.size(); i++){
        matrix.inMatrix[i] = new unsigned char[5];
        matrix.outMatrix[i] = new unsigned char[4];
    }

    for(int i=0;i<DYN_SIZE*nodes.size();i++)
    {
        for(int j=0;j<5;j++)
        {
            matrix.inMatrix[i][j]='0';
        }
    }
    for(int i=0;i<DYN_SIZE*nodes.size();i++)
    {
        for(int j=0;j<4;j++)
        {
            matrix.outMatrix[i][j]='0';
        }
    }

    unordered_map<string, bool>::iterator it = nodes.begin();
    string temp;int ccount=0;
    string matchList[4];
    string s;
    uint32_t indexInMatrix;
    uint32_t indexOutMatrix;
    unordered_map<string, bool>::iterator val;

    while(it != nodes.end()) {
        temp = it->first.substr(1);
        matchList[0] = temp+"A";matchList[1] = temp+"C";matchList[2] = temp+"G";matchList[3] = temp+"T";
        indexOutMatrix = getHashedValue(it->first, mph_bf);
       for(int i=0;i<4;i++){
            s=matchList[i];
            val = nodes.find(s);
            if(val != nodes.end()){
                indexInMatrix = getHashedValue(s, mph_bf);
                matrix.inMatrix[indexInMatrix][get_int_from_char(it->first[0])] = '1';
                matrix.outMatrix[indexOutMatrix][get_int_from_char(s.at(s.length()-1))] = '1';
            }
        }
        it++;
    }
    return matrix;
}

/**
    Create In and Out Matrix of the DeBruijn Graph
    @param: nodes: List of all nodes of DeBruijnGraph
    return: struct of InMatrix and OutMatrix
*/
void insert_into_matrix(struct inAndOutMatrix *matrix, string kMer)
{
    unsigned long index=MAX_NODE, ind1 = 0;
    char startChar = kMer[0];
    char endChar = kMer[kMer.length()-1];
    string temp = kMer.substr(1);
    string matchList[4]={temp+"A",temp+"C",temp+"G",temp+"T"};
    for(int i=0;i<4;i++)
    {
        if(is_node_existing(matchList[i])){
            unsigned long ind1 =  get_dmph(matchList[i]);
            if(matrix->inMatrix[ind1][0]!='e'){
                matrix->outMatrix[index][i] = '1';
                matrix->inMatrix[ind1][get_int_from_char(startChar)]= '1';
            }
        }
    }
    temp = kMer.substr(0,kMer.length()-1);
    matchList[0] = "A"+temp;matchList[1] = "C"+temp;matchList[2] = "G"+temp;matchList[3] = "T"+temp;

    for(int i=0;i<4;i++)
    {
        if(is_node_existing(matchList[i])){
            unsigned long ind1 =  get_dmph(matchList[i]);
            if(matrix->inMatrix[ind1][0]!='e'){
                matrix->inMatrix[index][get_int_from_char(matchList[i][0])] = '1';
                matrix->outMatrix[ind1][get_int_from_char(endChar)]= '1';
            }
        }
    }
}
