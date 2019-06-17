#ifndef Block_H
#define Block_H
#include <Skribot.h>
#include "UserFunctions.h"

class Block;

class Block {
public:
	Block();
	Block(Block*);
	Block(byte id,byte _nextBlockID,int _actionID = 0,byte _InputID = 0, byte _OutputID = 0);
	virtual ~Block();
	void do_action();
	virtual	Block* get_next();
	virtual bool get_bool_output();
  	virtual int32_t get_output();
  	void set_output(int x);
  	byte getID();
  	bool set_next(Block* blockList[],int blockList_N);
  	bool set_input(Block* blockList[],int blockList_N);
  	bool set_output_block(Block* blockList[],int blockList_N);
  	byte getNextID();
  	byte getInputID();
  	byte getOutputID();
  	byte getActionID();
  	virtual String get_string_output();
  	static void setRobot(Skribot *rob);
  	static Skribot *robot;



protected:
	byte blockID;
	int32_t output;
	Block *input_block;
	Block *output_block;
	Block *next; 
	byte inputblockID;
	byte outputblockID;
	byte nextblockID;
	byte actionID;
	
	
	};

#endif
