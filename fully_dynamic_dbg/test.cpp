#include <iostream>
#include <algorithm>
#include "hashing.h"
#include <unordered_map>
#include "InAndOutMatrix.h"
#include "deBruijnFileOperations.h"
#include "forest.h"
#include "BooPHF.h"
#include "utils.h"
#include <fstream>

using namespace std;

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;

boophf_t *min_perf_hash;
extern unsigned MAX_NODE;

extern int DYN_SIZE;

#define K_MER_SIZE 32

/**
 * Entry point for running test cases
 */
int main()
{
  string sequence;
  unordered_map<string, bool> nodes;
  vector<unsigned long> rabin_hash_values;
  boophf_t *mph_bf;
  int start_s, stop_s;

  //Read fastq file and create de Bruijn graph
  start_s = clock();
  cout << "\n---------------------- TESTING ---------------------\n" << endl;
  cout << "Reading fastq file : unit_test.fastq" << endl;
  nodes = read_fastq_file("unit_test.fastq", K_MER_SIZE);
  storeDeBruijnToFile("unit_test_debruijn.txt",nodes);
  cout << "\nCount of nodes : " << nodes.size() << endl;
  stop_s = clock();
  //cout << "\n## Time (read and create DBG): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n\n";
  cout << "\n ---------------------- C++ String Hashing ---------------------";

  MAX_NODE = nodes.size();

  start_s = clock();
  rabin_hash_values = rabin_karp(nodes);
  cout << "\nC++ String Hash size : " << rabin_hash_values.size() << endl;
  stop_s = clock();
  //cout << "\n## Time (C++ String hashing): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n\n";
  cout << "\n -------------------- Minimum Perfect Hashing ------------------------\n";

  DYN_SIZE = 3;

  // Minimum Perfect hashing
  start_s = clock();
  mph_bf  = minimum_perfect_hash(rabin_hash_values);
  stop_s = clock();
  //cout << "\n## Time (minimum perfect hashing): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n\n";


  min_perf_hash = mph_bf;

  cout << "\n- ----------- List of nodes ------------\n";
  for(auto i = nodes.begin(); i != nodes.end(); i++) {
    cout << i->first << " : " << get_dmph(i->first) << endl;
  }

  // Creating In and Out matrix
  start_s = clock();
  cout << "Creating in and out matrix" << endl;
  struct inAndOutMatrix matrix = createInAndOutMatrix(nodes, mph_bf);
  stop_s = clock();
  //cout << "\n## Time (in and out matrix creation): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n\n";

  cout << "\n---- IN MATRIX ----\nA\tC\tG\tT\n";
  for(int i = 0; i < nodes.size(); i++) {
    for(int j = 0; j < 4; j++) {
      cout << matrix.inMatrix[i][j] << "\t";

    }
    cout << "\n";
  }

  cout << "\n\n";

  cout << "---- OUT MATRIX ----\nA\tC\tG\tT\n";
  for(int i = 0; i < nodes.size(); i++) {
    for(int j = 0; j < 4; j++) {
      cout << matrix.outMatrix[i][j] << "\t";

    }
    cout << "\n";
  }

  cout << "\n ------------ Forest Creation ---------------------";

  // Creating forest
  start_s = clock();
  create_forest(nodes, &matrix, mph_bf);
  stop_s = clock();
  //cout << "\n## Time (forest creation): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << "ms\n";


  // generate_dynamic_hash("TEST");

  bool search;
  int search_node_count = 0;
  int totalSearch=0;
/*
  start_s = clock();
  for(auto i = nodes.begin(); i != nodes.end(); i++) {
    //cout << "\n Node : " << i->first << " Hash : " << get_dmph(i->first);
    search = search_forest(i->first, mph_bf);
    totalSearch++;
    if( search == false) {
      cout << "\nError in search : " << i->first;
    } else {
      //cout << "\nSuccessful search count : " << search_node_count;
      search_node_count++;
    }
  }
 stop_s=clock();
search = search_forest("TTAGATCACAGAAGAAAAGAGTGGCAATTATA", mph_bf);

  if(search == false) {
    cout << "\nNode not found : Test passed";
  } else {
    cout << "\nNode found : Test failed";
  }
  cout << "\nSearching forest successful search count : " << search_node_count<<"\nTotal Searched: "<<totalSearch;
*/search_node_count=0;
  start_s = clock();
totalSearch=0;
  cout << "\n\n\n--------------- Searching in Forest ---------------------";
  string line;
  ifstream myfile ("searchNodes.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if(line.length()==33)
            line.pop_back();
            if(line.length()!=32)
            {
                cout<<"\nkmer length needs to be 32 : "<<line;
                continue;
            }
            cout<<"\n\n\nNode to be searched: "<<line;
            totalSearch++;
            if(search_forest(line, mph_bf)){
                cout << "\nNode FOUND";
                search_node_count++;
            } else {

                cout << "\nNode NOT FOUND";
            }
        }
        myfile.close();
    }



  stop_s = clock();
  unsigned long time = (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000;

  cout << "\nSearching forest successful search count : " << search_node_count;

//  cout << "\n ----------------Search Forest : Negative Testing --------------------------";
/*
  search = search_forest("TTAAGATCAGAGAAGAAAAGAGTGGCAATAGT", mph_bf);

  if(search == false) {
    cout << "\nNode not found : Test passed";
  } else {
    cout << "\nNode found : Test failed";
  }
*/
  cout << "\n\n\n ---------Insert Forest  ----------------------------";

  int insert_count = 0;
  int delete_count = 0;
  string temp;
    totalSearch=0;
    search_node_count=0;
 start_s = clock();
 ifstream myfile1 ("insertNodes.txt");
    if (myfile1.is_open())
    {
        while ( getline (myfile1,line) )
        {
            if(line.length()==33)
                line.pop_back();
            if(line.length()!=32)
            {
                cout<<"\nkmer length needs to be 32 : "<<line;
                continue;
            }
            cout<<"\n\n\nNode to be inserted: "<<line;
            totalSearch++;
            if(insert_forest(line, &matrix))
            {
                search_node_count++;
                 cout<<"\nINSERTED : "<<line<<" SUCCESSFULLY!\nTrying to reinsert the node: ";
                if(insert_forest(line, &matrix))
                    cout<<"\nSHOULD NOT HAVE INSERTED";
                 else
                        cout<<"\nNODE ALREADY EXISTS!";
            }
            else
                cout<<"\nNODE ALREADY EXISTS";
        }
        myfile1.close();
    }

/*
  for(auto i = nodes.begin(); i != nodes.end(); i++) {
    temp = i->first;
    temp.pop_back();
    temp = temp + "G";
    insert_forest( temp, &matrix);
    search = search_forest(temp, mph_bf);
    if( search == false) {
      cout << "\nInsert fail : " << temp;
    } else {
      cout << "\nInsert success : " << temp;
      insert_count++;
    }

  }
*/
  stop_s = clock();
  time = (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000;
  cout << "\nSuccessful Insert count : " << search_node_count ;
  cout << "\n\n\n -------------------- Delete Forest -----------------------------";
  totalSearch=0;
  search_node_count=0;
  start_s = clock();
  ifstream myfile2 ("deleteNodes.txt");
  if (myfile2.is_open())
  {
        while ( getline (myfile2,line) )
        {
            if(line.length()==33)
                line.pop_back();

            if(line.length()<32)
            {
                cout<<"\nkmer length needs to be 32 : "<<line;
                continue;
            }
            totalSearch++;
            cout<<"\n\n\nNode to be deleted: "<<line;

            if(deleteNode(&matrix,line,mph_bf))
             {
                 search_node_count++;
                 cout<<"\nNODE DELETED: "<<line;
                 cout<<"\nTrying to delete again";
                if(deleteNode(&matrix,line,mph_bf))
                    cout<<"\nSHOULD NOT HAVE GIVEN SUCCESS";
                    else
                        cout<<"\nNODE DOES NOT EXIST";

             }
            else cout<<"\nNODE DOES NOT EXIST ";

        }
         myfile2.close();
    }

    stop_s = clock();
    time = (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000;
    cout << "\nSuccessful DELETE count : " << search_node_count ;




/*

  deleteNode(&matrix, "AAAGATCAGAGAAGAAAAGAGTGGCAATTATG", mph_bf);
*/
  cout << "\n\n---------------------------------- END of TESTING -----------------------------\n\n";

  return 0;

}
