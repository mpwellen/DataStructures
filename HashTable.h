//
// Created by Michael on 12/5/2018.
//

//
// Created by Michael on 11/28/2018.
//

#include <cstdlib>
#include <iostream>
#include <cstring>
//#include "Primes.cpp"

using namespace std;

#ifndef UNTITLED9_HASHTABLE_H
#define UNTITLED9_HASHTABLE_H

class HashTable {
public:
    bool rehashing;
    bool rehashMode;
    bool transferComplete;
    bool tableComplete;
    static char * const DELETED ;
    double m_loadFactor=0;
    char ** curr;
    char ** H1;
    char ** H2;
    char ** H3;
    int m_size1;
    int m_size2;
    int m_capacity1=0;
    int m_capacity2=0;
    int m_capacity3=0;
    double m_loadFactor1;
    double m_loadFactor2;
    HashTable(int n=107);
    ~HashTable() ;
    unsigned int HashCode(const char *str);
    void insert(const char *str);
    bool find(const char *str);
    char* remove(const char *str);
    void rehash(const char *str);
    void dump();
    HashTable (HashTable& other) ;
    HashTable& operator= (HashTable& rhs) ;
    bool isRehashing() ;
    int tableSize(int table=0) ;
    int size(int table=0) ;
    const char * at(int index, int table=0) ;
    void finalTransition();
    void transferRest();
    int roundUpPrime(int);

};
#endif //UNTITLED8_HASHTABLE_H
