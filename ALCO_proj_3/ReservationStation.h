#ifndef RESERVATIONSATAION_H
#define RESERVATIONSATAION_H

#include "Instruction.h"
#include "Register.h"
#include <vector>
#include <map>
extern Register* registers[6];
extern map <string, int> needCycle;
using namespace std;

class ReservationStation
{
public:
	ReservationStation()
		:numRS()
	{
		buffer = nullptr;
	}

	void set(int num)
	{
		numRS = num;
		myRS.resize(num);
	}

	bool full()
	{
		for (int i = 0; i < myRS.size(); i++)
		{
			if (myRS[i].empty())
				return false;
		}
		return true;
	}

	bool insertInst(Instruction inst, int cycle,string type)
	{
		for (int i = 0; i < myRS.size(); i++)
		{
			if (myRS[i].empty())
			{
				inst.setCycle(cycle);
				inst.setReg();
				myRS[i] = inst;
				string name = getRSName(inst, i);
				Register* temp = new Register;
				temp->setName(name);
				*temp = myRS[i].getOutputRegister()->getValue();
				int j = myRS[i].getOutputRegisterIndex();
				registers[j] = temp;
				myRS[i].setOutputRegister(temp);
				return true;
			}					
		}
		return false;
	}

	string getRSName(Instruction inst,int i)
	{
		string type = inst.getType();
		string name = "RS";
		if (type == "MUL")
			name.push_back('4' + i);
		else
			name.push_back('1' + i);
		return name;
	}

	bool execute()
	{
		bool flag = false;
		if (buffer!=nullptr)
		{
			count--;
			if (count == 0)
			{
				buffer->exection();
				buffer = nullptr;
				flag = true;
			}
		}
		return flag;
	}

	void update(int cycle)
	{
		if (buffer==nullptr)
		{
			int max = getMaxCycle();
			int min = getMinCycle();
			for (int i = min; i <= max; i++)
			{
				for (int j = 0; j < myRS.size(); j++)
				{
					if (i == myRS[j].getCycle() && myRS[j].ready() && i!=cycle)
					{
						buffer = &myRS[j];
						count = needCycle[myRS[j].getOperator()];
						break;
					}
				}
			}
		}
	}

	int getMinCycle()
	{
		int min = myRS[0].getCycle();
		for (int i = 0; i < myRS.size(); i++)
		{
			if (myRS[i].getCycle() < min && myRS[i].getCycle() != -1)
				min = myRS[i].getCycle();
		}
		return min == -1 ? 0 : min;
	}

	int getMaxCycle()
	{
		int max = myRS[0].getCycle();
		for (int i = 0; i < myRS.size(); i++)
		{
			if (myRS[i].getCycle() > max)
				max = myRS[i].getCycle();
		}
		return max == -1 ? 0 : max;
	}

	int getSize()
	{
		return numRS;
	}

	Instruction getInstruction(int n)
	{
		return myRS[n];
	}

	void outputBuffer()
	{
		cout << "BUFFER: ";
		if (buffer == nullptr)
			cout << "empty" << endl;
		else
			cout << "(" << buffer->getOutputRegister()->getName() << ")"
			<< " " << buffer->getReg(1) << " " << buffer->getSymbol()
			<< " " << buffer->getReg(2) << endl;
	}

private:
	int numRS;
	int count = 0;
	vector <Instruction> myRS;
	Instruction* buffer;
};

#endif
