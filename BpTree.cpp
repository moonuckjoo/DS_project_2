#include "BpTree.h"
#include <queue>
bool BpTree::Insert(LoanBookData* newData) {
	if (this->root == NULL) { //no root
		BpTreeNode* newNode = new BpTreeDataNode;
		newNode->insertDataMap(newData->getName(), newData);
		this->root = newNode;
		return true;
	}

	else { // exit rootNode
		BpTreeNode* curNode = root;

		if (curNode->getMostLeftChild() == NULL) { //curNode is not indexNode
			auto iter = curNode->getDataMap()->begin();
			while (iter != curNode->getDataMap()->end()) {
				if (iter->first == newData->getName()) {
					iter->second->updateCount();
					if (iter->second->getCode() < 201) {
						if (iter->second->getLoanCount() == 3) {
							cout <<newData->getName() << "\n";
							delete_data(iter->first);
							return false;
						}
					}
					else if (iter->second->getCode() < 501) {
						if (iter->second->getLoanCount() == 4) {
							cout <<newData->getName() << "\n";
							delete_data(iter->first);
							return false;
						}
					}
					else { // Code is 600~700
						if (iter->second->getLoanCount() == 2) {
							delete_data(iter->first);
							return false;
						}
					}
					return true;
				}
				else
					iter++;
			}
			curNode->insertDataMap(newData->getName(), newData);
			if (excessDataNode(curNode) == true)
				splitDataNode(curNode);
			return true;
		}
		else { // bptree has indexNode
			curNode = searchDataNode(newData->getName());
			auto iter = curNode->getDataMap()->begin();
			while (iter != curNode->getDataMap()->end()) {
				if (iter->first == newData->getName()) {
					iter->second->updateCount();
					if (iter->second->getCode() < 201) {
						if (iter->second->getLoanCount() == 3) {
							delete_data(iter->first);
							return false;
						}
					}
					else if (iter->second->getCode() < 401) {
						if (iter->second->getLoanCount() == 4) {
							delete_data(iter->first);
							return false;
						}
					}
					else { // Code is 500~700
						if (iter->second->getLoanCount() == 2) {
							delete_data(iter->first);
							return false;
						}
					}
					return true;
				}
				else
					iter++;
			}
			curNode->insertDataMap(newData->getName(), newData);
			if (excessDataNode(curNode) == true)
				splitDataNode(curNode);
			return true;
		}
	}
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > this->order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > this->order - 1)return true;//order is equal to the number of elements 
	else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {

	BpTreeNode* newDataNode = new BpTreeDataNode;
	auto iter = pDataNode->getDataMap()->begin();
	iter++; //second
	newDataNode->insertDataMap(iter->first, iter->second);
	//new indexNode;
	iter++;
	newDataNode->insertDataMap(iter->first, iter->second);

	pDataNode->deleteMap(iter->first);
	iter = pDataNode->getDataMap()->begin();
	iter++;
	pDataNode->deleteMap(iter->first);
	newDataNode->setNext(pDataNode->getNext());
	if (pDataNode->getNext()) pDataNode->getNext()->setPrev(newDataNode);
	pDataNode->setNext(newDataNode);
	newDataNode->setPrev(pDataNode);

	if (pDataNode->getParent() == NULL) { //if no parent index Node, new indexNode
		auto iter = newDataNode->getDataMap()->begin();
		BpTreeNode* newIndexNode = new BpTreeIndexNode;
		newIndexNode->insertIndexMap(iter->first, newDataNode);
		newIndexNode->setMostLeftChild(pDataNode);
		pDataNode->setParent(newIndexNode);
		newDataNode->setParent(newIndexNode);
		root = newIndexNode;
	}

	else { //if has a parentNode, 
		newDataNode->setParent(pDataNode->getParent());
		auto iter = newDataNode->getDataMap()->begin();
		pDataNode->getParent()->insertIndexMap(iter->first, newDataNode);
		if (excessIndexNode(pDataNode->getParent()) == true)
			splitIndexNode(newDataNode->getParent());
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	if (excessIndexNode(pIndexNode) == false) return; //condition of exit
	if (pIndexNode->getParent() == NULL) { //currentNode is root
		BpTreeNode* newIndexNode = new BpTreeIndexNode;
		BpTreeNode* newRootNode = new BpTreeIndexNode;
		auto iter = pIndexNode->getIndexMap()->begin();
		iter++;
		newIndexNode->setMostLeftChild(iter->second);
		newRootNode->insertIndexMap(iter->first, newIndexNode);
		iter++;
		newIndexNode->insertIndexMap(iter->first, iter->second);
		iter->second->setParent(newIndexNode);
		pIndexNode->deleteMap(iter->first);
		iter = pIndexNode->getIndexMap()->begin();
		iter++;
		iter->second->setParent(newIndexNode);
		pIndexNode->deleteMap(iter->first);
		root = newRootNode;
		newRootNode->setMostLeftChild(pIndexNode);
		pIndexNode->setParent(newRootNode);
		newIndexNode->setParent(newRootNode);
	}
	else { //currentNode is not root
		BpTreeNode* newIndexNode = new BpTreeIndexNode;
		auto iter = pIndexNode->getIndexMap()->begin();
		iter++;
		iter++;
		newIndexNode->insertIndexMap(iter->first, iter->second);
		iter->second->setParent(newIndexNode);
		pIndexNode->deleteMap(iter->first);
		iter = pIndexNode->getIndexMap()->begin();
		iter++;
		pIndexNode->getParent()->insertIndexMap(iter->first, newIndexNode);
		newIndexNode->setMostLeftChild(iter->second);
		iter->second->setParent(newIndexNode);
		newIndexNode->setParent(pIndexNode->getParent());
	}
	pIndexNode = pIndexNode->getParent();
	splitIndexNode(pIndexNode);
}

BpTreeNode* BpTree::searchDataNode(string name) {
	BpTreeNode* pCur = root;
	while (pCur->getMostLeftChild() != NULL) { //pCur is not indexNode
		auto iter = pCur->getIndexMap()->begin();
		if (pCur->getIndexMap()->size() == 1) { //size 1
			if (iter->first > name) {
				pCur = pCur->getMostLeftChild();
			}
			else {
				pCur = iter->second;
			}
		}
		else { //size 2
			if (iter->first > name) {
				pCur = pCur->getMostLeftChild();
			}
			else {
				iter++;
				if (iter->first > name) {
					iter = pCur->getIndexMap()->begin();
					pCur = iter->second;
				}
				else pCur = iter->second;
			}
		}
	}
	return pCur;
}


bool BpTree::searchBook(string name) {
	BpTreeNode* curNode = searchDataNode(name);
	auto iter = curNode->getDataMap()->begin();
	while(1) {
		while (iter == curNode->getDataMap()->end()) {
			if (curNode->getNext() == NULL) {
				return false; //no data
			}
			else{
				curNode = curNode->getNext();
				iter = curNode->getDataMap()->begin();
			}
		}
		if (name == iter->second->getName()) {
			*fout << "====== SEARCH_BP ======" << endl;
			*fout << iter->second->getName() << "/" << changeZero(iter->second->getCode()) << "/" << iter->second->getAuthor() << "/" << iter->second->getYear() << "/" << iter->second->getLoanCount() << endl;
			*fout << "=====================" << endl << endl;
			break;
		}
		iter++;
	}
	return true;
}


bool BpTree::searchRange(string start, string end) {
	BpTreeNode* startNode = searchDataNode(start);
	auto iter = startNode->getDataMap()->begin();

	while(1) {
		if (iter == startNode->getDataMap()->end()) {
			if (startNode->getNext() != NULL) { //startNode is not last DataNode
				startNode = startNode->getNext();
				iter = startNode->getDataMap()->begin();
			}
			else{
				return false;
			}

		}
		else {
			if (iter->first[0] >= start[0]) {
				break;
			}
			else {
				iter++;
			}
		}
	}

	if ((iter->first[0] > end[0]))
		return false;
	*fout << "====== SEARCH_BP ======" << endl;
	int cnt=0;
	while (iter->first[0]<=end[0]) { //start != end
		*fout << iter->second->getName() << "/" << changeZero(iter->second->getCode()) << "/" << iter->second->getAuthor() << "/" << iter->second->getYear() << "/" << iter->second->getLoanCount() << endl;
		iter++;
		while (iter == startNode->getDataMap()->end()) { //to the next node
			if (startNode->getNext() == NULL) {
				cnt++;
				break;
			}
			startNode = startNode->getNext();
			iter = startNode->getDataMap()->begin();
		}
		if (cnt > 0)
			break;
	}
	*fout << "=====================" << endl << endl;
	return true;

}

string BpTree::changeZero(int Code) {
	string code;
	if (Code == 0)
		code = "000";
	else
		code = to_string(Code);
	return code;
}


void BpTree::delete_data(string delName) {
	BpTreeNode* delNode = searchDataNode(delName);
	delNode->deleteMap(delName);
}
