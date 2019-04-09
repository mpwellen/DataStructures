//
// Created by Michael on 12/5/2018.
//

#include "HashTable.h"
//#include "Primes.cpp"

char * const HashTable::DELETED = (char *) &DELETED;

HashTable::HashTable(int n)
{
    rehashing=false;
    rehashMode=false;
    tableComplete=false;
    transferComplete=false;
    m_capacity1=n;
    m_capacity2=0;
    m_capacity3=0;
    m_size1=0;
    m_size2=0;
    m_loadFactor1=0;
    m_loadFactor2=0;
    //n=roundUpPrime(n);// I couldn't compile this right with Primes.cpp
    H1 = (char**)malloc(n * sizeof(char*));
    for(int i=0;i<m_capacity1;i++)
    {
        H1[i] = DELETED;
    }
}

HashTable::~HashTable()
{
    int count=0;
    int i=0;
    while(i<m_capacity1)
    {
        if(strcmp(H1[i],DELETED)!=0)
        {
            count++;
        }
	free(H1[i]);
        i++;
    }
    delete[] H1;
    int j=0;
    int countB=0;
    while(j<m_capacity2)
    {

        if(strcmp(H2[j],DELETED)!=0)
        {
            countB++;
        }
	free(H2[j]);
        j++;
    }
    delete[] H2;
    int k=0;
    int countC=0;
    while(k<m_capacity3)
    {
        if(strcmp(H3[k],DELETED)!=0)
        {
            countC++;
        }
	free(H3[k]);
        k++;
    }
    delete[] H3;
}
void HashTable::insert(const char *str) {

    if(strcmp(str,DELETED)==0)
    {
        return;
    }
    m_loadFactor1=(double)m_size1/(double)m_capacity1;

    if(m_loadFactor2>0.5)
    {
        tableComplete=true;
        finalTransition();
    }
    char *m_str = strdup(str);
    int modulo;
    //define which terms you want to add it to.
    if (m_loadFactor1 < 0.5 && rehashing==false && rehashMode==false) {
        curr = H1;
        modulo = m_capacity1;
    }
    else if(rehashing==true)
    {
        curr=H2;
        modulo=m_capacity2;
    }
    else {
        curr = H2;
        modulo = m_capacity2;
    }
    if (find(str) == 1) {
        cout << "Insert Function: This string is already in the hash table" << endl;
        free(m_str);
        return;
    }
    m_loadFactor1 = ((double) m_size1) / ((double) m_capacity1);//recalculate load factor
    m_loadFactor2 = ((double)m_size2)/((double)m_capacity2);
    int index = HashCode(m_str) % modulo; //calculate index
    int count = 1;
    //add to first array if under 0.5: otherwise, add to second array
    //only add to first array if loadFactor<0.5
    //if(m_loadFactor1<0.5) {
    //loop until you find a free space
    while (strcmp(curr[index],DELETED)!=0 && count < 10) {
        if (index == modulo-1) {
            index = -1;
        }
        index++;
        count++;
    }
    if (count == 10 && rehashing==false) {
        if(transferComplete==false) {
            m_size1++;//MPW TEST
            m_loadFactor1=(double)m_size1/(double)m_capacity1;
            H1[index] = m_str;
            rehash(m_str);
        }
        else
        {
            insert(m_str);
        }
    }
    else if (strcmp(curr[index],DELETED)==0) {
        curr[index] = m_str;
        if(curr==H1) {
            m_size1++;
        }
        else{
            m_size2++;
        }
        m_loadFactor1 = (double) m_size1 / (double) m_capacity1;
        m_loadFactor2 = (double) m_size2/(double)  m_capacity2;
        if(m_loadFactor2>0.5)
        {
            tableComplete=true;
            finalTransition();
        }
    }
}

bool HashTable::find(const char *str)
{
    if(rehashMode==1)
    {
        rehash(str);
    }
    int index = HashCode(str)%m_capacity1;
    bool foundName=0;
    int count=0;
    //Loop until you find a gap, you find what you're looking for, or count gets too high
    while(strcmp(H1[index],DELETED) && foundName==false && count<10)
    {
        if(strcmp(str,H1[index])==0)
        {
            foundName=true;
            return foundName;
        }
        //loop to beginning of array if it's at the end
        if(index==m_capacity1-1)
        {
            index=-1;
        }
        index++;
        count++;
    }
    //Loop until you find a gap, you find what you're looking for, or count gets too high
    if(rehashMode==true) {
        int index=HashCode(str)%m_capacity2;
        while (strcmp(H2[index],DELETED)!=0 && foundName == false && count < 10) {
            if (strcmp(str, H2[index]) == 0) {
                foundName = true;
                return foundName;
            }
            //loop to beginning of array if it's at the end
            if (index == m_capacity1 - 1) {
                index = -1;
            }
            index++;
            count++;
        }
    }
    foundName=false;
    if(m_loadFactor2>0.5)
    {
        tableComplete=true;
        finalTransition();
    }
    return foundName;
}

