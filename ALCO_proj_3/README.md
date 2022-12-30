# ALCO_proj_3

## Video
<https://youtu.be/U_cpSb4RdTA>
## 前提
1. fetch 為一個cycle
2. execute add為 5 cycle mul為 19 cycle
3. write 為一個cycle
4. 若是有WAW WAR 的dependency 需要等在write的當前的cycle即可取到值
5. RS 釋放出空間的下一個cycle才能再次fetch新的instruction
6. register的初值分別為 0、2、4、6、8

## Input
```
ADDI F1, F2, 1
SUB F1, F3, F4
DIV F1, F2, F3
MUL F2, F3, F4
ADD F2, F4, F2
ADDI F4, F1, 2
MUL F5, F5, F5
ADD F1, F4, F4
```
並放入到Input.txt中

## 程式碼解釋
--- 
## Global Variable

### A. 相關參數
```C++
int numAddRS = 3;
int numMulRS = 2;
```

ADD 的RS大小為3

MUL 的RS大小為2

### B. registers
```
Register* registers[6];
```
為六個pointer指向Register的陣列分別對應R1~R6

### C. needCycle
```
map <string, int> needCycle;
```
紀錄對應的operator所需的cycle

## Class

### A. Instruction
--- 
### Private:

### 1. myRegIndex
```c++
vector <int> myRegIndex
```
用來存放所屬的register的index
ex: `ADDI F1, F2, 1`
則會存放1、2

### 2. myReg
```c++
vector < Register* > myReg
```
用來存放這個Instruction所使用到的Register

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

### 5. cycle
```C++
int cycle = -1;
```
用來記錄這個Instruction進入RS的cycle

### Public
### 1. Constructor

```c
Instruction()
	{}
```
為預設的constructor

```c++
Instruction(string myOperator, int reg1, int reg2, int reg3)
		:myOperator(myOperator)
{
    myRegIndex.push_back(reg1);
    myRegIndex.push_back(reg2);
    myRegIndex.push_back(reg3);
}
```
為用來建構一個Instruction包含三個Register的Constructor

Ex: `SUB F1, F3, F4`

```C++
Instruction(string myOperator, int reg1, int reg2, string imm)
    :myOperator(myOperator),
    myImmediate(stoi(imm))
{
    myRegIndex.push_back(reg1);
    myRegIndex.push_back(reg2);
}
```
為用來建構一個Instruction包含二個Register和一個Immediate的Constructor

Ex: `ADDI F1, F2, 1`

### 2. exection
```c++
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
```
判斷當myOperator為特定的Operator時，

執行相對應的運算，並將output的register的renaming清除，

且執行完後呼叫clear，來表示說已經結束完畢

### 3. setReg
```c++
void setReg()
	{
		for (int i = 0; i < myRegIndex.size();i++)
			myReg.push_back( registers[myRegIndex[i]]);
	}
```
透過myRegIndex內的Index來來設定myReg內所儲存的*Register

### 4. getOperator
```c++
string getOperator()
{
    return myOperator;
}
```
回傳myOperator

### 5. getSymbol
```c++
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
```
根據對應的operator來回傳相對應的符號

### 7. getType
```C++
string getType()
{
    if (myOperator == "MUL" || myOperator == "DIV")
        return "MUL";
    else
        return "ADD";
}
```
根據對應的myOperator來回傳她是哪一種的reservation station

### 8. clear
```C++
void clear()
{
    myOperator.clear();
    myReg.clear();
    myImmediate = 0;
    cycle = -1;
}
```
將這個Instruction內的data member做初始化

### 9. ready
```C++
bool ready()
{
    for (int i = 1; i < myReg.size(); i++)
    {
        if (myReg[i]->getName().size() != 0)
            return false;
    }
    return true;
}
```
用來判斷這個Instruction是否已經ready可以來去做執行

判斷的條件是其Register的renaming皆沒有

### 10. empty
``` c++
bool empty()
{
    return myOperator.empty();
}
```
回傳這個Instruction是否為empty

判斷的條件為myOperator是否是empty

### 11. setCycle
```C++
void setCycle(int c)
{
    cycle = c;
}
```
設定這個Instruction進入RS的cycle

### 12. getCycle
```C++
int getCycle()
{
    return cycle;
}
```
回傳這個Instruction進入RS的cycle

### 13. getOutputRegister
```C++
Register* getOutputRegister()
{
    return myReg[0];
}
```
回傳這個Instruction的output register

### 14. setOutputRegister
```C++
void setOutputRegister(Register* r)
{
    myReg[0] = r;
}
```
設定這個Instruction的output register

### 15. getOutputRegisterIndex
```C++
int getOutputRegisterIndex()
{
    return myRegIndex[0];
}
```

回傳這個Instruction的output register 的Index

### 16. getReg
```C++
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
```

### B. Register
--- 
### Private
### 1. myValue
```c
int myValue
```
用來記錄Register內的值

### 2. renaming
```C++
string renaming
```
用來記錄其別名

### Public
### 1. Constructor
```c++
Register()
	:myValue()
{}
```
初始化myValue

