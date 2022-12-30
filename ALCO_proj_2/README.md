ALCO_proj_2
=== 
## Video
<https://youtu.be/VFHMsZUzLuo/>

## Input
```
0x110		li R2,0			; v=0 addi R2,R0,0
0x114		li R3,16		; Loop bound for LoopI addi R3,R0,16
0x118		li R4,0			; i=0 addi R4,R0,0
	LoopI
0x11C		beq R4,R3,EndLoopI	; Exit LoopI if i==16
0x120		li R5,0			; j=0 addi R5,R0,0
	LoopJ
0x124		beq R5,R3,EndLoopJ     ; Exit LoopJ if j==16
0x128		add R6,R5,R4		; j+i
0x12C		andi R6,R6,3		; (j+i)%4
0x130		bne R6,R0,Endif	; Skip if (j+i)%4!=0
0x134		add R2,R2,R5		; v+=j
	Endif
0x138		addi R5,R5,1		; j++
0x13C		beq R0,R0,LoopJ	; Go back to LoopJ
	EndLoopJ
0x140		addi R4,R4,1		; i++
0x144		beq R0,R0,LoopI	; Go back to LoopI
	EndLoopI
```
並將其放到input.txt中

## Global Variable
### A. registers
```c++
vector <Register*> registers(32);
```
為一個vector，用來儲存指向Register的pointer，

其代表的意義為代表32個的register

## Class
### A. Instruction
--- 
### Private:
### 1. myReg
```c++
vector < Register* > myReg
```
用來存放這個Instruction所使用到的Register

### 2.  myAddress
```c++
int myAddress
```
用來存放這個Instruction的位置

### 3. myOperator
```c++
string myOperator
```
用來存放這個Instruction的operator

### 4. myImmediate
```c++
int myImmediate
```
用來存放這個Instruction中immediate的值

### 5. myLabel
```c++
string myLabel
```
用來存放這個Instruction中的Label的名子

### Public
### 1. Constructor
```c++
Instruction(int address, string myOperator, Register* reg1, Register* reg2, Register* reg3)
	:myAddress(address),
	myOperator(myOperator)
{
	myReg.push_back(reg1);
	myReg.push_back(reg2);
	myReg.push_back(reg3);
}
```
為用來建構一個Instruction包含三個Register的Constructor

Ex: `add R6,R5,R4`

```C++
Instruction(int address, string myOperator, Register* reg1, Register* reg2, int imm)
	:myAddress(address),
	myOperator(myOperator),
	myImmediate(imm)
{
	myReg.push_back(reg1);
	myReg.push_back(reg2);
}
```
為用來建構一個Instruction包含二個Register和一個Immediate的Constructor

Ex: `addi R5,R5,1`

```C++
Instruction(int address, string myOperator, Register* reg1, int imm)
	:myAddress(address),
	myOperator(myOperator),
	myImmediate(imm)
{
	myReg.push_back(reg1);
}
```

為用來建構一個Instruction包含一個Register和一個Immediate的Constructor

Ex: `li R3,16`

```C++
Instruction(int address, string myOperator, Register* reg1, Register* reg2, string label)
	:myAddress(address),
	myOperator(myOperator),
	myLabel(label)
{
	myReg.push_back(reg1);
	myReg.push_back(reg2);
}
```
為用來建構一個Instruction包含一個Register和一個Label的Constructor

Ex: `beq R5,R3,EndLoopJ`

#### 2. getAddress
```C++
int getAddress()
{
	return myAddress;
}
```
回傳這個Instruction的address

### 3. exection
```c++
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
```
判斷當myOperator為特定的Operator時，

執行相對應的運算，若是會branch則return true，

若是不會branch則return false

### 4. getLabel
```c++
string getLabel()
	{
		return myLabel;
	}
```
回傳myLabel


### Friend:
#### 1. ostream& operator<<(ostream& output, Instruction inst)
```C++
ostream& operator<<(ostream& output, Instruction inst)
{
	output << inst.myOperator << ' ' << 'R' << findRegIndex(inst.myReg[0], registers) << ',';
	if (inst.myReg.size() == 3)//ex:add R6,R5,R4
		output << 'R' << findRegIndex(inst.myReg[1], registers)
		<< ','  << 'R' << findRegIndex(inst.myReg[2], registers);
	else if (inst.myLabel.size() != 0)//ex:beq R5,R3,EndLoopJ 
		output << 'R' << findRegIndex(inst.myReg[1], registers) << ',' << inst.myLabel;
	else if (inst.myReg.size() == 2)//ex:addi R5,R5,1
		output  << 'R' << findRegIndex(inst.myReg[1], registers) << ',' << inst.myImmediate;
	else if (inst.myReg.size() == 1)//ex:li R3,16
		output << inst.myImmediate;
	return output;
}
```
先判斷Instruction的類型，在輸出其對應的格式

