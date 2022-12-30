#ifndef REGISTER_H
#define REGISTER_H
#include <vector>
#include <map>
#include <string>
using namespace std;

class Register
{
public:
	int& operator=(int value)
	{
		myValue = value;
		return myValue;
	}

	int& operator+(int value)
	{
		int v = myValue + value;
		return v;
	}

	int& operator+(Register r)
	{
		int v = myValue + r.myValue;
		return v;
	}

	bool operator==(Register r)
	{
		return (myValue == r.myValue);
	}

	bool operator!=(Register r)
	{
		return (myValue != r.myValue);
	}

	int getValue()
	{
		return myValue;
	}

	Register()
		:myValue()
	{}
private:
	int myValue;
};

#endif