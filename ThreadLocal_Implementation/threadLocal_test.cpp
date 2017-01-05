#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <string>
#include <unistd.h>
#include "threadLocal.h"

using namespace std;

std::mutex cout_mutex;

class TestObj {
    private :
                int value;
};

const int MAX_TEST_CASES                                    = 7;

const int TEST_GET                                                      = 1;
const int TEST_SET                                                      = 2;
const int TEST_REMOVE                                             = 3;
const int TEST_MULTI_SET_GET                              = 4;
const int TEST_MULTI_GET_SET                              = 5;
const int TEST_MULTI_SET_GET_REMOVE           = 6;
const int TEST_MULTI_GET_SET_REMOVE           = 7;
const int TEST_OBJ                                                       = 8;

void test_setM();
void test_getM();
void test_removeM();
void test_MultiSetGetM();
void test_MultiGetSetM();
void test_MultiSetGetRemoveM();
void test_MultiGetSetRemoveM();
void test_Object();


template<typename  T> T get_Wrapper(int,int);
template<typename  T> void set_Wrapper(int, int, T);
template<typename  T> void remove_Wrapper(int,int);
template<typename  T> T getTestVal(int);


template<typename T> void test_setMethod(int);
template<typename T> void test_getMethod(int);
template<typename T> void test_removeMethod(int);
template<typename T> void test_MultiSetGetMethod(int);
template<typename T> void test_MultiGetSetMethod(int);
template<typename T> void test_MultiSetGetRemoveMethod(int);
template<typename T> void test_MultiGetSetRemoveMethod(int);


atomic<int> num_errors_tc[MAX_TEST_CASES + 2];

cop5618::threadLocal<int> thdLcl_int[MAX_TEST_CASES + 1];
cop5618::threadLocal<char> thdLcl_char[MAX_TEST_CASES + 1];
cop5618::threadLocal<double> thdLcl_double[MAX_TEST_CASES + 1];
cop5618::threadLocal<TestObj> thdLcl_obj;

int test_threadLocal()
{
    int num_errors = 0;
    const int max_thread_count = 2;
    vector<std::thread> threads;
    int test_case_index = 1;
    int i = 0;

    cout << "############## Inside test_threadLocal() ################" << endl;
    cout << "Number of test cases = " << MAX_TEST_CASES + 1<< endl;
    cout << "Number of threads per test case = " << max_thread_count << endl ;
    cout << "Datatype Tested : <int> <char> <double> <object>" << endl << endl;

    //init error count
    for(i = 1; i <= MAX_TEST_CASES; i++) {
        num_errors_tc[i].store(0);
    }

    while(test_case_index <= MAX_TEST_CASES ) {
         void (*func_ptr)();

       switch(test_case_index) {

            case TEST_GET :
                                           func_ptr = &test_getM;
                                            break;
            case TEST_SET:
                                            func_ptr= &test_setM;
                                            break;
            case TEST_REMOVE:
                                            func_ptr= &test_removeM;
                                            break;
            case TEST_MULTI_SET_GET:
                                            func_ptr= &test_MultiSetGetM;
                                            break;
            case TEST_MULTI_GET_SET:
                                            func_ptr= &test_MultiGetSetM;
                                            break;
            case TEST_MULTI_SET_GET_REMOVE:
                                            func_ptr= &test_MultiSetGetRemoveM;
                                            break;
            case TEST_MULTI_GET_SET_REMOVE:
                                            func_ptr= &test_MultiGetSetRemoveM;
                                            break;


            default:
                            cout << "All test cases are over";

        }
        test_case_index++;

        // Creating threads for testing get()/set()/remove() methods
        for( i = 0; i < max_thread_count ; i++) {
            threads.push_back(thread(func_ptr));
        }
        /* Give time for test case # to complete for output's sake! */
        sleep(2);
        cout_mutex.lock();
        cout << "-------------------------------------------------------------------------------------------" << endl;
        cout_mutex.unlock();
    }
    // Test with object as datatype as well
    for( i = 0; i < max_thread_count ; i++) {
            threads.push_back(thread(test_Object));
    }

    for (auto& t : threads) {
            t.join();
    }

    for(i = 1; i <= MAX_TEST_CASES + 1;  i++  ) {
        num_errors += num_errors_tc[i].load();
    }

    cout << "[Totat Test Cases : " <<  MAX_TEST_CASES + 1<< "] [Test Cases Passed : " << (MAX_TEST_CASES + 1- num_errors)<< "] [Error Count :  " << num_errors <<"}"<< endl;
    cout << "(Note : Mutiple errors per test case are possible for a single test case as multiple threads are executing and multiple errors can occur during a test case execution)." << endl;
    cout <<   "############################################################" << endl;

    return num_errors;
}

