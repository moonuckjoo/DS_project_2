#include "Manager.h"

using namespace std;

void Manager::run(const char* command)
{
	fin.open(command);
	if (!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}
	string cmd;
	string str;
	while (!fin.eof())
	{
		cmd.clear();
		str.clear();
		fin >> cmd;
		getline(fin, str);
		if (cmd == "LOAD") LOAD();
		else if (cmd == "ADD") ADD(str);
		else if (cmd == "SEARCH_BP") {
			int cnt = 0;
			string search_data;
			for (int i = 0; str[i] != NULL; i++) {
				if (str[i] != '\t')
					continue;
				else
					cnt++;
			}
			if(cnt >2){
				printErrorCode(300);
				
			}
			else{
				if (cnt == 1) { //case of Name
					size_t len = str.find('\t', 0);
					search_data = str.substr(len+1, str.length() - len-2);
					SEARCH_BP_BOOK(search_data);
				}
				else { //case of range
					size_t len = str.find('\t', 0);
					size_t len2 = str.find('\t', len + 1);
					search_data = str.substr(len + 1, len2 - len - 1);
					string search_data2 = str.substr(len2 + 1, str.length() - len2);
					SEARCH_BP_RANGE(search_data, search_data2);
				}

			}
		}
		else if (cmd == "PRINT_BP") PRINT_BP();
		else if (cmd == "PRINT_ST") {
			string print_data;
			size_t len = str.find('\t', 0);
			print_data = str.substr(len + 1, str.length() - len);
			int code = stoi(print_data);
			PRINT_ST(code);
		}
		else if (cmd == "DELETE") {
			DELETE();
		}
		else if (cmd == "EXIT") {
			delete bptree;
			delete stree;
			flog << "=========EXIT=======" << endl;
			flog << "Success" << endl;
			flog << "====================" << endl << endl;
			flog.close();
			return;

		}
		else if (cmd ==""){
			continue;
		}
		else {
			printErrorCode(700);
		}

	}
	fin.close();
	return;
}

bool Manager::LOAD() {
	if (this->check_load > 0) {  //If it's already loaded, print errorCode
		printErrorCode(100);
		return false;
	}
	ifstream data_fin;
	data_fin.open("loan_book.txt");
	if (!data_fin) { //not open loan_book
		printErrorCode(100);
		return false;
	}
	string data_arr;
	while (!data_fin.eof()) {
		data_arr.clear(); //initialize
		getline(data_fin, data_arr);
		if (data_arr == "")
			break;
		LoanBookData* newData = parsing_data(data_arr);;
		//Replicating data values
		if (!this->bptree->Insert(newData)) {
			stree->Insert(newData);
		}
	}
	flog << "=========LOAD=======" << endl;
	flog << "Success" << endl;
	flog << "====================" << endl << endl;
	data_fin.close();
	this->check_load += 1;
	return true;
}


LoanBookData* Manager::parsing_data(string data_arr) {
	//str info
	string name;
	string str_code;
	string author;
	string str_year;
	string str_count;
	//Node info
	int count = 0;
	int code;
	int year;
	int loan_count;
	for (int i = 0; i < data_arr.length(); i++) {
		if (data_arr[i] == '\t') {
			count += 1;
		}
		else if (count == 0) { //no tap
			name += data_arr[i];
		}
		else if (count == 1) { // one tap
			str_code += data_arr[i];
		}
		else if (count == 2) { // two tap
			author += data_arr[i];
		}
		else if (count == 3) { //three tap
			str_year += data_arr[i];
		}
		else if (count == 4) {
			str_count += data_arr[i];
		}
	}
	
	//string to int
	code = stoi(str_code);
	year = stoi(str_year);
	loan_count = stoi(str_count);
	
	//newNode;
	LoanBookData* newNode = new LoanBookData;
	newNode->setBookData(name, code, author, year);

	for (int i = 0; i < loan_count; i++) {
		newNode->updateCount();
	}
	return newNode;
}


bool Manager::ADD(string str)
{
	int error = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] != '\t')
			continue;
		else {
			error++;
		}
	}
	//ErrorCase
	if (error != 4) {
		printErrorCode(200);
		return false;
	}
	size_t len = str.find('\t');
	size_t len2 = str.find('\t', len + 1);
	size_t len3 = str.find('\t', len2 + 1);
	size_t len4 = str.find('\t', len3 + 1);
	//initialize
	string add_name = "";
	int add_code = -1;
	string add_author = "";
	int add_year = -1;
	//set data;
	add_name = str.substr(len + 1, len2 - len - 1);
	add_code = stoi(str.substr(len2 + 1, len3 - len2 - 1));
	add_author = str.substr(len3 + 1, len4 - len3 - 1);
	add_year = stoi(str.substr(len4 + 1, str.length() - len4));

	LoanBookData* newData = new LoanBookData;
	newData->setBookData(add_name, add_code, add_author, add_year);
	if (this->bptree->Insert(newData)==false) {
		if (newData->getCode() < 201)
			newData->setLoanCount(3);
		else if (newData->getCode() < 401)
			newData->setLoanCount(4);
		else  //500~700
			newData->setLoanCount(2);
		this->stree->Insert(newData);
	}
	flog << "=========ADD=========" << endl;
	flog << add_name << "/" << bptree->changeZero(add_code) << "/" << add_author << "/" << add_year << endl;
	flog << "=====================" << endl << endl;


	return true;
} 

bool Manager::SEARCH_BP_BOOK(string book)
{
	if (bptree->getRoot()==nullptr) {
		printErrorCode(300);
		return false;
	}

	if (bptree->searchBook(book)==false) {
		printErrorCode(300);
		return false;
	}
	return true;
}

bool Manager::SEARCH_BP_RANGE(string s, string e)
{
	if (bptree->getRoot()==nullptr) {
		printErrorCode(300);
		return false;
	}
	if (bptree->searchRange(s, e)==false) {
		printErrorCode(300);
		return false;
	}
	return true;
}

bool Manager::PRINT_BP()
{
	BpTreeNode* curNode = bptree->getRoot();
	if (curNode == NULL) { //no bptree
		printErrorCode(400);
		return false;
	}
	else {
		while (curNode->getMostLeftChild() != NULL) {
			curNode = curNode->getMostLeftChild();
		}
		auto iter = curNode->getDataMap()->begin();
		flog << "=======PRINT_BP=======" << endl;
		while (curNode != NULL) {
			if (curNode->getDataMap()->size() == 0) {
				curNode = curNode->getNext();
				iter = curNode->getDataMap()->begin();
				continue;
			}
			flog << iter->second->getName() << "/" << bptree->changeZero(iter->second->getCode()) << "/" << iter->second->getAuthor() << "/" << iter->second->getYear() << "/" << iter->second->getLoanCount() << endl;
			iter++;
			if (iter == curNode->getDataMap()->end()) {
				curNode = curNode->getNext();
				if (curNode == NULL)
					break;
				iter = curNode->getDataMap()->begin();
			}
		}
		flog << "=====================" << endl << endl;
	}
	return true;
}

bool Manager::PRINT_ST(int code)
{
	if (stree->getRoot()->getBookData() == NULL)
		printErrorCode(500);
	else {
		stree->printBookData(code);
	}
	return true;
}

bool Manager::DELETE()
{
	if (!stree->Delete()) {
		printErrorCode(600);
		return false;
	}
	else {
		flog << "========DELETE========" << endl;
		flog << "Success" << endl;
		flog << "=====================" << endl << endl;
	}
	return true;
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	flog << "=========ERROR=========" << endl;
	flog << n << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "=======================" << endl;
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}