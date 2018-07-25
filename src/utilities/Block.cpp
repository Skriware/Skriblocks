#include "Block.h"

Skribot* Block::robot = NULL;

Block::Block() {
  input_block = NULL;
  next = NULL;
};

Block::~Block() {}

Block::Block(Block* b) {
  next = b;
  input_block = NULL;
  next = NULL;
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
  //output = 69;
}

/*Block::Block(byte ID,byte _nextID ,byte _actionID ,byte _byteInput) {
  blockID = ID;
  byteInput = _byteInput; 
  actionID = _actionID;
  nextblockID = _nextID;
}*/

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

  switch(actionID){
     case 0:
        Block::robot->Stop();
        break;
    case 1:
        Block::robot->SetSpeed(output_block->get_output() + 155);
        Block::robot->MoveForward(input_block->get_output());
        break;
    case 2:
        Block::robot->SetSpeed(output_block->get_output() + 155);
        Block::robot->MoveBack(input_block->get_output());
        break;
    case 3:
        Block::robot->SetSpeed(255);
        Block::robot->FaceRight(input_block->get_output());
        break;
    case 4:
        Block::robot->SetSpeed(255);
        Block::robot->FaceLeft(input_block->get_output());
        break;
    case 5:
        Block::robot->wait_And_Check_BLE_Connection(input_block->get_output(),10);
        break;
    case 6:
        Block::robot->Stop();
        break;
    case 8:
        delay(500);
        Block::robot->CloseClaw();
        break;
    case 9:
        delay(500);
        Block::robot->OpenClaw();
        break;
    case 10:
        delay(500);
        Block::robot->Pick_Up();
        break;
    case 11:
        delay(500);
        Block::robot->Put_Down();
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
    case 99:
        Block::robot->RawRotorMove(input_block->get_output(),output_block->get_output());
        break;
    case 101:
        output_block->set_output(Block::robot->ReadDistSensor(input_block->get_output()));
        break;
    case 102:
        output_block->set_output(Block::robot->ReadLineSensor(input_block->get_output()));
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

