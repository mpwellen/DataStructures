//
// Created by Michael on 11/14/2018.
//
#ifndef Median_HEAP_CPP
#define Median_HEAP_CPP

#include "MedianHeap.h"


//Constructor for Heap. Feeds in "less than" or "greater than" from main file depending on whether maxHeap or minHeap.
//The function pointer uses whichever one was fed in. This doesn't change the rest of the functions, since what is
//really important is whether descendants are further from MedianHeap's median or not. The Heap Capacity is
//equal to the Median Heap Capacity divided by 2 plus 1. The extra one allows the re-organization functions to work.
template <typename T>
Heap<T>::Heap(int cap,bool (*lt) (const T&, const T&))
{
    fptr=lt;
    heap_size = 0;
    m_capacity = cap;
    harr=new T[cap];
}

//copy constructor
template <typename T>
Heap<T>::Heap(const Heap<T>& other)
{
    fptr=other.fptr;
    m_capacity = other.m_capacity;
    harr=new T[m_capacity];
    extreme=other.extreme;

    for (int i = 1; i < m_capacity; i++)
    {
        harr[i] = other.harr[i];
    }
    heap_size = other.heap_size;
}

//Destructor
template <typename T>
Heap<T>::~Heap()
{
    delete[] harr;
}

//Starting with i, swaps down to leaves based on heap rules. "Smallest" is used to indicate furthest from root, owing to
//original MaxHeap that was used as template to create MinHeap.
template <typename T>
void Heap<T>::Heapify(int i)
{
    int smallest;
    int l = left(i);
    //cout << "left: "<<harr[l] << endl;
    int r = right(i);
    //cout << "right: "<<harr[r]<<endl;

    smallest = i;
    if (l <= heap_size && !fptr(harr[l], harr[i])) {
        smallest = l;
        //cout << "left is greater than root" << endl;
    }
    if (r <= heap_size && !fptr(harr[r], harr[smallest])) {
        smallest = r;
        //cout << "right is greater than root" << endl;
    }

    if (smallest != i) {
        //cout << "FURTHEST FROM ROOT" << smallest << endl;
        swap(&harr[i], &harr[smallest]);
        Heapify(smallest);
    }
}

//returns location of parent of node in i location. Based on formula. leftNode=parentNode*2, and rightNode=parentNode*2+1.
//This means that the parent node is leftNode divided by 2. Since C++ rounds down, rightNode/2 also gives parent node.
template <typename T>
int Heap<T>::parent(int i)
{
    return i/2;
}

//returns array location of left child of i. See description above.
template <typename T>
int Heap<T>::left(int i)
{
    return(2*i);
}

//returns array location of right child of i. See description above.
template <typename T>
int Heap<T>::right(int i)
{
    return(2*i+1);
}

//Extracts Root by replacing root with last value, reheapifying, and then reducing heap_size by 1.
template <typename T>
T Heap<T>::extractRoot() {
    //cout << "START OF EXTRACT MIN FUNCTION" << endl;

    if (heap_size <= 0) {
        throw std::out_of_range("Why is the heap size below zero?");
    }

    if (heap_size == 1) {
        heap_size--;
        return harr[1];
    }

    // Store the minimum value, and remove it from heap
    T root = harr[1];
    harr[1]=harr[heap_size];
    heap_size--;
    Heapify(1);

    return root; //root
}

//Root will always be harr[1]
template <typename T>
T Heap<T>::getRoot()
{
    return harr[1];
}

//Finds item, replaces with last item in heap, heapifies (similar to extractRoot), and then returns true. If item not
//found, simply returns false instead.
template <typename T>
bool Heap<T>::deleteKey(T givenItem,bool (*lt) (const T&, const T&))
{
    //cout << "START OF DELETE FUNCTION" << endl;
    eqptr=lt;
    int index=32767; //using largest allowed index as dummy variable to indicate falseness. Will be replaced if true.
    for(int i=1;i<=heap_size;i++)
    {
        if(eqptr(harr[i],givenItem))
        {
            index=i;
        }
    }
    if(index==32767)
    {
        return false;
    }
    if (heap_size <= 0) {
      throw std::out_of_range("Can't Delete Item from Empty Heap");
        return 0;
    }

    if (heap_size == 1) {
        heap_size--;
        return true;
    }

    // Store the minimum value, and remove it from heap
    T root = harr[index];
    harr[index]=harr[heap_size];
    heap_size--;
    Heapify(index);

    return true; //root
}

