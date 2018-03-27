#ifndef AritmeticBlock_H
#define AritmeticBlock_H
#include "Block.h"
class AritmeticBlock : public Block {

public:
  AritmeticBlock(byte id, byte operation  ,byte left_id,byte rigth_id);
  void set_connections(Block *blockList[],int blockList_N);
  int get_output();
  ~AritmeticBlock(); 

 protected:
 	byte leftID,rightID,operation;
 	Block *input_left;
 	Block *input_right;
 	

};

#endif