### B. Register
--- 
### Private
### 1. myValue
```c
int myValue;
```
用來記錄Register內的值
### Public
### 1. Constructor
```c
Register()
	:myValue()
{}
```
初始化myValue

### 2. operator=
```
int& operator=(int value)
{
	myValue = value;
	return myValue;
}
```
將傳入的型態為int的值傳給myValue

並回傳myValue

### 3. operator+
```c++
int& operator+(int value)
{
	int v = myValue + value;
	return v;
}
```
將傳入型態為int的值加上myValue，並回傳

```c++
int& operator+(Register r)
{
	int v = myValue + r.myValue;
	return v;
}
```
將傳入的Register其myValue加上自身的myValue，並回傳

### 4. operator==
```c
bool operator==(Register r)
{
	return (myValue == r.myValue);
}
```
如是傳入的Register的myValue等於自身的myValue則回傳true，反之亦然

### 5. operator!=
```c
bool operator!=(Register r)
{
	return (myValue != r.myValue);
}
```
如是傳入的Register的myValue不等於自身的myValue則回傳true，反之亦然

### 6. getValue
```c
int getValue()
{
	return myValue;
}
```
回傳myValue

### C. Label
--- 
### Private
### 1. name
```c
string name
```
用來儲存Label的名稱

### 2. address
```c
int address
```
用來儲存Label的位置

### Public
### 1. Constructor
```C++
Label(string n, int a)
	:name(n),
	address(a)
{}
```
將傳入的值做初始化

### 2. getAddress
```C
int getAddress()
{
	return address;
}
```
回傳Label的address

### 3. getName
```C
string getName()
{
	return name;
}
```
用來回傳Label的名稱

### D. Predictor
--- 
### Private
### 1. index
```C
int index
```
為當前Predictor所使用的2-bit counter

Ex: 01 則 index為2

### 2. historyBit
```C
vector <int> historyBit
```
為用來存放的3-bit history的值

Ex: 0 1

### 3. state
```C
vector <string> state
```
用來存放preditor的8個2-bit history counter

Ex: `WN, SN, ST, SN, SN, SN, SN, SN`

### 4. misprediction
```C
int misprediction;
```
用來存放misprediction的次數

### Public
### 1. Constructor
```C
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
```
用來初始化index和misprediction，

且將historyBit的大小設定成3，

再將state的大小設定成8，

最後再將state內都先設定為SN

### 2. prediction
```C
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
```
先透過history計算出計算出index，

再根據對應的state回傳true or false

### 3. update
```C
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
```
先將historyBit向左一位且最右邊的一位為這次真正的結果，

再根據這次的結果來更新state，

且當prediction != outcome時misprediction+1

### 4. getMispred
```C
int getMispred()
{
	return misprediction;
}
```
回傳misprediction

### Friend
### 1. operator<<
```C
ostream& operator<<(ostream& output, Predictor pred)
{
	output << "(";
	for (int i = 0; i < 3; i++)
		output << pred.historyBit[i];
	for (int i = 0; i < 8; i++)
		output << ", " << pred.state[i];
	output << ")";
	return output;
}
```
先輸出historyBit，在輸出每個狀態，並用逗號隔開，且前後為括號

## Function
### A. readfile
```C
void readfile(vector <Instruction>& insts, vector <Label>& labs)
```
讀取txt檔，並將內部的instruction放入insts且將label放入到labs中
```C
ifstream inFile("input.txt", ios::in);
if (!inFile)
{
	cout << "File could not be opened!!" << endl;
	exit(1);
}
```
開啟txt檔，並檢查是否有開啟成功

```C
cin.clear();
string temp;
vector <string> input;//用來存讀檔後的每行的string
while (getline(inFile, temp))
	input.push_back(temp);
```
使用getline的方式來進行讀檔，並放入到input中

```C
int i = 0;
while (i < input.size())
{
	if (input[i][0] != '\t')//判斷是否不為label
		insts.push_back(converInst(input[i]));
	else
	{
		if (i != input.size() - 1)//判斷是否不為最後一個label
		{
			string n = converLabel(input[i]);//name
			Label temp(n, insts[insts.size() - 1].getAddress()+4);
			labs.push_back(temp);
		}
		else
		{
			string n = converLabel(input[i]);
			int a = -1;//address
			Label temp(n, a);//設定最後一個label的位置為-1
			labs.push_back(temp);
		}
	}
	i++;
}
inFile.close();
```
若是為Label則開頭為`'\t'` ，

若不為Label則呼叫convertInst並push_back到inst中，

