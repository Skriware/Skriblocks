#ifndef Block_Handler
#define Block_Handler
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
	void addConst(int id, int32_t value);
	void addConst(int id,String value);																			//Adding Blocks types
	
	bool MakeConections();
	bool doBlock(bool loopmode = false);																							//Making conectons(assigning pointers from IDs) 
																												// Starting code
	

	int freeRam();
	int cti(char c);
	int32_t readInt();																								//BT message parsing help functions
	int Handle_Msg();																							// Handle one line of the message
	void ReadfromBT();	
	void clear();	
	void init();																					// BT message hendler												

	bool runCode;

	static const int blockList_MAX 			= 200;
	static const int IfblockList_MAX 		= 20;
	static const int LoopblockList_MAX 		= 20;
	static const int LogicblockList_MAX 	= 40;
	static const int AritmeticblockList_MAX	= 15;

	Block *blockList[blockList_MAX];
	Conditional *IfblockList[IfblockList_MAX];
	Loop *LoopblockList[LoopblockList_MAX];
	AritmeticBlock *AritmeticblockList[AritmeticblockList_MAX];
	LogicBlock *LogicblockList[LogicblockList_MAX];
	Block *current;
	Block *StartBlock;
	int blockList_N;
	int IfblockList_N;
	int LoopblockList_N;
	int LogicblockList_N;
	int AritmeticblockList_N;					//Actual number of blocks in lists
	int Mcursor;								// variable for message parsing
	char AllMessage[1600];
	int messageLength;

	

};

#endif