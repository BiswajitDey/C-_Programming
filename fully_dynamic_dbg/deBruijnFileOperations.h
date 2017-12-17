#ifndef DBGF_H
#define DBGF_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
unordered_map<string,bool> read_fastq_file(string filePath, int k);
void storeDeBruijnToFile(string filePath, unordered_map<string,bool> &nodes);

#endif