/*
* A wrapper function for get on different datatypes
*/
template<typename T>
T get_Wrapper(int type, int index) {
    try {
    T ret_val;
        switch(type) {
            case 1 :
                        ret_val = thdLcl_int[index].get();
                        return ret_val;
            case 2:
                        ret_val = thdLcl_char[index].get();
                        return ret_val;
            case 3:
                       ret_val = thdLcl_double[index].get();
                        return ret_val;
        }
    }
    catch(std::out_of_range oe) {
       throw oe;
    }
    catch(std::exception e) {
        throw e;
    }
}

/*
* A wrapper function for set on different datatypes
*/
template<typename T>
void set_Wrapper(int type, int index, T value) {
    try {
        switch(type) {
            case 1 :
                        thdLcl_int[index].set(value);
                        cout << "Map contents after set : " << thdLcl_int[index] << endl;
                        break;
            case 2:
                        thdLcl_char[index].set(value);
                        cout << "Map contents after set : " << thdLcl_char[index] << endl;
                        break;
            case 3:
                       thdLcl_double[index].set(value);
                       cout << "Map contents after set : " << thdLcl_double[index] << endl;
                        break;
        }
    }
    catch(std::out_of_range oe) {
       throw oe;
    }
    catch(std::exception e) {
        throw e;
    }
}

/*
* A wrapper function for remove on different datatypes
*/
template<typename T>
void remove_Wrapper(int type, int index) {
    try {
        switch(type) {
            case 1 :
                        thdLcl_int[index].remove();
                        break;
            case 2:
                        thdLcl_char[index].remove();
                        break;
            case 3:
                       thdLcl_double[index].remove();
                        break;
        }
    }
    catch(std::out_of_range oe) {
       throw oe;
    }
    catch(std::exception e) {
        throw e;
    }
}

/*
* test_getMethod() - Test single get
* PASS Scenario : Exception should be thrown as no threadlocal data present
*/
template<typename T>
void test_getMethod(int type)
{
    T ret_val;

    cout_mutex.lock();
    cout << "Test Case #" << TEST_GET <<"  test_getMethod(). ThreadId : " <<  std::this_thread::get_id()<< endl;
    cout_mutex.unlock();

    try {
        ret_val = get_Wrapper<T>(type, TEST_GET);
        num_errors_tc[TEST_GET]++;
    }
    catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
    }
    if (num_errors_tc[TEST_GET].load() <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout_mutex.unlock();
}


/*
* test_setMethod() - Tests single set
* PASS Scenario : No exception should be thrown as set should be successful
*/
template<typename T>
void test_setMethod(int type)
{
    const T set_val = 65;
    cout_mutex.lock();
    cout <<"Test Case #" << TEST_SET <<"  test_setMethod(). ThreadId : " <<  std::this_thread::get_id() << endl;
    cout_mutex.unlock();

    try {
        set_Wrapper<T>(type, TEST_SET, set_val);
    }catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
        num_errors_tc[TEST_SET]++;
    }

    if (num_errors_tc[TEST_SET] <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout_mutex.unlock();
}

/*
* test_setMethod() - Tests single set
* PASS Scenario : No exception should be thrown as set should be successful
*/
template<typename T>
void test_removeMethod(int type)
{
    const T set_val = 65;
    cout_mutex.lock();
    cout <<"Test Case #" << TEST_REMOVE <<"  test_removeMethod(). ThreadId : " <<  std::this_thread::get_id() << endl;
    cout_mutex.unlock();

    try {
        remove_Wrapper<T>(type, TEST_REMOVE);
        num_errors_tc[TEST_REMOVE]++;
    }catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
    }

    if (num_errors_tc[TEST_REMOVE] <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout_mutex.unlock();
}


