#ifndef ConstBlock_H
#define ConstBlock_H
#include "Block.h"
class ConstBlock : public Block {

public:
  ConstBlock(int id,int32_t const);
  ConstBlock(int id, uint8_t *tab);
  ~ConstBlock();
  uint8_t* get_table_output();
  uint8_t *table;

};

#endif