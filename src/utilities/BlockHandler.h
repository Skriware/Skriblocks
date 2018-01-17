#ifndef Block_Handler
#define Block_Handler
#include <SkriBot.h>
#include "Block.h"
#include "Loop.h"
#include "Conditional.h"
#include "LogicBlock.h"
#include "ConstBlock.h"
#include "LogicCompare.h"
#include "AritmeticBlock.h"
class BlockHandler {
public:
	BlockHandler();

	void addLoop(int id,	int startBlockID,	int endBlockID,		int count);
	void addBlock(int id,	int _nextBlockID,	int _actionID ,	int _inputBlockID = -1, int _outputBlockID = -1);
	void addIf(int id,int _next_true, int _next_false, int _logic_block);
	void addLogic(int id,int logicOperation,int _input_left, int _input_right);
	void addLogicCompare(int id,int _logicOperation,int _input_left, int _input_right);
	void addAritmeticBlock(int id,int _operation,int _left,int _right);
	void addConst(int id, float value);
	void addConst(int id,String value);																			//Adding Blocks types
	
	void MakeConections();
	bool doBlock(bool loopmode);																							//Making conectons(assigning pointers from IDs) 
																												// Starting code
	

	int freeRam();
	int cti(char c);
	int readInt();																								//BT message parsing help functions
	int Handle_Msg();																							// Handle one line of the message
	void ReadfromBT();	
	void clear();	
	void init();																					// BT message hendler												

	bool runCode;

	Block *blockList[60];
	Conditional *IfblockList[10];
	Loop *LoopblockList[10];
	AritmeticBlock *AritmeticblockList[20];
	LogicBlock *LogicblockList[20];
	ConstBlock *ConstblockList[30];
	Block *current;
	Block *StartBlock;
	int blockList_N;
	int IfblockList_N;
	int LoopblockList_N;
	int LogicblockList_N;
	int ConstblockList_N;
	int AritmeticblockList_N;						//Actual number of blocks in lists
	int Mcursor;								// variable for message parsing
	char AllMessage[800];
	int messageLength;

};

#endif