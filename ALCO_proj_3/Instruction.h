#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <vector>
#include <map>
#include <string>
#include "Register.h"
using namespace std;

extern Register* registers[6];

class Instruction
{
public:
	Instruction()
	{}

	Instruction(string myOperator, int reg1, int reg2, int reg3)
		:myOperator(myOperator)
	{
		myRegIndex.push_back(reg1);
		myRegIndex.push_back(reg2);
		myRegIndex.push_back(reg3);
	}

	Instruction(string myOperator, int reg1, int reg2, string imm)
		:myOperator(myOperator),
		myImmediate(stoi(imm))
	{
		myRegIndex.push_back(reg1);
		myRegIndex.push_back(reg2);
	}

	void exection()
	{
		if (myOperator == "ADD")
		{
			*myReg[0] = *myReg[1] + *myReg[2];
		}
		else if (myOperator == "ADDI")
		{
			*myReg[0] = *myReg[1] + myImmediate;
		}
		else if (myOperator == "DIV")
		{
			*myReg[0] = *myReg[1] / *myReg[2];
		}
		else if (myOperator == "MUL")
		{
			*myReg[0] = *myReg[1] * *myReg[2];
		}
		Register* r = getOutputRegister();
		r->getName().clear();
		clear();
	}

	void setReg()
	{
		for (int i = 0; i < myRegIndex.size();i++)
			myReg.push_back( registers[myRegIndex[i]]);
	}

	string getOperator()
	{
		return myOperator;
	}

	string getSymbol()
	{
		if (myOperator == "ADDI" || myOperator == "ADD")
			return "+";
		if (myOperator == "DIV")
			return "/";
		if (myOperator == "MUL")
			return "*";
		if (myOperator == "SUB")
			return "-";
		else
			return "";
	}

	string getType()
	{
		if (myOperator == "MUL" || myOperator == "DIV")
			return "MUL";
		else
			return "ADD";
	}

	void clear()
	{
		myOperator.clear();
		myReg.clear();
		myImmediate = 0;
		cycle = -1;
	}

	bool ready()
	{
		for (int i = 1; i < myReg.size(); i++)
		{
			if (myReg[i]->getName().size() != 0)
				return false;
		}
		return true;
	}

	bool empty()
	{
		return myOperator.empty();
	}

	void setCycle(int c)
	{
		cycle = c;
	}

	int getCycle()
	{
		return cycle;
	}

	Register* getOutputRegister()
	{
		return myReg[0];
	}

	void setOutputRegister(Register* r)
	{
		myReg[0] = r;
	}

	int getOutputRegisterIndex()
	{
		return myRegIndex[0];
	}

	string getReg(int n)
	{
		if(empty())
			return "";
		if (n < myReg.size() && myReg[n]->getName().size()>0)
			return myReg[n]->getName();
		else if (n < myReg.size())
			return to_string(myReg[n]->getValue());
		else
			return to_string(myImmediate);
	}


private:	
	vector <int> myRegIndex;
	vector <Register*>myReg;
	string myOperator = "";
	int myImmediate = 0;
	int cycle = -1;
	string OuputRemaing;
};

#endif
