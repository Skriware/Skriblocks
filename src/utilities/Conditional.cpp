#include "Conditional.h"

Conditional::~Conditional() {}


Conditional::Conditional(byte id,byte _next_true, byte _next_false, byte _logic_block){
  next_True_ID  = _next_true;
  next_False_ID = _next_false;
  first_LogicblockID = _logic_block;
  blockID = id;
  input_block = NULL;
  next = NULL;
  output_block = NULL;
  next_true = NULL;
  next_false = NULL;
  logic_block = NULL;

}

void Conditional::set_logics(Block* blockList[], int blockList_N,LogicBlock* logicblockList[], int logicblockList_N){
  Serial.println("Setting logics for IF!");
  for(int ii = 0; ii < blockList_N; ii++){
      if(blockList[ii]->getID() == next_True_ID){
            next_true = blockList[ii];
            Serial.println("True and False set!");
      }else if(blockList[ii]->getID() == next_False_ID){
            next_false = blockList[ii];
            Serial.println("True and False set!");
      }else if(blockList[ii]->getID() == first_LogicblockID){
            logic_block = (LogicBlock*)blockList[ii];
            Serial.println("Logic set!");
      }
   
}
 
  /*for(int yy = 0; yy < logicblockList_N; yy++){
    if(logicblockList[yy]->getID() == first_LogicblockID){
            logic_block = logicblockList[yy];
      }
  }*/
  

}




void Conditional::do_action() {
  /*LogicBlock *current = logic_block;
  if(operation){
    main_logic_value = true;
  }else{
    main_logic_value = false;
  }     
        while (current != NULL) {
          bool temp = current->get_value();
          Serial.print("L");
          Serial.println(current->getID());
          if(operation){
            main_logic_value = main_logic_value && temp;
          }else{
            main_logic_value = main_logic_value || temp;
          }
          current = (LogicBlock*)current->get_next(); 
        }
*/
}

Block* Conditional::get_next() {
  Serial.print("Pointing to logic block:");
  Serial.println(logic_block->getID());
  if(logic_block->get_bool_output())
    return next_true;
  else
    return next_false;
}
