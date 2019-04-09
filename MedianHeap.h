//
// Created by Michael on 11/14/2018.
//

#ifndef UNTITLED5_MEDIANHEAP_H
#define UNTITLED5_MEDIANHEAP_H
#include <iostream>
//#include "MedianHeap.cpp"
/* Michael Wellen
 * Project 4
 * This project creates a "MedianHeap" that allows easy access to the median, minimum, and maximum of a set of data.
 * The data can be of any type, so longs as "less than" and "greater" than functions are fed in.
 * The Median Heap is not a true Heap in and of itself, but it contains 2 Heaps, a max heap and a min heap. The
 * median will always be the root of one of these heaps. The max heap has as it's root the largest value in the
 * bottom half (root is largest number), and the min heap has as it's root the smallest value in the top half.
 *
 */
using namespace std;

template <typename T>
class Heap {

public:
    // Constructor
    Heap();
    Heap(int cap,bool (*lt) (const T&, const T&));
    Heap(const Heap<T>& other);
    ~Heap();
    void Heapify(int i);
    void HeapifyMax(int i);
    int parent(int i);
    int left(int i);
    int right(int i);
    T extractRoot();
    T getRoot();
    bool deleteKey(T i,bool (*lt) (const T&, const T&));
    void insertKey(T k);
    T findItem(T k);
    void swap(T *x,T *y);
    void dump();
    const Heap<T>& operator=(const Heap<T>& rhs);
    int capacity();
    int size();
    T getMin();
    T getMax();

    T * harr; // pointer to array of elements in heap
    int m_capacity; // maximum possible size of min heap
    int heap_size = 0; // Current number of elements in min heap
    bool (*fptr)(const T&,const T& );
    bool (*eqptr)(const T&,const T&);
    T extreme;
};
//#include "MedianHeap.cpp"

template <typename T>
class MedianHeap {
public:

    MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap=100 ) ;

    MedianHeap(const MedianHeap<T>& otherH) ;

    const MedianHeap<T>& operator=(const MedianHeap<T>& rhs) ;

    int size() ;

    int capacity() ;

    void insert(T item) ;

    T getMedian() ;

    T getMin() ;

    T getMax() ;

    bool deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ) ;

    void dump() ;

    int maxHeapSize() ;

    int minHeapSize() ;

    T locateInMaxHeap(int pos) ;

    T locateInMinHeap(int pos) ;

    int mh_capacity; // maximum possible size of min heap
    int mh_heap_size = 0; // Current number of elements in min heap
    T median;
    bool (*lptr)(const T&,const T& );
    bool (*gptr)(const T&,const T&);
    Heap<T> minHeap{};
    Heap<T> maxHeap{};
    friend class Heap<T>;

};
#include "MedianHeap.cpp"
#endif //UNTITLED5_MEDIANHEAP_H
