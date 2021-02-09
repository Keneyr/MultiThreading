#pragma once
//////////////////////////////////////////////////////////////////////////
//ÈÎÎñÀà
//////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>

class Task {

public:
	Task() {}
	~Task() {}
	int getId() {
		return id;
	}
	void setId(int id) {
		this->id = id;
	}

	std::string getName() {
		return name;
	}

	void setName(std::string name) {
		this->name = name;
	}

	int getNum() {
		return num;
	}

	void setNum(int num) {
		this->num = num;
	}
private:
	int id;
	std::string name;
	int num;
};