```c++
Register(int n)
    :myValue(n)
{}
```
根據傳入的來設定myValue

### 2. operator=
```c++
int& operator=(int value)
{
	myValue = value;
	return myValue;
}
```
將傳入的型態為int的值傳給myValue

並回傳myValue


```C++
void operator=(Register r)
{
    myValue = r.myValue;
}
```
將傳入的Register內的myValue賦予本身的myValue

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

### 4. operator*
```c++
int& operator*(Register r)
{
    int v = myValue * r.myValue;
    return v;
}
```
將傳入的Register的myValue與本身的myValue做相乘，並回傳

### 5. operator/
```c
int& operator/(Register r)
{
    int v = myValue / r.myValue;
    return v;
}
```
將傳入的Register的myValue與本身的myValue做相除，並回傳

### 6. getValue
```c
int getValue()
{
	return myValue;
}
```
回傳myValue

### 7. setName
```C++
void setName(string s)
{
    renaming = s;
}
```
設定renameing

### 8. getName
```C++
string& getName()
{
    return renaming;
}
```
回傳renaming

### C. Rservation
---
### Private
### 1. numRS
```C++
int numRS
```
用來記錄可以儲存Instruction的數量

### 2. count
```c
int count = 0
```
用來記錄當前RS內Instruction的執行所剩餘的cycle

### 3. myRS
```C
vector <Instruction> myRS
```
為一個vector用來儲存當前RS內的Instruction

### 4. buffer
```C
Instruction* buffer
```
為一個pointer指向Instruction用來儲存當前正在執行的Instruction

### Public
### 1. Construction
```c
ReservationStation()
    :numRS()
{
    buffer = nullptr;
}
```
初始化Reservation Station

### 2. set
```C
void set(int num)
{
    numRS = num;
    myRS.resize(num);
}
```
用來設定numRS和requireCycle

### 3. full
```C
bool full()
{
    for (int i = 0; i < myRS.size(); i++)
    {
        if (myRS[i].empty())
            return false;
    }
    return true;
}
```
用來判斷RS內的Instruction是否已滿


### 4. insertInst
```C++
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
```
先找到為空的RS，然後設定傳入的instruction進入RS的cycle，

接著在呼叫setReg來設定inst內的register，然後建一個temp，

其值與inst原本的output register的值相同，在設定其renaming，

因為RAT中的renaming會被覆蓋為最新的，故我們的registers內的pointer，

也要更新，故這樣做的幫法的好處是當RAT的第一格被覆蓋時，其被覆蓋的

register寫回時只會寫回到有被使用的register並不會寫回到RAT

### 5. getRSName
```C
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
```
根據傳入的instruction和index來回傳其名稱 ex:`RS1`

### 6. execute
```C
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
```
當buffer不為空的時候，代表有instruction

正在執行故count-1，當count為零時，代表已經執行完畢，便呼叫exection

來執行，並buffer指到nullptr，回傳true代表有instruction執行完成

### 7. update
```C
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
```
當buffer為空的時候，會從最小的cycle來找可執行的instruction，

其判斷的條件為要ready且不為當前的cycle，後者其意義為不能fetch進來的

第一個cycle就來做執行

### 8. getMaxCycle
```C
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
```
回傳當前在RS中最小的cycle，若是為-1代表存取到被清除的instruction，

則回傳0

### 9. getMaxCycle
```C
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
```
回傳當前在RS中最大的cycle，若是為-1代表存取到被清除的instruction，

則回傳0

### 10. getSize
```C
int getSize()
{
    return numRS;
}
```
回傳RS的Size

### 11. getInstruction
```C
Instruction getInstruction(int n)
{
    return myRS[n];
}
```
回傳RS內第n個instruction

### 12. outputBuffer
```C
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
```
若是buffer為指向nullptr，則輸出emtpy，反之則輸出其operator和數值

## Function
### A. readFile
```C
void readFile(vector <Instruction>& insts)
{
	ifstream inFile("Input.txt", ios::in);
	if (!inFile)
	{
		cout << "Input.txt could not be openend!" << endl;
		exit(1);
	}
	vector <string> input;
	string temp;
	while (getline(inFile, temp))
		input.push_back(temp);
	for (int i = 0; i < input.size(); i++)
		insts.push_back(converInst(input[i],insts));
}
```
讀取txt檔，並將內部的instruction放入insts，

使用getline的方式來進行讀檔，並放入到input中，

最後再呼叫converInst來取得Instruction並push_back到insts中

### B. converInst
```C
Instruction converInst(string input, vector <Instruction>& insts)
```
讀取txt檔，並將內部的instruction放入insts
```C++
string temp;
string Opterator;
vector <string> regs;
string reg;
string imm;
int i = 0;
while (input[i] != ' ')
    Opterator.push_back(input[i++]);
i++;//remove ' '
i++;//romove 'F'
reg.push_back(input[i]);
regs.push_back(reg);
reg.clear();
```
i為當前input的index

因為operator和第一個register中間有隔一個空格，可以代表空格前的都是

