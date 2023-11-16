#include "SelectionTree.h"

bool SelectionTree::Insert(LoanBookData* newData) {
	int idx = 0;
	//search index of Node
	if (newData->getCode() == 0) idx = 8;
	else if (newData->getCode() == 100) idx = 9;
	else if (newData->getCode() == 200) idx = 10;
	else if (newData->getCode() == 300) idx = 11;
	else if (newData->getCode() == 400) idx = 12;
	else if (newData->getCode() == 500) idx = 13;
	else if (newData->getCode() == 600) idx = 14;
	else if (newData->getCode() == 700) idx = 15;
	SelectionTreeNode* curNode = searchNode(idx);
	curNode->getHeap()->Insert(newData); // heap insert
	curNode->setBookData(curNode->getHeap()->getRoot()->getBookData()); //curNode->setData

	AorderTree(curNode);

	//vesus Node
	return true;
}

void SelectionTree::AorderTree(SelectionTreeNode* curNode) {
	SelectionTreeNode* rivalNode = NULL;;
	while (curNode != this->root) {
		if (curNode->getParent()->getLeftChild() == curNode)  //curNode is leftChild
			rivalNode = curNode->getParent()->getRightChild();

		else if (curNode->getParent()->getRightChild() == curNode) //curNode is rightChild
			rivalNode = curNode->getParent()->getLeftChild();

		//curNode VS rivalNode , data of winner is data of ParentNode
		if ((rivalNode->getBookData() == NULL) && (curNode->getBookData() == NULL))
			break;
		else if (curNode->getBookData() == NULL) {
			curNode->getParent()->setBookData(rivalNode->getBookData());
			curNode = curNode->getParent();
		}
		else if (rivalNode->getBookData() == NULL) {
			curNode->getParent()->setBookData(curNode->getBookData());
			curNode = curNode->getParent();
		}

		else if (curNode->getBookData()->getName() > rivalNode->getBookData()->getName()) { //rivalNode win
			curNode->getParent()->setBookData(rivalNode->getBookData());
			curNode = curNode->getParent();
		}
		else { //curNode < rivalNode
			curNode->getParent()->setBookData(curNode->getBookData());
			curNode = curNode->getParent();
		}
	} //versus
}

SelectionTreeNode* SelectionTree::searchNode(int idx) {
	SelectionTreeNode* curNode = this->root;
	if (idx == 1) {
		return curNode;
	}
	int stack[300];
	for (int i = 0; i < 300; i++) {
		stack[i] = -1;
	}
	int top = -1; //stack index

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
}
bool SelectionTree::Delete() {
	if (this->root->getBookData() == NULL) {

		return false;
	}
	else {// delete
		int idx = 0;
		SelectionTreeNode* delNode = root;
		int stack[300] = { -1 };
		for (int i = 0; i < 300; i++) {
			stack[i] = -1;
		}
		int top = -1; //stack index
		//search index of Node
		if (root->getBookData()->getCode() == 0) idx = 8;
		else if (root->getBookData()->getCode() == 100) idx = 9;
		else if (root->getBookData()->getCode() == 200) idx = 10;
		else if (root->getBookData()->getCode() == 300) idx = 11;
		else if (root->getBookData()->getCode() == 400) idx = 12;
		else if (root->getBookData()->getCode() == 500) idx = 13;
		else if (root->getBookData()->getCode() == 600) idx = 14;
		else if (root->getBookData()->getCode() == 700) idx = 15;
		delNode->setBookData(NULL);
		while (idx / 2 != 1) {
			stack[++top] = idx % 2;
			idx /= 2;
		}
		stack[++top] = idx % 2;
		while (top != -1) {
			if (stack[top] == 1) {
				delNode = delNode->getRightChild();
				delNode->setBookData(NULL);
				top--;
			}
			else { //stack[top] == 0
				delNode = delNode->getLeftChild();
				delNode->setBookData(NULL);
				top--;
			}
		}
		delNode->getHeap()->Delete(); //heap root delete and reorder
		delNode->setBookData(delNode->getHeap()->getRoot()->getBookData()); //newdata set
		AorderTree(delNode); //selection tree reorder
	}
	return true;
}

