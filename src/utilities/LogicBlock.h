#ifndef LogicBlock_H
#define LogicBlock_H
#include "Block.h"
class LogicBlock : public Block {

public:
  LogicBlock(byte id,byte logicOperation,byte _input_left, byte _input_right);
  ~LogicBlock();
  LogicBlock();
  virtual void get_value();
  void do_action();
  void set_logics(Block *blockList[100], int blockList_N);
  bool get_bool_output();
protected:
  byte input_Left_ID;
  byte input_Right_ID;
  Block *input_left;
	Block *input_right;
  int val_left, val_right;
  byte operation;
  bool bool_output;

};

#endif