/*
* test_MultiSetGetMethod() - Tests set() followed by get()
* PASS Scenario : Get Value should be same as set value and no exception thrown
*/
template<typename T>
void test_MultiSetGetMethod(int type)
{
    const T set_val = 65;
    T ret_val = 0;
    cout_mutex.lock();
    cout << "Test Case #" << TEST_MULTI_SET_GET <<" test_MultiSetGetMethod(). ThreadId : " <<  std::this_thread::get_id() << endl;
    cout_mutex.unlock();

    try {
        set_Wrapper<T>(type, TEST_MULTI_SET_GET, set_val);
        ret_val = get_Wrapper<T>(type, TEST_MULTI_SET_GET);

        if(ret_val != set_val) {
            cout << "Set-get operation failed" << endl;
            num_errors_tc[TEST_MULTI_SET_GET]++;
        }
    }
    catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
       cout << "Exception occurred :" << e.what() << endl;
        num_errors_tc[TEST_MULTI_SET_GET]++;
    }

    if (num_errors_tc[TEST_MULTI_SET_GET] <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
     cout << "-------------------------------------------------------------------------------------------" << endl;
    cout_mutex.unlock();
}

/*
* test_MultiGetSetMethod() - Tests get() followed by set()
* PASS Scenario : Exception should be thrown for first get() and set() get() should work
*                                properly after that
*/
template<typename T>
void test_MultiGetSetMethod(int type)
{
    const T set_val = 65;
    T ret_val = 0;
    cout_mutex.lock();
    cout << "Test Case #" << TEST_MULTI_GET_SET <<"  test_MultiGetSetMethod(). ThreadId : " <<  std::this_thread::get_id()<< endl;
    cout_mutex.unlock();

    try {
        ret_val = get_Wrapper<T>(type, TEST_MULTI_GET_SET);
        num_errors_tc[TEST_MULTI_GET_SET]++;
    }
    catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
    }
    try {
        set_Wrapper<T>(type, TEST_MULTI_SET_GET, set_val);
        ret_val = get_Wrapper<T>(type, TEST_MULTI_GET_SET);

         if(ret_val != set_val) {
                cout << "Set operation failed" << endl;
                num_errors_tc[TEST_MULTI_GET_SET]++;
        }
    }catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
        num_errors_tc[TEST_MULTI_GET_SET]++;
    }

     if (num_errors_tc[TEST_MULTI_GET_SET] <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout_mutex.unlock();
}

/*
* test_MultiSetGetRemoveMethod() - Tests multiple set(),get() and remove()
* PASS Scenario : Exception should be thrown for trying to get after remove()
*/
template<typename T>
void test_MultiSetGetRemoveMethod(int type)
{
    const T set_val = 65;
    T ret_val = 0;
    cout_mutex.lock();
    cout << "Test Case #" << TEST_MULTI_SET_GET_REMOVE <<" test_MultiSetGetRemoveMethod(). ThreadId : " <<  std::this_thread::get_id() << endl;
    cout_mutex.unlock();

    try {
        set_Wrapper<T>(type, TEST_MULTI_SET_GET_REMOVE, set_val);
        ret_val = get_Wrapper<T>(type, TEST_MULTI_SET_GET_REMOVE);

        if(ret_val != set_val) {
            cout << "Set-get operation failed" << endl;
            num_errors_tc[TEST_MULTI_SET_GET_REMOVE]++;
        }

        remove_Wrapper<T>(type, TEST_MULTI_SET_GET_REMOVE);

        ret_val = get_Wrapper<T>(type, TEST_MULTI_SET_GET_REMOVE);

        num_errors_tc[TEST_MULTI_SET_GET_REMOVE]++;
    }
    catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
       cout << "Exception occurred :" << e.what() << endl;
       num_errors_tc[TEST_MULTI_SET_GET_REMOVE]++;
    }

    if (num_errors_tc[TEST_MULTI_SET_GET_REMOVE] <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
     cout << "-------------------------------------------------------------------------------------------" << endl;
     cout_mutex.unlock();
}


