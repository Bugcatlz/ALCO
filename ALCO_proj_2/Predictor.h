#ifndef PREDICTOR_H
#define PREDICTOR_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

class Predictor
{
	friend ostream& operator<<(ostream& output, Predictor pred);
public:
	Predictor():
		index(),
		misprediction()
	{
		historyBit.resize(3);
		for (int i = 0; i < 3; i++)
			historyBit[i] = 0;
		state.resize(8);
		for (int i = 0; i < 8; i++)
			state[i] = "SN";
	}

	bool prediction()
	{
		index = 0;
		int count = 0;
		for (int i = 2; i >= 0; i--)
			index += pow(2, count++) * historyBit[i];
		if (state[index][1] == 'N')
			return false;
		else
			return true;
	}

	void update(bool myPred, bool outcome)
	{
		historyBit[0] = historyBit[1];
		historyBit[1] = historyBit[2];
		historyBit[2] = outcome;
		if (outcome)//taken
		{
			if (state[index] == "SN")
				state[index] = "WN";
			else if (state[index] == "WN")
				state[index] = "WT";
			else if(state[index] == "WT")
				state[index] = "ST";
		}
		else//noTaken
		{
			if (state[index] == "WN")
				state[index] = "SN";
			else if (state[index] == "WT")
				state[index] = "WN";
			else if(state[index] == "ST")
				state[index] = "WT";
		}
		if(myPred!=outcome)
			misprediction++;		
	}

	int getMispred()
	{
		return misprediction;
	}
private:
	int index;//當前history bit所使用的2-bit counter
	vector <int> historyBit;//用來存放3-bit的history
	vector <string> state;//用來存放preditor的8個2-bit counter
	int misprediction;
};
#endif
