#pragma once
#include "LoanBookData.h"
#include "LoanBookHeapNode.h"

class LoanBookHeap
{
private:
    LoanBookHeapNode* root;
    int size;
public:
    LoanBookHeap() {
        this->root = NULL;
        this->size = 0;
    };
    ~LoanBookHeap() {

    }

    void setRoot(LoanBookHeapNode* pN) { this->root = pN; }
    LoanBookHeapNode* getRoot() { return root; }

    void heapifyUp(LoanBookHeapNode* pN);
    void heapifyDown(LoanBookHeapNode* pN);
    void upsize() { this->size += 1; }
    void downsize() { this->size -= 1; }
    int getsize() { return this->size; }
    bool Insert(LoanBookData* data);
    void Delete();
    LoanBookHeapNode* searchNode(int idx);
    

};