bool SelectionTree::printBookData(int bookCode) {
	if (root->getBookData() == NULL) {
		*fout << "=========ERROR=========" << endl;
		*fout << "500" << endl;
		*fout << "=======================" << endl << endl;
		return false;
	}
	int idx;
	//search index of Node
	if (bookCode == 0) idx = 8;
	else if (bookCode == 100) idx = 9;
	else if (bookCode == 200) idx = 10;
	else if (bookCode == 300) idx = 11;
	else if (bookCode == 400) idx = 12;
	else if (bookCode == 500) idx = 13;
	else if (bookCode == 600) idx = 14;
	else if (bookCode == 700) idx = 15;
	else { //bookCode is wrong
		*fout << "=========ERROR=========" << endl;
		*fout << "500" << endl;
		*fout << "=======================" << endl << endl;
		return false;
	}

	SelectionTreeNode* curNode = searchNode(idx);
	if (curNode->getHeap()->getRoot() == NULL) { //heap is empty
		*fout << "=========ERROR=========" << endl;
		*fout << "500" << endl;
		*fout << "=======================" << endl << endl;
		return false;
	}
	map <string, LoanBookData*> Aorder;
	LoanBookHeapNode* printNode = curNode->getHeap()->getRoot();
	for (int i = 1; i <= curNode->getHeap()->getsize(); i++) {
		printNode = curNode->getHeap()->searchNode(i);
		Aorder.insert(map<string, LoanBookData*>::value_type(printNode->getBookData()->getName(), printNode->getBookData()));
	}
	if(Aorder.size()==0){
		*fout << "=========ERROR=========" << endl;
		*fout << "500" << endl;
		*fout << "=======================" << endl << endl;
		return false;
	}
	*fout << "=======PRINT_ST========" << endl;
	for (auto iter = Aorder.begin(); iter != Aorder.end(); iter++) {
		*fout << iter->second->getName() << "/" << changeZero(iter->second->getCode()) << "/" << iter->second->getAuthor() << "/" << iter->second->getYear() << "/" << iter->second->getLoanCount() << endl;
	}
	*fout << "=====================" << endl << endl;
	for (auto iter = Aorder.rbegin(); iter != Aorder.rend(); iter++) {
		Aorder.erase(iter->first);
		if (Aorder.size() == 0)
			break;
	}
	return true;
}

void SelectionTree::newSelectionTree() {

	SelectionTreeNode* newNode = new SelectionTreeNode;
	upsize();
	this->root = newNode;
	for (int i = 2; i < 16; i++) {
		initializeTree();
	}
}

void SelectionTree::initializeTree() {

	SelectionTreeNode* newNode = new SelectionTreeNode;
	upsize();
	SelectionTreeNode* curNode = searchParentNode(getsize());
	if (curNode->getLeftChild() == NULL) { //priority is left > right
		if (size == 8) newNode->setHeap(this->heap_000);
		else if (size == 10) newNode->setHeap(this->heap_200);
		else if (size == 12) newNode->setHeap(this->heap_400);
		else if (size == 14) newNode->setHeap(this->heap_600);
		else newNode->setHeap(NULL);
		curNode->setLeftChild(newNode);
		newNode->setParent(curNode);
	}
	else {
		if (size == 9) newNode->setHeap(this->heap_100);
		else if (size == 11) newNode->setHeap(this->heap_300);
		else if (size == 13) newNode->setHeap(this->heap_500);
		else if (size == 15) newNode->setHeap(this->heap_700);
		else newNode->setHeap(NULL);
		curNode->setRightChild(newNode);
		newNode->setParent(curNode);
	}
}


SelectionTreeNode* SelectionTree::searchParentNode(int idx) {
	SelectionTreeNode* curNode = this->root;
	if (idx == 1) {
		return curNode;
	}
	int stack[300];
	for (int i = 0; i < 300; i++) {
		stack[i] = -1;
	}
	int top = -1; //stack index

	while (idx / 2 != 1) {
		stack[++top] = idx % 2;
		idx /= 2;
	}
	stack[++top] = idx % 2;
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
	return curNode;
}
string SelectionTree::changeZero(int Code) {
	string code;
	if (Code == 0)
		code = "000";
	else
		code = to_string(Code);
	return code;
}

