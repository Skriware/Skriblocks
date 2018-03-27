#ifndef Block_H
#define Block_H
#include <SkriBot.h>

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
  	virtual int get_output();
  	void set_output(int x);
  	byte getID();
  	void set_next(Block *_next);
  	void set_input(Block *_input);
  	void set_output_block(Block *_output_block);
  	byte getNextID();
  	byte getInputID();
  	byte getOutputID();
  	virtual String get_string_output();
  	static void setRobot(SkriBot *rob);
  	static SkriBot *robot;



protected:
	byte blockID;
	int output;
	Block *input_block;
	Block *output_block;
	Block *next; 
	byte inputblockID;
	byte outputblockID;
	byte nextblockID;
	int actionID;
	
	
	};

#endif
