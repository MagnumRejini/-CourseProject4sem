#pragma once

#include <iostream>

using namespace std;

class MyException : public exception
{
protected:
	int code;
public:
	MyException(int x = 0) { code = x; }
	virtual void ErrorText() {};
};
