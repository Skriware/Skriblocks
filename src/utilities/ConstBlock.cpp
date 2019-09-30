#include "ConstBlock.h"

ConstBlock::ConstBlock(int id, int32_t value){
	  blockID = id;
	  output = value;
	  input_block 	= NULL;
  	next 			= NULL;
  	output_block 	= NULL;
  	inputblockID 	= 0;
  	outputblockID 	= 0;
  	actionID 		= 0;
  	nextblockID 	= 0;
    table = NULL;

}

ConstBlock::ConstBlock(int id, uint8_t *tab){
    blockID = id;
    if(tab !=NULL){
      output = tab[0];
    }else{
      output = 0;
    }
    input_block   = NULL;
    next      = NULL;
    output_block  = NULL;
    inputblockID  = 0;
    outputblockID   = 0;
    actionID    = 0;
    nextblockID   = 0;
    table = tab;

}

ConstBlock::~ConstBlock(){}

uint8_t* ConstBlock::get_table_output(){
  return(table);
}

