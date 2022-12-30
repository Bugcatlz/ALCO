#ifndef LABEL_H
#define LABEL_H
#include <string>
using namespace std;

class Label
{
public:
	Label(string n, int a)
		:name(n),
		address(a)
	{}

	int getAddress()
	{
		return address;
	}

	string getName()
	{
		return name;
	}
private:
	string name;
	int address;
};

#endif