//inserts item to end. Then "heapifies" upwards until heap condition met. Heapify function sorts downward,
//so that wouldn't work here.
template <typename T>
void Heap<T>::insertKey(T k)
{

    //cout << "START OF Heap INSERTKEY" << endl;
    //cout << k << endl;
    //heap_size++;

    if (heap_size+1 == m_capacity) {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }
    heap_size++;
    // First insert the new key at the end

    int i=heap_size;
    harr[i] = k;

    // Fix the min heap property if it is violated
    while (i != 1 && fptr(harr[parent(i)],harr[i])) //The second parameter here decides whether max or min is put at harr[1]
    {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
    if(heap_size==1)
    {
        extreme=harr[1];
    }
    if(fptr(k,extreme))
    {
        extreme=k;
    }
}

//utility function to find item. I didn't actually use this in my project, but it's useful to have around.
template <typename T>
T Heap<T>::findItem(T item)
{
    for(int i=0;i<heap_size;i++)
    {
        if(eqptr(harr[i],item))
        {
            return item;
        }
    }
}

//simple utility function to swap values
template <typename T>
void Heap<T>::swap(T *x, T *y)
{
    //cout << "swap" << endl;
    T temp=*x;
    *x=*y;
    *y=temp;
}

//dumps out size of Heap, followed by all items of Heap's array.
template <typename T>
void Heap<T>::dump()
{
    cout << "SIZE OF HEAP IS: "<< heap_size << endl;
    //cout << "Value Furthest From Median is: " << extreme  << endl;
    for (int i = 1; i < heap_size; i++)
    {
      cout << "Heap["<<i<<"] =  " << harr[i] << endl;
    }
}

//Assignment Operator for Heaps
template <typename T>
const Heap<T>& Heap<T>::operator=(const Heap<T>& rhs)
{
    if(this==&rhs)
    {
        return *this;
    }
    m_capacity = rhs.m_capacity;
    heap_size = rhs.heap_size;
    fptr=rhs.fptr;
    extreme=rhs.extreme;
    for (int i = 1; i < m_capacity; i++)
    {
        harr[i] = rhs.harr[i];
    }
    return *this;
}

//returns capacity
template <typename T>
int Heap<T>::capacity()
{
    return m_capacity;
}

//return size
template <typename T>
int Heap<T>::size()
{
    return heap_size;
}

//looks through array to found the smallest value
template <typename T>
T Heap<T>::getMin()
{
    /*int min=1;
    for(int i=1;i<=size();i++)
    {
        if(fptr(harr[i],harr[min]))
        {
            min=i;
        }
    }
    return harr[min];*/
    return extreme;
}

//looks through array to get the biggest value.
template <typename T>
T Heap<T>::getMax()
{
    /*int max=1;
    for(int i=1;i<=size();i++)
    {
        if(fptr(harr[i],harr[max]))
        {
            max=i;
        }
    }
    return harr[max];*/
    return extreme;
}

//Constructor. The goofy Heap Constructors past the colon are to prevent the compiler from grabbing the empty constructor,
//which it will want to do because of the templatization.
template <typename T>
MedianHeap<T>:: MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&),int cap) : minHeap((cap/2)+2,gt),maxHeap((cap/2)+2,lt)
{
    // gt gives root with the biggest value. Therefore, it is maxHeap.
    // lt gives root with the smallest value. Therefore, it is minHeap.
    lptr=lt;
    gptr=gt;
    mh_capacity = cap;
    mh_heap_size = 0;//minHeap.size()+maxHeap.size();
}

//MedianHeap copy constructor
template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap<T>& otherH) : minHeap(otherH.minHeap),maxHeap(otherH.maxHeap)
{
    //cout << "MPW Copy Constructor" << endl;
    mh_capacity=otherH.mh_capacity;
    //cout << "MPW Copy Constructor" << endl;
    maxHeap=otherH.maxHeap;
    minHeap=otherH.minHeap;
    mh_heap_size=otherH.mh_heap_size;
    lptr=otherH.lptr;
    gptr=otherH.gptr;
    median=otherH.median;
}

//Median Heap Assignment Operator.
template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator=(const MedianHeap<T>& rhs)
{
    if(this==&rhs)
    {
        return *this;
    }
    maxHeap=rhs.maxHeap;
    minHeap=rhs.minHeap;

    mh_heap_size=rhs.mh_heap_size;
    mh_capacity=rhs.mh_capacity;
    lptr=rhs.lptr;
    gptr=rhs.gptr;
    median=rhs.median;
    return *this;
}

//returns size of median heap
template <typename T>
int MedianHeap<T>::size()
{
    return mh_heap_size;
}

//returns capacity
template <typename T>
int MedianHeap<T>::capacity()
{
    return mh_capacity;
}

