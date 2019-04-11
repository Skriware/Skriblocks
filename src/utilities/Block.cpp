#include "Block.h"

Skribot* Block::robot = NULL;

Block::Block() {
  input_block = NULL;
  next = NULL;
};

Block::~Block() {
  
}

Block::Block(Block* b) {
  next = b;
  input_block = NULL;
  next = NULL;
  actionID = 1000;
}

Block::Block(byte ID,byte _nextID,int _actionID,byte _InputID, byte _OutputID) {
  blockID = ID;
  inputblockID = _InputID;
  outputblockID = _OutputID;
  actionID = _actionID;
  nextblockID = _nextID;
  input_block = NULL;
  next = NULL;
  output_block = NULL;
}

byte Block::getActionID(){
  return(actionID);
}

byte Block::getInputID(){
  return(inputblockID);
}

byte Block::getOutputID(){
  return(outputblockID);
}

byte Block::getID(){
  return(blockID);
}

byte Block::getNextID(){
  return(nextblockID);
}

 void Block::do_action() {
 /*
    0 up regular actions

    100 up reading from sensors actions

    100 down custom actions(for pilot control)
 */ 
 byte msg[]  = {9,1,0xe7,2, 0xe7,3, 0xe7,4, 0x00,5, 0x81,6, 0x42,7, 0x3c,8, 0x00,0x0A,15};
 byte msg1[] = {9,1,0xe7,2, 0xe7,3, 0xe7,4, 0x00,5, 0x00,6, 0x00,7, 0x7e,8, 0x81,0x0A,15};
  switch(actionID){
     case 0:
        Block::robot->Stop();
        break;
    case 1:
        //UserFunction_3(input_block->get_output());
      if(!Block::robot->config_mode){
        Block::robot->SetSpeed(output_block->get_output() + 155);
        Block::robot->MoveForward(input_block->get_output());
      }else{
        Block::robot->Invert_Left_Rotors(input_block->get_output()/1000);
      }
        break;
    case 2:
        if(!Block::robot->config_mode){
        Block::robot->SetSpeed(output_block->get_output() + 155);
        Block::robot->MoveBack(input_block->get_output());
         }else{
        Block::robot->Invert_Right_Rotors(input_block->get_output()/1000);
        }
        break;
    case 3:
        if(!Block::robot->config_mode){
          Block::robot->SetSpeed(255);
          Block::robot->FaceRight(input_block->get_output());
        }else{
          Block::robot->Scale_Right_Rotors(input_block->get_output()/1000);
        }
        break;
    case 4:
        if(!Block::robot->config_mode){
          Block::robot->SetSpeed(255);
          Block::robot->FaceLeft(input_block->get_output());
        }else{
          Block::robot->Scale_Left_Rotors(input_block->get_output()/1000);
        }
        break;
    case 5:
        //UserFunction_3(input_block->get_output());
        Block::robot->wait_And_Check_BLE_Connection(input_block->get_output(),10);
        break;
    case 6:
        Block::robot->Stop();
        break;
    case 8:
        Block::robot->CloseClaw();
        Block::robot->wait_And_Check_BLE_Connection(200,10);
        break;
    case 9:
         //Block::robot->comm->SPITransfer(msg1);
       
        Block::robot->OpenClaw();
        Block::robot->wait_And_Check_BLE_Connection(200,10);
        break;
    case 10:
       
        Block::robot->Pick_Up();
         Block::robot->wait_And_Check_BLE_Connection(200,10);
        break;
    case 11:
       
        Block::robot->Put_Down();
        Block::robot->wait_And_Check_BLE_Connection(200,10);
        break;
    case 12:
         switch(input_block->get_output()){
            case 0 : 
                Block::robot->TurnLEDOn(255,0,0);
            break;
             case 1 : 
                Block::robot->TurnLEDOn(0,0,255);
            break;
             case 2 : 
                Block::robot->TurnLEDOn(0,255,0);
            break;
             case 4 : 
                Block::robot->TurnLEDOn(255,0,255);
            break;
             case 5 : 
                Block::robot->TurnLEDOn(255,255,255);
            break;
             case 3 : 
                Block::robot->TurnLEDOn(184, 255, 3);
            break;
            default:

            break;
          }
          Block::robot->wait_And_Check_BLE_Connection(10,5);
        break;
    case 13:
        Block::robot->TurnLEDOff();
        break;
    case 94:
        UserFunction_5(input_block->get_output(),output_block->get_output());
        break;
    case 95:
        output_block->set_output(UserFunction_4());
        break;
    case 96:
        output_block->set_output(UserFunction_3(input_block->get_output()));
        break;
    case 97:
        UserFunction_2(input_block->get_output());
        break;
    case 98:
        UserFunction_1();
        break; 
    case 99:
        Block::robot->RawRotorMove(input_block->get_output(),output_block->get_output());
        break;
    case 101:
        output_block->set_output(Block::robot->ReadDistSensor(input_block->get_output()));
        break;
    case 102:
        output_block->set_output(Block::robot->ReadLineSensor(input_block->get_output()));
        break;
    case 255:
      //Saved for loops and ifs
      break;
    default :
        break;

  }
  //Serial.println("Action Done!");
}

void Block::set_next(Block *_next) {
  next = _next;
}

void Block::set_input(Block *_input){
  input_block = _input;
}

void Block::set_output_block(Block *_output){
 output_block = _output;
}
Block* Block::get_next() {
  return(next);
}

int Block::get_output() {
  return output;
}

String Block::get_string_output(){
  return("");
}

void Block::setRobot(Skribot *_robot){
  Block::robot = _robot;
}


void Block::set_output(int x){
  output = x;
}

bool Block::get_bool_output(){
  return(false);
}

