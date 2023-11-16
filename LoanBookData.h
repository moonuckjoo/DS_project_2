#pragma once

#include<iostream>
#include<string>
#include <map>
using namespace std;

class LoanBookData
{
private:
    string name;
    int code;
    string author;
    int year;
    int loan_count;

public:
    LoanBookData() { 
        this->loan_count = 0;}
    ~LoanBookData() {}

    void setBookData(string name, int code, string author, int year) {
        this->name = name;
        this->code = code;
        this->author = author;
        this->year = year;
        this->loan_count = loan_count;
    }

    void setName(string name) { this->name = name; }
    void setCode(int code) { this->code = code; }
    void setAuthor(string author) { this->author = author; }
    void setYear(int year) { this->year = year; }
    void updateCount() { this->loan_count += 1; }
    void setLoanCount(int loan_count) { this->loan_count = loan_count; }

    string getName() { return name; }
    int getCode() { return code; }
    string getAuthor() { return author; }
    int getYear() { return year; }
    int getLoanCount() { return loan_count; }


    void exyear() { year += 1; }
};