
/** Steps to Execute fully dynamic DBG **/

---------------------------------------------------------------
System Requirements :
---------------------------------------------------------------
OS          : Linux (Tested on Ubuntu 16.04 LTS) (64 BIT Version)
ARCHITECTURE: 64 BIT  
Memory      : Based on file executed. Recommeneded : Atleast 8GB.
Compiler    : g++
---------------------------------------------------------------

STEPS :-
---------------------------------------------------------------
1. Download fully_dynamic_dbg.zip from website.
2. Extract fully_dynamic_dbg.zip.
3. Open terminal(Ctrl+Shift+T in Ubuntu) and goto extracted fully_dynamic_dbg/ directory (Command : cd /home/<username>/Downloads/fully_dynamic_dbg). Or you can right click on the extracted   fully_dynamic_dbg directory and click on open in terminal.
4. Run 'make' command to build binary.
5. This will generate 3 binary files:-
  
  a) dbg
  DESCRIPTION       : Used to test construction of data structure for a given fastq file as input.
  EXECUTION COMMAND : ./dbg <input_file.fastq>
  EXAMPLE COMMAND   : ./dbg norovirus.fastq


  b) dbg_unit_test
  DESCRIPTION 	    : Used for unit testing a sample fastq file already present in current directory : unit_test.fastq
  	              Edit searchNodes.txt to specify what all nodes to search. (Sample nodes already present)	
  	      	      Edit insertNodes.txt to specify what all nodes to insert. (Sample nodes already present)
		      Edit deleteNodes.txt to specify what all nodes to delete. (Sample nodes already present)
	
  EXECUTION COMMAND : ./dbg_unit_test 
  EXAMPLE           : ./dbg_unit_test

  Can open generated unit_test_debruijn.txt to see the De Bruijn Graph.

  c) norovirus_test
  DESCRIPTION : Used for testing construction of datastructure for norovirus.fastq input file.

  EXECUTION COMMAND : ./norovirus_test 
  EXAMPLE           : ./norovirus_test

  Can open generated norovirus.txt to see the De Bruijn Graph

6. For any queries Contact us using our website/Contact section.
   https://www.dynamicdebruijnuf.com/

---------------------------------------------------------------
