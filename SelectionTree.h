#pragma once
#include "SelectionTreeNode.h"

#include <fstream>
#include <iostream>

class SelectionTree
{
private:
    SelectionTreeNode* root;
    ofstream* fout;
    int size = 0;
    LoanBookHeap* heap_000 = new LoanBookHeap;
    LoanBookHeap* heap_100 = new LoanBookHeap;
    LoanBookHeap* heap_200 = new LoanBookHeap;
    LoanBookHeap* heap_300 = new LoanBookHeap;
    LoanBookHeap* heap_400 = new LoanBookHeap;
    LoanBookHeap* heap_500 = new LoanBookHeap;
    LoanBookHeap* heap_600 = new LoanBookHeap;
    LoanBookHeap* heap_700 = new LoanBookHeap;

public:
    SelectionTree(ofstream* fout) {
        this->root = NULL;
        this->fout = fout;
        newSelectionTree();
    }
    ~SelectionTree() {
        delete heap_000;
        delete heap_100;
        delete heap_200;
        delete heap_300;
        delete heap_400;
        delete heap_500;
        delete heap_600;
        delete heap_700;
    }
    void newSelectionTree();
    void upsize() { size += 1; }
    int getsize() { return this->size; }
    void setRoot(SelectionTreeNode* pN) { this->root = pN; }
    SelectionTreeNode* getRoot() { return root; }

    bool Insert(LoanBookData* newData);
    bool Delete();
    bool printBookData(int bookCode);
    SelectionTreeNode* searchNode(int idx);
    string changeZero(int Code); //if code ==0 , 0 to 000
    void AorderTree(SelectionTreeNode* curNode);

    SelectionTreeNode* searchParentNode(int idx);
    void initializeTree();
};