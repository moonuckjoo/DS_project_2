#include "LoanBookHeap.h"

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) { //Arrangement is implemented recursively
	if (pN->getParent() == NULL) {
		return; //condition to exit
	}
	if (pN->getParent()->getBookData()->getName() > pN->getBookData()->getName()) { //if newData is min, updata heap
		//data change
		LoanBookData* tempData = pN->getParent()->getBookData();
		pN->getParent()->setBookData(pN->getBookData());
		pN->setBookData(tempData);
		pN = pN->getParent();
		heapifyUp(pN);
	}
	else { //no sort
		return;
	}
}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) { //Arrangement is implemented recursively
	if ((pN->getLeftChild() == NULL) && (pN->getRightChild() == NULL)) return; //root or leaf case
	else if ((pN->getLeftChild() != NULL) && (pN->getRightChild() != NULL)) { // left and right have Node;
		if ((pN->getLeftChild()->getBookData()->getName()) > (pN->getBookData()->getName()) && (pN->getRightChild()->getBookData()->getName()) > (pN->getBookData()->getName())) {
			if ((pN->getLeftChild()->getBookData()->getName()) > (pN->getRightChild()->getBookData()->getName())) {
				LoanBookData* tempData = pN->getBookData();
				pN->setBookData(pN->getRightChild()->getBookData());
				pN->getRightChild()->setBookData(tempData);
				pN = pN->getRightChild();
				heapifyDown(pN);
			}
			else {
				LoanBookData* tempData = pN->getBookData();
				pN->setBookData(pN->getLeftChild()->getBookData());
				pN->getLeftChild()->setBookData(tempData);
				pN = pN->getLeftChild();
				heapifyDown(pN);
			}
		}

		else if ((pN->getLeftChild()->getBookData()->getName()) < (pN->getBookData()->getName())) {//only left child is small than pN
			LoanBookData* tempData = pN->getBookData();
			pN->setBookData(pN->getLeftChild()->getBookData());
			pN->getLeftChild()->setBookData(tempData);
			pN = pN->getLeftChild();
			heapifyDown(pN);
		}
		else if ((pN->getRightChild()->getBookData()->getName()) < (pN->getBookData()->getName())) {//only right child is small than pN
			LoanBookData* tempData = pN->getBookData();
			pN->setBookData(pN->getRightChild()->getBookData());
			pN->getRightChild()->setBookData(tempData);
			pN = pN->getRightChild();
			heapifyDown(pN);
		}
		else
			return;
	}

	else { // if only one child Node , it's last cyle
		if (pN->getLeftChild() != NULL) {
			if ((pN->getLeftChild()->getBookData()->getName()) < (pN->getBookData()->getName())) {
				LoanBookData* tempData = pN->getBookData();
				pN->setBookData(pN->getLeftChild()->getBookData());
				pN->getLeftChild()->setBookData(tempData);
				pN = pN->getLeftChild();
			}
			else
				return;
		}
		if (pN->getRightChild() != NULL) {
			if ((pN->getRightChild()->getBookData()->getName()) < (pN->getBookData()->getName())) {
				LoanBookData* tempData = pN->getBookData();
				pN->setBookData(pN->getRightChild()->getBookData());
				pN->getRightChild()->setBookData(tempData);
				pN = pN->getRightChild();
			}
			else
				return;
		}
	}
}



bool LoanBookHeap::Insert(LoanBookData* data) {
	LoanBookHeapNode* newNode = new LoanBookHeapNode;
	newNode->setBookData(data); //data set
	this->upsize();
	if (this->root == NULL) {
		root = newNode; //no root
		return true;
	}
	else {
		LoanBookHeapNode* curNode = root;
		int num = getsize();
		int stack[300] = { -1, }; //max height of heap is 10000, initialize 
		for (int i = 0; i < 300; i++) {
			stack[i] = -1;
		}
		int top = -1; //stack index
		//seach Node to insert 
		while (num / 2 != 1) {
			stack[++top] = num % 2;
			num /= 2;
		}
		stack[++top] = num % 2;
		while (top != 0) {
			if (stack[top] == 1) {
				curNode = curNode->getRightChild();
				top--;
			}
			else { //stack[top] == 0
				curNode = curNode->getLeftChild();
				top--;
			}
		}
		if (curNode->getLeftChild() == NULL) { //priority is left > right
			curNode->setLeftChild(newNode);
			newNode->setParent(curNode);
		}
		else {
			curNode->setRightChild(newNode);
			newNode->setParent(curNode);
		}
		heapifyUp(newNode);
		return true;
	}
}









void LoanBookHeap::Delete() {
	LoanBookHeapNode* delNode = this->root;
	int num = getsize();
	int stack[300] = { -1, }; //max height of heap is 300, initialize 
	for (int i = 0; i < 300; i++) {
		stack[i] = -1;
	}
	int top = -1; //stack index
	//seach Node to insert 
	if (num ==1) {
		delNode->setBookData(NULL); //data change
		downsize();
		return;
	}
	while (num / 2 != 1) {
		stack[++top] = num % 2;
		num /= 2;
	}
	stack[++top] = num % 2;
	while (top != -1) {
		if (stack[top] == 1) {
			delNode = delNode->getRightChild();
			top--;
		}
		else { //stack[top] == 0
			delNode = delNode->getLeftChild();
			top--;
		}
	}
	LoanBookHeapNode* rootNode = this->root;
	rootNode->setBookData(delNode->getBookData()); //data change
	heapifyDown(rootNode);
	delNode->setBookData(NULL);
	if (delNode->getParent()->getLeftChild() == delNode) {// delNode is LeftChildNode of parentNode
		delNode->getParent()->setLeftChild(NULL);
		delNode->setParent(NULL);
		delete delNode;
		downsize();
		return;
	}
	else {// delNode is RightChildNode of parentNode
		delNode->getParent()->setRightChild(NULL);
		delNode->setParent(NULL);
		delete delNode;
		downsize();
		return;
	}


}
LoanBookHeapNode* LoanBookHeap::searchNode(int idx) {
	LoanBookHeapNode* curNode = root;
	if (idx == 1) {
		return curNode;
	}
	int stack[300] = { -1, }; //max height of heap is 10000, initialize 
	for (int i = 0; i < 300; i++) {
		stack[i] = -1;
	}
	int top = -1; //stack index
	//seach Node to insert 
	while (idx / 2 != 1) {
		stack[++top] = idx % 2;
		idx /= 2;
	}
	stack[++top] = idx % 2;
	while (top != -1) {
		if (stack[top] == 1) {
			curNode = curNode->getRightChild();
			top--;
		}
		else { //stack[top] == 0
			curNode = curNode->getLeftChild();
			top--;
		}
	}
	return curNode;
};