char* HashTable::remove(const char *str) {
    if(strcmp(str,DELETED)==0)
    {
        return DELETED;
    }
    //char *m_str = _strdup(str);
    m_loadFactor1=(double)m_size1/(double)m_capacity1;
    if(m_loadFactor1>0.5 && rehashing==false)
    {
        rehash(str);
    }
    int index = HashCode(str) % m_capacity1;
    //cout << "New Index" << index << endl;
    bool foundName = false;
    int count = 0;
    bool nextArray=false;
    while ((strcmp(H1[index],DELETED)!=0 || foundName == false)&&nextArray==false) {
        if (strcmp(str, H1[index]) == 0) {
            //cout << "Item removed: " << H1[index] << endl;
            char* str=H1[index];
            H1[index] = DELETED;
            m_size1--;
            return str;
        }
        if (index == m_capacity1 - 1) {
            index = -1;
        }
        index++;
        count++;
        if(count>10)
        {
            nextArray=true;
        }
    }
    index = HashCode(str) % m_capacity2;
    while ((strcmp(H2[index], DELETED)!=0 || foundName == false)&&nextArray==true) {
        if (strcmp(str, H2[index]) == 0) {
            char* m_str=H2[index];
            H2[index] = DELETED;
            m_size2--;
            return m_str;
        }
        if (index == m_capacity1 - 1) {
            index = -1;
        }
        index++;
        count++;
        if(count>10)
        {
            //cout << "Item not found in array two" << endl;
            char* m_str=DELETED;
            return m_str;
        }
        return DELETED;
    }
}

unsigned int HashTable::HashCode(const char *str)
{
    unsigned int val = 0 ;
    const unsigned int thirtyThree = 33 ; // magic number from textbook
    int i = 0 ;
    while (str[i] != '\0')
    {
        val = val * thirtyThree + str[i] ;
        i++ ;
    }
    return val ;
}

void HashTable::dump()
{

    cout << "Dump Function" << endl;
    cout << "M_Capacity 1: "<<m_capacity1 << endl;
    cout << "M_Size 1: "<<m_size1<< endl;
    cout << "Load Factor 1: "<< m_loadFactor1 << endl;
    cout << "M_Capacity 2: " << m_capacity2 << endl;
    cout << "M_Size 2: " << m_size2 << endl;
    cout << "Load Factor 2: " << m_loadFactor2 << endl;
    int count=0;
    int i=0;
    while(i<m_capacity1)
    {
        //cout << H1[i] << endl;
        if(strcmp(H1[i],DELETED)!=0)
        {
            cout << "H1["<<i<<"]: "<<H1[i] << " , ";
            count++;
        }
        i++;
    }
    //cout << "Load Factor 1 Check: " << (double)count/(double)m_capacity1 << endl;
    int j=0;
    int countB=0;
    while(j<m_capacity2)
    {
        //cout << H1[i] << endl;
        //cout << j << endl;
        if(strcmp(H2[j],DELETED)!=0)
        {
            cout << "H2["<<j<<"]: "<<H2[j] << " , ";
            countB++;

        }
        j++;
    }
    //cout << "Size2 Check: " << countB << endl;
    int k=0;
    int countC=0;
    while(k<m_capacity3)
    {
        if(strcmp(H3[k],DELETED)!=0)
        {
            cout << "H3["<<k<<"]: "<<H3[k] << " , ";
            countC++;
        }
        k++;
    }
}

void HashTable::rehash(const char *str) {
    rehashing = true;
    rehashMode = true;
    if(strcmp(str,DELETED)==0)
    {
        return;
    }
    if (m_capacity2 == 0) {
        m_capacity2 = m_size1 * 4;

        m_size2 = 0;
        H2 = (char **) malloc(m_capacity2 * sizeof(char *));
        for (int k = 0; k < m_capacity2; k++) {
            H2[k] = DELETED;
        }

    }

    int index1 = HashCode(str) % m_capacity1;
    int index2 = HashCode(str) % m_capacity2;

    while (strcmp(H1[index1],DELETED)!=0) {
        if (index1 == 0) {
            index1 = m_capacity1;
        }
        index1--;
    }

    index1++;
    int count=0;
    while (strcmp(H1[index1],DELETED)!=0 && count<10) {
        count++;
        if (index1 == m_capacity1 - 1) {
            index1 = 0;
        }
        char *m_strB = remove(H1[index1]);
        insert(m_strB);
        index1++;
        count++;
    }
    rehashing = false;
    m_loadFactor1=(double)m_size1/(double)m_capacity1;
    if(m_loadFactor2>0.5)
    {
        tableComplete=true;
        finalTransition();
    }
    else if(m_loadFactor1<0.03&& m_loadFactor1!=0 && m_loadFactor1!=0 && rehashMode==true)
    {
        transferRest();
    }
}


