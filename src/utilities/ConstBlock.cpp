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

}

ConstBlock::~ConstBlock(){}

/*ConstBlock::ConstBlock(int id , String value){
	blockID = id;
	string_output = value;
	input_block = NULL;
  next = NULL;
  output_block = NULL;
}

String ConstBlock::get_string_output(){
  return(string_output);
}*/
