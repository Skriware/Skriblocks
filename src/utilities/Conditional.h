#ifndef Conditional_H
#define Conditional_H

#include "Block.h"
#include "LogicBlock.h"
#include "LogicCompare.h"
class Conditional : public Block {
public:
  Conditional(byte,byte,byte,byte);
  ~Conditional();
  void do_action();
  Block* get_next();
  void set_logics(Block *blockList[60], int blockList_N, LogicBlock *lblockList[20], int lblockList_N);
protected:
	Block *next_true;
	Block *next_false;
  LogicBlock* logic_block;
  byte next_True_ID;
  byte next_False_ID;
  byte first_LogicblockID;
  byte operation;
  bool main_logic_value;

  

};

#endif