HashTable::HashTable (HashTable& other)
{
    if(other.rehashing==true)
    {
        other.transferRest();
        other.rehashing=false;
    }
    m_capacity1=other.m_capacity1;
    m_capacity2=other.m_capacity2;
    m_capacity3=other.m_capacity3;
    m_size1=other.m_size1;
    m_size2=other.m_size2;
    rehashing=other.rehashing;
    rehashMode=other.rehashMode;
    transferComplete=other.transferComplete;
    tableComplete=other.tableComplete;
    m_loadFactor=other.m_loadFactor;
    m_loadFactor1=other.m_loadFactor1;
    m_loadFactor2=other.m_loadFactor2;
    curr=other.curr;
    H1 = (char**)malloc(m_capacity1 * sizeof(char*));
    H2 = (char**)malloc(m_capacity2 * sizeof(char*));
    H3 = (char**)malloc(m_capacity3 * sizeof(char*));
    for(int i=0;i<m_capacity1;i++) {
        H1[i] = strdup(other.H1[i]);
    }
    for(int j=0;j<m_capacity2;j++)
    {
        H2[j]=strdup(other.H2[j]);
    }
    cout << "COPY" << endl;

}
HashTable& HashTable::operator= (HashTable& rhs)
{
    if(rhs.rehashing==true)
    {
        rhs.transferRest();
        rhs.rehashing=false;
    }
    m_capacity1=rhs.m_capacity1;
    m_capacity2=rhs.m_capacity2;
    m_capacity3=rhs.m_capacity3;
    m_size1=rhs.m_size1;
    m_size2=rhs.m_size2;
    rehashing=rhs.rehashing;
    rehashMode=rhs.rehashMode;
    transferComplete=rhs.transferComplete;
    tableComplete=rhs.tableComplete;
    m_loadFactor=rhs.m_loadFactor;
    m_loadFactor1=rhs.m_loadFactor1;
    m_loadFactor2=rhs.m_loadFactor2;
    curr=rhs.curr;
    H1 = (char**)malloc(m_capacity1 * sizeof(char*));
    H2 = (char**)malloc(m_capacity2 * sizeof(char*));
    H3 = (char**)malloc(m_capacity3 * sizeof(char*));
    for(int i=0;i<m_capacity1;i++)
    {
        H1[i]=strdup(rhs.H1[i]);
    }
    for(int j=0;j<m_capacity2;j++)
    {
        H2[j]=strdup(rhs.H2[j]);
    }
    for(int k=0;k<m_capacity3;k++)
    {
        H3[k]=strdup(rhs.H3[k]);
    }
    return rhs;
}

bool HashTable::isRehashing()
{
    return rehashing;
}
int HashTable::tableSize(int table)
{
    if(rehashing==true)
    {
        return m_capacity1*2;
    }
    else
    {
        if(table==0)
        {
            return m_capacity1;
        }
        else{
            return m_capacity2;
        }
    }
}
int HashTable::size(int table)
{
    if(rehashing)
    {
        return m_size1+m_size2;
    }
    else{
        if(table==0)
        {
            return m_size1;
        }
        else
        {
            return m_size2;
        }
    }
}
const char * HashTable::at(int index, int table)
{
    if(table==0)
    {
        return (char*)H1[index];
    }
    else
    {
        return (char*)H2[index];
    }
}

void HashTable::finalTransition()
{
    cout << m_loadFactor1 << endl;
    cout << m_loadFactor2 << endl;
    m_capacity3=(m_size1+m_size2)*4;
    H3 = (char**)malloc(m_capacity3* sizeof(char*));
    m_capacity3=(m_size1+m_size2)*4;
    for(int z=0;z<m_capacity3;z++)
    {
        H3[z]=DELETED;
    }
    int l=0;
    for(int i=0;i<m_capacity1;i++)
    {
        if(strcmp(H1[i],DELETED)!=0)
        {
            H3[l]=strdup(H1[i]);
            H1[i]=DELETED;
            m_size1--;
            l++;
        }
    }
    for(int j=0;j<m_capacity2;j++)
    {
        if(strcmp(H2[j],DELETED)!=0)
        {
            H3[l]=strdup(H2[j]);
            H2[j]=DELETED;
            m_size2--;
            l++;
        }
    }
}

void HashTable::transferRest()
{
    cout << "Transfer Rest of H1" << endl;
    cout << m_loadFactor1 << endl;
    cout << m_loadFactor2 << endl;

    int l=0;
    for(int i=0;i<m_capacity1;i++)
    {
        if(strcmp(H1[i],DELETED)!=0)
        {
            insert(remove(H1[i]));
        }
    }
    m_loadFactor1=(double)m_size1/(double)m_capacity1;
    transferComplete=true;
}