operator，之後再+2代表除去一個空格和F，則此時的i為registers中的index

```C++
i += 4;//remove " ,F"
reg.push_back(input[i]);
regs.push_back(reg);
reg.clear();
i += 3;
if (input[i] == 'F')
{
    i++;
    reg.push_back(input[i]);
    regs.push_back(reg);
}
else
{
    imm.push_back(input[i]);
}
```
接著i在加等於4來除去上一個register 的index 和中間的空格、逗號和F

此時的i為第二個register的index，此時在家等於三除去register 的index 和中間的空格、逗號和F

若此時input[i]為F則代表還有一個register，反之還有一個immediate的值


```C++
if (regs.size() == 3)
{
    Instruction temp(Opterator, stoi(regs[0]), stoi(regs[1]), stoi(regs[2]));
    return temp;
}
else
{
    Instruction temp(Opterator, stoi(regs[0]), stoi(regs[1]), imm);
    return temp;
}
```
最後再根據相對應的registers的數量來呼叫對應的constructor

### C. output
```C++
void output(ReservationStation RS[], int cycle)
```
用來輸出當前RS、RAT、RF的狀態

```C++
cout << endl << "Cycle: " << cycle << endl << endl;
cout << "   _ RF __" << endl;
for (int i = 1; i < 6; i++)
    cout << 'F' << i << " |" << setw(4) << registers[i]->getValue() << " |" << endl;
cout << "   -------" << endl;
cout << endl;
```
先輸出當前的cycle，在輸出RF內全部的值

```C
cout << "   _ RAT __" << endl;
for (int i = 1; i < 6; i++)
    cout << 'F' << i << " |" << setw(4) << registers[i]->getName() << " |" << endl;
cout << "   -------" << endl;
cout << endl;
```
再輸出RAT內的renaming

```C
cout << "    _ RS _________________" << endl;
for (int i = 0; i < RS[0].getSize(); i++)
{
    Instruction temp = RS[0].getInstruction(i);
    cout << "RS" << i + 1 << " |"
        << setw(5) << temp.getSymbol() << " |"
        << setw(5) << temp.getReg(1) << " |"
        << setw(5) << temp.getReg(2) << " |" << endl;
}
cout << "    ----------------------" << endl;
RS[0].outputBuffer();
cout << endl;
```
接著輸出ADD的RS內的instruction，和當前buffer內的狀態

```C++
cout << "    ______________________" << endl;
for (int i = 0; i < RS[1].getSize(); i++)
{
    Instruction temp = RS[1].getInstruction(i);
    cout << "RS" << i + 4 << " |"
        << setw(5) << temp.getSymbol() << " |"
        << setw(5) << temp.getReg(1) << " |"
        << setw(5) << temp.getReg(2) << " |" << endl;
}
cout << "    ----------------------" << endl;
RS[1].outputBuffer();
cout << endl << "--------------------" << endl ;
```
最後在輸出輸出MUL的RS內的instruction，和當前buffer內的狀態

## Main Function
```C++
	needCycle = { {"ADD",2},{"ADDI",2},{"SUB",2},{"MUL",10},{"DIV",40} };
	for (int i = 1; i <= 5; i++)
		registers[i] = new Register((i - 1) * 2);
	vector <Instruction> insts;//用來裝所有的instruction
	ReservationStation RS[2];//用來裝所有的RS
	RS[0].set(numAddRS);
	RS[1].set(numMulRS);
	int cycle = 1;//當前的cycle
	int index = 0;//當前fetch到的index
	int done = 0;//當前instruction完成的數量
	readFile(insts);
```
先設定相對應的operator所需的cycle，在來設定registers，

而ints為用來裝所有的instruction的vector，

RS為用來裝所有的Reservation Station其size為二

cycle為當前的cycle，index為當前fetch到的index，

done為當前instruction完成的數量，再呼叫ints來取得所有的Insts


```C++
while (done != insts.size())
	{
		if (index <insts.size())
		{
			if (insts[index].getType() == "ADD" && !RS[0].full())
			{
				insts[index].setCycle(cycle);
				if (RS[0].insertInst(insts[index], cycle, "ADD"))
					index++;
			}
			else if(insts[index].getType() == "MUL" && !RS[1].full())
			{
				insts[index].setCycle(cycle);
				if (RS[1].insertInst(insts[index], cycle, "ADD"))
					index++;
			}
		}
```
當完成的index的數量等於insts的總數時跳出迴圈，

先判斷index是否小於instruction的總數，

在判斷其instruction的type來找對應的RS在判斷是否不為已滿，

若是皆符合，則設定instruction進入RS的cycle再來去insert到RS內，

此時index+1，

```C++
		if (RS[0].execute())
			done++;
		if (RS[1].execute())
			done++;
		RS[0].update(cycle);
		RS[1].update(cycle);
		output(RS, cycle);
		cycle++;
	}
system("pause");
return 0;
```
之後再來去分別去呼叫RS的execute，若是true代表有instruction完成，

則done加一，接著再分別呼叫update並傳入當前的cycle

最後在輸出當前的狀態，cycle再加一
