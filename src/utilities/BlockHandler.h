#ifndef Block_Handler
#define Block_Handler
#include "Block.h"
#include "Loop.h"
#include "Conditional.h"
#include "LogicBlock.h"
#include "ConstBlock.h"
#include "LogicCompare.h"
#include "AritmeticBlock.h"
#include "InterruptHandler.h"

#define MIN_CONNECTION_BREAK_TIME 100

#define INVALID_MSG_ERROR_CODE 255
#define TIMEOUT_ERROR_CODE 254
#define MESSAGE_TIMEOUT 1000
#define ACK_RESEND_TIME 300
#define NO_MSG_CODE 0
#define CODE_COMPLETE 2
#define CODE_PASSED 1

//Line Tags
#define BAPTISED 'b'
#define RENAME 'R'
#define VERSION 'V'
#define RESET 'r'
#define END 'E'
#define BEGIN 'B'

class BlockHandler {


public:
	BlockHandler();

	void addLoop(int id,	int startBlockID,	int endBlockID,		int count);
	void addBlock(int id,	int _nextBlockID,int _actionID,int _input,int output);
	void addIf(int id,int _next_true, int _next_false, int _logic_block);
	void addLogic(int id,int logicOperation,int _input_left, int _input_right);
	void addLogicCompare(int id,int _logicOperation,int _input_left, int _input_right);
	void addInterrupt(byte type,byte input,byte trigger,byte _priority,byte _starting_block,int value = 0);
	void addAritmeticBlock(int id,int _operation,int _left,int _right);
	void addConst(int id, int32_t value);
	void addConst(int id, int32_t *value,byte N);
	void addConst(int id,String value);	
	bool checkForInterrupts();																		//Adding Blocks types
	
	void active_wait(uint32_t ms,int interval);

	bool MakeConections();
	bool doBlock(bool loopmode = false);
	
	byte readMessageLine();
	byte readCodeLine();
	void processMessageLine(byte LineCode);
	void AddToMessage(char x);
	void CheckLongCodes(char *x);
	bool CheckForTimeout();																							//Making conectons(assigning pointers from IDs) 
																												// Starting code
	int freeRam();
	int cti(char c);
	int32_t readInt();																						//BT message parsing help functions
	int Handle_Msg();																							// Handle one line of the message
	void ReadfromBT();	
	void clear();	
	void init();																					// BT message hendler												

	bool runCode,transfereBlocks;

	 #define blockList_MAX 			200
	 #define IfblockList_MAX 		20
	 #define LoopblockList_MAX 		20
	 #define LogicblockList_MAX 	40
	 #define AritmeticblockList_MAX	15
	 #define MAX_INTERRUPTS 8

	Block *blockList[blockList_MAX];
	Conditional *IfblockList[IfblockList_MAX];
	Loop *LoopblockList[LoopblockList_MAX];
	AritmeticBlock *AritmeticblockList[AritmeticblockList_MAX];
	LogicBlock *LogicblockList[LogicblockList_MAX];
	Block *current;
	Block *StartBlock;
	InterruptHandler *Interrupts[MAX_INTERRUPTS];
	byte interrupt_running;
	byte interrupts_N;
	uint32_t millis_left_from_interrupt;
	long last_connection_break;
	bool interruped_precesed;
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