則先判斷是否為最後一個Label，因為最後一個Label是沒有具體的位置的

故我們將最後一個Label的位置設為-1，且先呼叫converLabel先將Label傳入n中，

並最後再建一個Label根據n及address，建立一個Label存入labs中，

若不為最後一個Label則一樣先呼叫converLabel取得name，

而其位置是上一個instruction的位置+4，

最後再push_back到labs中

### B. converInst
```C
Instruction converInst(string input)
```
為將一行的string轉為Instruction
```c++
string addressString;//用來存放型態為string的address
string Operator;
vector <string> regs;//用來存存放當中全部的register
string reg; //用來存register的index ex:R8 中的 8
string imm; //用來存immediate型態為string
string label; 
int Address; 
bool flag = false; //是否為最後一個operand
int k = 0;  //input的index
```
將下面的input作為範例

Ex: input = `0x110\t\tli R2,0\t\t\t; v=0 addi R2,R0,0`
```C
//address
while (input[k] != '\t')
	addressString.push_back(input[k++]);
Address = stringToint(addressString);
//address和operator之間的兩個\t
k++;
k++;
```
先處理address，因為先將address放入addressString

在呼叫stringToint將addressString轉成十進位的int

在呼叫k++兩次因為address與operator間隔了兩個`\t`

```C
//operator
while (input[k] != ' ')
	Operator.push_back(input[k++]);
k++;
```
再處理Operator，因為Operator與Operand之間隔一個空格，

所以先到第一個空格間都是Operator，

最後再k++去除那個空格

```C
//第一個register
k++;	//去除R
while (input[k] != ',')
	reg.push_back(input[k++]);
regs.push_back(reg);
reg.clear();
k++;
```
先k++一次去除R8中的R，我們只取其index

因為第一個register與第二個operand之前格一個逗號，

所以在這其中都是register的index，

最後再將其放入regs中，

並將其clear，已達到重複使用

最後再K++去除那個逗號

```C
//第二個
if (input[k] == 'R')//為reg
{
	k++;	//去除R
	while (input[k] != ',')
		reg.push_back(input[k++]);
	regs.push_back(reg);
	reg.clear();
	k++;
}
else
{
	while (true)
	{
		imm.push_back(input[k++]);
		if (input[k] == '\t')//判斷是否為最後一個
		{
			flag = true;
			break;
		}
		if (input[k] == ',')
		{
			k++;
			break;
		}
	}
}
```
先判斷是否為register，若是則同上一步的動作，

先找到index，再push_back到regs中，

若不是則為immediate，

故依序push_back到imm中，

再判斷是否為`\t`若是則為最後一個，

因為以`\t`作為結尾，故將flag轉為true，

若是遇到逗號，則代表後續還有operand

```C
if (!flag)//不為最後一個
{
	if (input[k] == 'R')//判斷是否為register
	{
		k++; //去除R
		while (!(input[k] == '\t' || input[k] == ' '))
			reg.push_back(input[k++]);
		regs.push_back(reg);
	}
	else if ((input[k] <= '9' && input[k] >= '0') || input[k] == '-')//判斷是否為immeditate
	{
		while (!(input[k] == '\t' || input[k] == ' '))
			imm.push_back(input[k++]);
	}
	else//為label
	{
		while (!(input[k] == '\t' || input[k] == ' '))
			label.push_back(input[k++]);
	}
}
```
先判斷最後是否為register，若是則處理方法與前面相同但是結束的條件為`\t`或是' '，

若是為0~9之間或'-'，則為immediate處理方法與前面相同同但是結束的條件為`\t`或是' '，

若都不是則為Label，依樣依序push_back到label中結束的條件為`\t`或是' '

```C
if (regs.size() == 3)//ex:add R6,R5,R4
{
	Instruction temp(Address, Operator, registers[stoi(regs[0])], registers[stoi(regs[1])], registers[stoi(regs[2])]);
	return temp;
}
if (label.size() != 0)//ex:beq R5,R3,EndLoopJ 
{
	Instruction temp(Address, Operator, registers[stoi(regs[0])], registers[stoi(regs[1])], label);
	return temp;
}
if (regs.size() == 1)//ex:li R3,16
{
	Instruction temp(Address, Operator, registers[stoi(regs[0])], stoi(imm));
	return temp;
}
if (regs.size() == 2)//ex:addi R5,R5,1
{
	Instruction temp(Address, Operator, registers[stoi(regs[0])], registers[stoi(regs[1])], stoi(imm));
	return temp;
}
```
判斷register的數量，及是否存在labe來依序呼叫對應的Constructor，

最後再回傳此物件