//Calls InsertKey. If the results cause the heaps to be more than one different insize, extracts the root from bigger heap and
//inserts it into the smaller heap. If it causes one heap to be bigger than the other, than that heap's root
//is assigned to the median.
template <typename T>
void MedianHeap<T>::insert(T item)
{
    //cout << "Beginning of MH insert function" << endl;
    //cout << "Median is" << median << endl;
    //cout << "item is " << item << endl;
    if(mh_heap_size==0)
    {
        maxHeap.insertKey(item);
        median=maxHeap.getRoot();
        mh_heap_size++;
    }
    else if(lptr(item,median))
    {
        mh_heap_size++;

        maxHeap.insertKey(item);
        //cout << "ITEM " << item<< " INSERTED INTO MAX HEAP" << endl;
    }
    else if(gptr(item,median))
    {
        mh_heap_size++;
        minHeap.insertKey(item);
        //cout << "ITEM "<<item<<" INSERTED INTO MIN HEAP" << endl;
    }
    //Re-organize if one heap size is too big.
    if(maxHeap.heap_size-minHeap.heap_size>1)
    {
        minHeap.insertKey(maxHeap.extractRoot());
        median=maxHeap.getRoot();
    }
    else if(minHeap.heap_size-maxHeap.heap_size>1)
    {
        maxHeap.insertKey(minHeap.extractRoot());
        median=minHeap.getRoot();
    }
    else if(maxHeap.heap_size-minHeap.heap_size==1)
    {
        median=maxHeap.getRoot();
    }
    else if(minHeap.heap_size-maxHeap.heap_size==1)
    {
        median=minHeap.getRoot();
    }
}

template <typename T>
T MedianHeap<T>::getMedian()
{
    //cout << "getMedian Running" << endl;
    return median;
}

//The MinHeap is the top heap. Therefore, max of median heap will be max of MinHeap.
template <typename T>
T MedianHeap<T>::getMax(){
    return minHeap.getMax();
}

//The MaxHeap is the bottom heap. therefore, min of median heap will be min of MaxHeap.
template <typename T>
T MedianHeap<T>::getMin(){
    return maxHeap.getMin();
}

//Calls DeleteKey. If the results cause the heaps to be more than one different insize, extracts the root from bigger heap and
//inserts it into the smaller heap. If it causes one heap to be bigger than the other, than that heap's root
//is assigned to the median.
template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) )
{
    T num;
    bool testmin=false;
    bool testmax=false;
    //cout << "MPW Delete" << endl;
    for(int i=1;i<=minHeap.size();i++)
    {
        if (equalTo(givenItem, minHeap.harr[i])) {
            //mh_heap_size--;
            testmin = minHeap.deleteKey(givenItem, equalTo);
            //cout << "MPW" << testmin<<endl;
            //return test;
        }
    }
    //cout << "MPW" << endl;
    for(int i=1;i<=maxHeap.size();i++) {
         if (lptr(givenItem, maxHeap.harr[i])) {
             //mh_heap_size--;
             testmax = maxHeap.deleteKey(givenItem, equalTo);
            //return test;
        }
    }
    //Re-organize if one heap size is too big.
    if(maxHeap.heap_size-minHeap.heap_size>1)
    {
        minHeap.insertKey(maxHeap.extractRoot());
        median=maxHeap.getRoot();
    }
    else if(minHeap.heap_size-maxHeap.heap_size>1)
    {
        maxHeap.insertKey(minHeap.extractRoot());
        median=minHeap.getRoot();
    }
    else if(maxHeap.heap_size-minHeap.heap_size==1)
    {
        median=maxHeap.getRoot();
    }
    else if(minHeap.heap_size-maxHeap.heap_size==1)
    {
        median=minHeap.getRoot();
    }
    //MH_HEAP_SIZE IS GETTING SCREWED UP SOMEWHERE UNKNOWN, BUT THIS SHOULD CORRECT IT.
    mh_heap_size=minHeap.size()+maxHeap.size();
    return (testmin||testmax);
}

template <typename T>
void MedianHeap<T>::dump()
{
    cout << "Max Heap" << endl;
    maxHeap.dump();
    cout << "Min Heap" << endl;
    minHeap.dump();
    cout << "Min" << maxHeap.getMin() << endl;
    cout << "Max" << minHeap.getMax() << endl;
    cout << "Median" << getMedian() << endl;
}

template <typename T>
int MedianHeap<T>::maxHeapSize()
{
    return maxHeap.size();
}

template <typename T>
int MedianHeap<T>::minHeapSize()
{
    return minHeap.size();
}

//I just searched with for loop through harr when I needed thsi functionality, but this would've been smarter.
template <typename T>
T MedianHeap<T>::locateInMaxHeap(int pos)
{
    if(pos>maxHeap.heap_size || pos<1)
    {
        cout << "That position is not in the max heap" << endl;
        throw std::out_of_range ("NOT IN RANGE");
    }
    return maxHeap.harr[pos];
    //return T
}

template <typename T>
T MedianHeap<T>::locateInMinHeap(int pos)
{
    if((pos>minHeap.heap_size) || pos<1)
    {
        cout << "That position is not in the min heap" << endl;
        throw std::out_of_range ("NOT IN RANGE");
    }
    return minHeap.harr[pos];

}

#endif
