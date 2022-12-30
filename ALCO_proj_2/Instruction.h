#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <vector>
#include <map>
#include <string>
#include "Register.h"
using namespace std;

class Instruction
{
	friend ostream& operator<<(ostream& output, Instruction inst);
public:
	Instruction(int address, string myOperator, Register* reg1, Register* reg2, Register* reg3)
		:myAddress(address),
		myOperator(myOperator)
	{
		myReg.push_back(reg1);
		myReg.push_back(reg2);
		myReg.push_back(reg3);
	}

	Instruction(int address, string myOperator, Register* reg1, Register* reg2, int imm)
		:myAddress(address),
		myOperator(myOperator),
		myImmediate(imm)
	{
		myReg.push_back(reg1);
		myReg.push_back(reg2);
	}

	Instruction(int address, string myOperator, Register* reg1, int imm)
		:myAddress(address),
		myOperator(myOperator),
		myImmediate(imm)
	{
		myReg.push_back(reg1);
	}

	Instruction(int address, string myOperator, Register* reg1, Register* reg2, string label)
		:myAddress(address),
		myOperator(myOperator),
		myLabel(label)
	{
		myReg.push_back(reg1);
		myReg.push_back(reg2);
	}

	int getAddress()
	{
		return myAddress;
	}

	bool exection()
	{
		if (myOperator == "li")
		{
			*myReg[0] = myImmediate;
		}
		else if (myOperator == "add")
		{
			*myReg[0] = *myReg[1] + *myReg[2];
		}
		else if (myOperator == "addi")
		{
			*myReg[0] = *myReg[1] + myImmediate;
		}
		else if (myOperator == "beq")
		{
			if (*myReg[0] == *myReg[1])
				return true;
		}
		else if (myOperator == "bne")
		{
			if (*myReg[0] != *myReg[1])
				return true;
		}
		else if (myOperator == "andi")
		{
			*myReg[0] = myReg[1]->getValue() & myImmediate;
		}
		return false;
	}

	string getLabel()
	{
		return myLabel;
	}
private:
	vector < Register* > myReg;//用來存放所使用的register
	int myAddress;
	string myOperator;
	int myImmediate;
	string myLabel;
};

#endif
