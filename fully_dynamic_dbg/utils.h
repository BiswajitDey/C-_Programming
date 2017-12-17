#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>

using namespace std;

int get_int_from_char(char c);

char get_char_from_int(int i);

string get_kmer(int i, unordered_map<string, long> mph_result);

bool validate_in_out(struct inAndOutMatrix matrix, unordered_map<string, long> mph_result);

unsigned long generate_dynamic_hash(string kmer);

unsigned long get_dmph(string kmer);

bool is_node_existing(string kmer);

string get_modified_kmer(string kmer, char C);

char get_parent_char(int, int);

#endif