### C. converInst
```C
string converLabel(string input)
{
	int k = 1; //去除第一個\t
	string temp;
	while (k != input.size())
		temp.push_back(input[k++]);
	return temp;
}
```
Ex: input = `\tLoopI`

故要先去除第一個`\t`，

再依序push_back到temp中直到最後一個index，

並回傳temp

### C. addressToIndex
```C
int addressToIndex(int a, vector <Instruction>& insts)
{
	for (int i = 0; i < insts.size(); i++)
	{
		if (a == insts[i].getAddress())
			return i;
	}
}
```
當address為a時，其在instructions中的index即為第幾個instruction

### D. labelToAddress
```C
int labelToAddress(string l, vector <Label>& labs)
{
	for (int i = 0; i < labs.size(); i++)
	{
		string temp = labs[i].getName();
		if (temp == l)
			return labs[i].getAddress();
	}
}
```
當Label為l時，找其再labs中的address

### E. stringToint 
```C
int stringToint(string s)
{
	int sum = 0;
	int count = 0;
	int i = s.size() - 1;
	while (s[i] != 'x')
	{
		if(s[i]<='9' && s[i] >='0')
			sum += pow(16, count++) * (s[i]-'0');
		else
		{
			int temp = s[i] - 'A' + 10;
			sum += pow(16, count++) * temp;
		}
		i--;
	}
	return sum;
}
```
將16進位的string轉成int

### F. findRegIndex
```C
int findRegIndex(Register* r, vector <Register*> registers)
{
	for (int i = 0; i < registers.size(); i++)
		if (registers[i] == r)
			return i;
}
```
當register的位置為r時，找到其在regiseters中的位置

### G. isPowerBy2
```C
bool isPowerByTwo(int num)
{
	return num > 0 && (num & num - 1) == 0;
}
```
因為當n為2的倍數時，在二進位中只有最高的項為一，

而其減一會變成只有最高項是0其餘是1，

故作and時會是0，故可判斷是否為2的次方

## Main Function
```C
for (int i = 0; i < 32; i++)//初始化32個pointer to register
	registers[i] = new Register;
vector <Instruction> insts;//用來裝所有的instruction
vector <Label> labs;//用來裝所有的label
readfile(insts, labs);
```
先初始化所有的register，

再建立用來儲存所有instruction合lab的vector，

並讀檔放入insts和labs中

```C
int num;//用來存放number of entry

cout << "Please input entry(entry > 0):" << endl;
cin >> num;

if (num <= 0 || !isPowerByTwo(num) ) //大於零且為二的次方
{
	cout << "Number of entries is an error!" << endl;
	system("pause");
	exit(1);
}
vector <Predictor> preds;//用來存放所有的Predictor
preds.resize(num);
```
輸入entry的數量放到num中，

並判斷num是否合法，需大於零且為二的次方，

因為當Index有n個bits時，則有2^n個entry，

故entry不可能不為2的次方，

最後再來建立相對應數量的predictor

```C
int next = insts[0].getAddress();
while (next != -1)//下一個不為最後一個label
{
	//predictor
	int entry = (next / 4) % num;//找到為第幾的entry
	bool predBranch = preds[entry].prediction();//Prediction的結果
	//instruction
	int index = addressToIndex(next, insts);//為第幾個instruction
	bool outcome = insts[index].exection();//執行運算
	if (outcome)
	{
		string nextLabel = insts[index].getLabel();
		next = labelToAddress(nextLabel, labs);
	}
	else
		next = insts[index + 1].getAddress();
	cout << "entry: " << entry << "\t\t" << insts[index] << endl;
	cout << "prediction: ";
	if (predBranch)
		cout << "T";
	else
		cout << "N";
	cout << "\t\t";
	cout << "outcome: ";
	if (outcome)
		cout << "T";
	else
		cout << "N";
	cout << endl;

	for (int i = 0; i < preds.size(); i++)
		cout << i << "." << preds[i] << endl;	
	preds[entry].update(predBranch, outcome);
	cout << "misprediciton: " << preds[entry].getMispred() << endl;
	cout << "------------------------------------------" << endl;

}
system("pause");
```
next為此次要實行的instruction的address

先找到為第幾個entry，為(num/4)%4，

因為instruction為32 bit為4 bytes，

故address都是4的倍數，

最後再對num取餘數即可獲得entry的index，

根據其就可以獲得prediction的結果，

而再根據next可找出為第幾個instruction，

具可以運算出結果來瞭解是否會branch，

若是會branch根據其Label來找到要branch的address，

若是不會branch，next就等於下一個的instruction的位置，

最後在輸出詳細的數據包含此次的entry、prediction和outcome，

再輸出每個prediction的狀態，

並根據outcome和prediction更新predictor的狀態

最後在輸出misprediction的數量即可
