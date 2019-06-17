#ifndef ConstBlock_H
#define ConstBlock_H
#include "Block.h"
class ConstBlock : public Block {

public:
  ConstBlock(int id,int32_t const);
  ~ConstBlock(); 

};

#endif