#ifndef ConstBlock_H
#define ConstBlock_H
#include "Block.h"
class ConstBlock : public Block {

public:
  ConstBlock(int id,int const);
  ConstBlock(int id,String const);
  String get_string_output();
  ~ConstBlock(); 
 protected:
 	String string_output;

};

#endif