/*
* test_MultiGetSetRemoveMethod() - Tests multiple set(),get() and remove()
* PASS Scenario : Exception should be thrown for trying to get after remove()
*/
template<typename T>
void test_MultiGetSetRemoveMethod(int type)
{
    const T set_val = 65;
    T ret_val = 0;
    cout_mutex.lock();
    cout << "Test Case #" << TEST_MULTI_GET_SET_REMOVE <<" test_MultiGetSetRemoveMethod(). ThreadId : " <<  std::this_thread::get_id() << endl;
    cout_mutex.unlock();

    try {
        ret_val = get_Wrapper<T>(type, TEST_MULTI_GET_SET_REMOVE );
        num_errors_tc[TEST_MULTI_GET_SET_REMOVE]++;
    }
    catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
    }
    try {
        set_Wrapper<T>(type, TEST_MULTI_GET_SET_REMOVE, set_val);
       remove_Wrapper<T>(type, TEST_MULTI_GET_SET_REMOVE);
        ret_val = get_Wrapper<T>(type, TEST_MULTI_GET_SET_REMOVE );

         num_errors_tc[TEST_MULTI_GET_SET_REMOVE]++;
    }catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
    }

     if (num_errors_tc[TEST_MULTI_GET_SET_REMOVE] <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
     cout << "-------------------------------------------------------------------------------------------" << endl;
     cout_mutex.unlock();
}

/* Test object as type in map */
void test_Object() {
    TestObj tObj;
    TestObj ret_val ;
    cout_mutex.lock();
    cout << "Test Case #" << TEST_OBJ <<" test_Object ThreadId : " <<  std::this_thread::get_id() << endl;
    cout_mutex.unlock();

    try {
        thdLcl_obj.set(tObj);
        thdLcl_obj.remove();

    }catch(std::out_of_range oe) {
        cout << "Exception occurred :" << oe.what() << endl;
        num_errors_tc[TEST_OBJ]++;
    }
    catch(std::exception e) {
        cout << "Exception occurred :" << e.what() << endl;
        num_errors_tc[TEST_OBJ]++;
    }

     if (num_errors_tc[TEST_OBJ] <= 0) {
        cout << "Test success! " <<endl;
    } else {
        cout << "Test failure!" <<endl;
    }

    cout_mutex.lock();
     cout << "-------------------------------------------------------------------------------------------" << endl;
     cout_mutex.unlock();
}


/*
* Functions to call test cases for different datatypes
* Written separately as function pointer does not accept template based functions
*/

void test_setM()
{
    test_setMethod<int>(1);
    test_setMethod<char>(2);
    test_setMethod<double>(3);
}

void test_getM()
{
    test_getMethod<int>(1);
    test_getMethod<char>(2);
    test_getMethod<double>(3);
}

void test_removeM()
{
    test_removeMethod<int>(1);
    test_removeMethod<char>(2);
    test_removeMethod<double>(3);
}

void test_MultiSetGetM()
{
    test_MultiSetGetMethod<int>(1);
    test_MultiSetGetMethod<char>(2);
    test_MultiSetGetMethod<double>(3);
}

void test_MultiGetSetM()
{
    test_MultiGetSetMethod<int>(1);
    test_MultiGetSetMethod<char>(2);
    test_MultiGetSetMethod<double>(3);
}

void test_MultiSetGetRemoveM()
{
    test_MultiSetGetRemoveMethod<int>(1);
    test_MultiSetGetRemoveMethod<char>(2);
    test_MultiSetGetRemoveMethod<double>(3);
}

void test_MultiGetSetRemoveM()
{
    test_MultiGetSetRemoveMethod<int>(1);
    test_MultiGetSetRemoveMethod<char>(2);
    test_MultiGetSetRemoveMethod<double>(3);
}
