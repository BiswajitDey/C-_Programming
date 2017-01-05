/*
 * threadLocal.h
 *  Start with this and add what is necessary
 */

#ifndef THREADLOCAL_H_
#define THREADLOCAL_H_

#include <iostream>
#include <mutex>
#include <unordered_map>
#include <thread>

namespace cop5618 {

 template <typename T>
class threadLocal {
public:
	threadLocal();
	~threadLocal();

	//disable copy, assign, move, and move assign constructors
	 threadLocal(const threadLocal&)=delete;
	 threadLocal& operator=(const threadLocal&)=delete;
	 threadLocal(threadLocal&&)=delete;
	 threadLocal& operator=(const threadLocal&&)=delete;

	 /**
	 * Returns the current thread's value.
	 * If no value has been previously set by this
	 * thread, an out_of_range exception is thrown.
	 */
	const T& get() const;


	/**
	 * Sets the value of the threadLocal for the current thread
	 * to val.
	 */
	void set(T val);

	/**
	 * Removes the current thread's value for the threadLocal
	 */
	void remove();

	/**
	 * Friend function.  Useful for debugging only
	 */
	template <typename U>
	friend std::ostream& operator<< (std::ostream& os, const threadLocal<U>& obj);

private:
    std::unordered_map< std::thread::id,T> tIdtLocalMap;
    mutable std::mutex mtx;
};

template <typename U>  std::ostream& operator<< (std::ostream& os, const threadLocal<U>& obj) {
     for(auto elem : obj.tIdtLocalMap) {
           os << " [" << elem.first << " -> " << elem.second << "] ";
       }
       return os;
}

template<typename T>  threadLocal<T>::threadLocal()  {
    std::thread::id this_id = std::this_thread::get_id();
}
template<typename T>  threadLocal<T>::~threadLocal()  {
    std::lock_guard <std::mutex> guard(mtx);
    std::thread::id this_id = std::this_thread::get_id();
    this->tIdtLocalMap.clear();
}

template<typename T> const T& threadLocal<T>::get() const {
    std::lock_guard <std::mutex> guard(mtx);
    std::thread::id this_id = std::this_thread::get_id();
    try {
        if( tIdtLocalMap.find(this_id) != tIdtLocalMap.end() ) {
            std::cout << "-> Thread data found. Thread Id : "  << this_id << " Value :  "<< tIdtLocalMap.find(this_id)->second<< std::endl;
            return tIdtLocalMap.find(this_id)->second;
        } else {
            throw std::out_of_range("Out_of_range:No thread-data for given thread!");
        }
    } catch(std::out_of_range oe) {
            throw oe;
    } catch(std::exception e) {
            throw e;
    }
}

template<typename T> void threadLocal<T>::remove() {
    std::lock_guard <std::mutex> guard(mtx);
    std::thread::id this_id = std::this_thread::get_id();

    try {
        if( tIdtLocalMap.find(this_id) == tIdtLocalMap.end() ) {
            throw std::exception();
        } else {
            tIdtLocalMap.erase(this_id);
        }
    } catch(std::exception e) {
        throw e;
    }
}

template<typename T> void threadLocal<T>::set(T val) {
    std::lock_guard <std::mutex> guard(mtx);
    std::thread::id this_id = std::this_thread::get_id();

    try {
        if( tIdtLocalMap.find(this_id) == tIdtLocalMap.end() ) {
            tIdtLocalMap[this_id]= val;
        } else {
            tIdtLocalMap[this_id] = val;
        }
    } catch(std::exception e) {
        throw e;
    }
}

} /* namespace cop5618 */

#endif /* THREADLOCAL_H_ */
