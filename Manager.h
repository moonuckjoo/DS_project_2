#pragma once
#include "SelectionTree.h"
#include "BpTree.h"
#include <string>
class Manager
{
private:
	char* cmd;
	BpTree* bptree;
	SelectionTree* stree;
	int check_load = 0;
public:
	Manager(int bpOrder)	//constructor
	{
		this->cmd = nullptr;
		this->bptree = new BpTree(&flog, 3); //initialize bpTree
		this->stree = new SelectionTree(&flog);
		flog.open("log.txt",ios::app);
	}


	~Manager()//destructor
	{
	}

	ifstream fin;
	ofstream flog;

	LoanBookData* parsing_data(string data_arr); //data parsing function
	void run(const char* command);
	bool LOAD();
	bool ADD(string str);

	bool SEARCH_BP_BOOK(string book);
	bool SEARCH_BP_RANGE(string start, string end);

	bool PRINT_BP();
	bool PRINT_ST(int code);

	bool DELETE();

	void printErrorCode(int n);
	void printSuccessCode();

};

