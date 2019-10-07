#include "Block.h"
#include "BlockHandler.h"

Skribot* Block::robot = NULL;
BlockHandler* Block::BH = NULL;

Block::Block() {
  next = NULL;
  interrupted = false;
  action_with_no_interrupt = true;
  used_blocks_N = 0;
};

Block::~Block() {
  delete used_blocks;
  delete used_blocksIDs;
}

Block::Block(Block* b) {
  next = b;
  next = NULL;
  actionID = 1000;
  interrupted = false;
  action_with_no_interrupt = true;
}

Block::Block(byte id,byte _nextBlockID,int _actionID,byte *_usedblockIDs,byte Nused_blocks){
  blockID = id;
  actionID = _actionID;
  nextblockID = _nextBlockID;
  used_blocksIDs = _usedblockIDs;
  used_blocks_N = Nused_blocks;
  next = NULL;
  interrupted = false;
  action_with_no_interrupt = true;
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
action_with_no_interrupt = true;
        byte font[][8] = {{ 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e, 0x66, 0x66 }, // A
                          { 0x78, 0x7c, 0x6c, 0x7c, 0x7e, 0x66, 0x7e, 0x7c }, // B
                          { 0x7e, 0x7e, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x7e }, // C
                          { 0x7c, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x7e, 0x7c }, // D
                          { 0x7e, 0x7e, 0x60, 0x78, 0x78, 0x60, 0x7e, 0x7e }, // E
                          { 0x7e, 0x7e, 0x60, 0x78, 0x78, 0x60, 0x60, 0x60 }}; // F
  switch(actionID){
     case 0:
        Block::robot->Stop();
        break;
    case 1:
      if(!Block::robot->config_mode){
        Block::robot->SetSpeed(used_blocks[1]->get_output() + 155);
        Block::robot->MoveForward();
        Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
        Block::robot->Stop();
      }else{
        Block::robot->Invert_Left_Rotors(used_blocks[0]->get_output()/1000);
        EEPROM.write(EEPROM_LEFT_INVERT_ADDR,used_blocks[0]->get_output()/1000);
        if(!Block::robot->user_config){
          EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
          Block::robot->user_config = true;
          #ifdef ESP_H 
          EEPROM.commit(); 
          #endif
        }
      }
        break;
    case 2:
        if(!Block::robot->config_mode){
        Block::robot->SetSpeed(used_blocks[1]->get_output() + 155);
        Block::robot->MoveBack();
        Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
        Block::robot->Stop();
         }else{
        Block::robot->Invert_Right_Rotors(used_blocks[0]->get_output()/1000);
         EEPROM.write(EEPROM_RIGHT_INVERT_ADDR,used_blocks[0]->get_output()/1000);
         #ifdef ESP_H 
          EEPROM.commit(); 
          #endif
         if(!Block::robot->user_config){
          EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
          Block::robot->user_config = true;
          #ifdef ESP_H 
          EEPROM.commit(); 
          #endif
        }
        }
        break;
    case 3:
        if(!Block::robot->config_mode){
          Block::robot->SetSpeed(255);
          Block::robot->FaceLeft();
          Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
          Block::robot->Stop();
        }else{
          Block::robot->TurnLEDOn(184, 255, 3);
          Block::robot->Scale_Left_Rotors(used_blocks[0]->get_output()/1000);
          byte lscale = byte(used_blocks[0]->get_output()/1000);
          #ifdef DEBUG_MODE
            Serial.print("Scalling left rotor: ");
            Serial.println(lscale);
          #endif
          EEPROM.write(EEPROM_LEFT_SCALE_ADDR,lscale);
          #ifdef ESP_H 
          EEPROM.commit(); 
          #endif
          if(!Block::robot->user_config){
            EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
            Block::robot->user_config = true;
            #ifdef ESP_H 
            EEPROM.commit(); 
            #endif
          }
        }
        break;
    case 4:
        if(!Block::robot->config_mode){
          Block::robot->SetSpeed(255);
          Block::robot->FaceRight();
          Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
          Block::robot->Stop();
        }else{
          Block::robot->TurnLEDOn(184, 255, 3);
          Block::robot->Scale_Right_Rotors(used_blocks[0]->get_output()/1000);
          byte rscale = byte(used_blocks[0]->get_output()/1000);
          EEPROM.write(EEPROM_RIGHT_SCALE_ADDR,rscale);
          #ifdef DEBUG_MODE
            Serial.print("Scalling right rotor: ");
            Serial.println(rscale);
          #endif
          #ifdef ESP_H 
            EEPROM.commit(); 
            #endif
          if(!Block::robot->user_config){
            EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
            #ifdef ESP_H 
            EEPROM.commit(); 
            #endif
            Block::robot->user_config = true;
          }
        }
        break;
    case 5:
        Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
        break;
    case 6:
         Block::robot->Stop();
         Block::BH->active_wait(10,10,interrupted,&action_with_no_interrupt);
        break;
    case 8:
        Block::robot->CloseClaw();
        Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        break;
    case 9:
        if(!Block::robot->config_mode){
          Block::robot->OpenClaw();
          Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        }else{
          Serial.println("Config mode operation!");
          Block::robot->TurnLEDOn(0,0,0);
          for(byte ii = 6; ii<17;ii++){
            EEPROM.write(ii,255);
            #ifdef ESP_H 
            EEPROM.commit(); 
            #endif
            delay(100);
          }
          Block::robot->TurnLEDOn(255,255,255);
        }
        break;
    case 10:
          Block::robot->Pick_Up();
          Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        break;
    case 11:
        Block::robot->Put_Down();
        Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        break;
    case 12:
         switch(used_blocks[0]->get_output()){
            case 0 : 
                Block::robot->TurnLEDOn(255,0,0);
            break;
             case 1 : 
                if(!Block::robot->config_mode){
                  Block::robot->TurnLEDOn(0,0,255);
                }else{
                   Serial.println("Config mode operation!");
                   for(int zz = 0; zz < Block::robot->NLineSensors ; zz++){
                      Block::robot->TurnLEDOn(255,255,255);
                      Block::robot->LineSensors[zz]->Line_Readout();
                      Block::robot->TurnLEDOn(0,0,0);
                      Block::BH->active_wait(100,5);
                      if(Block::robot->LineSensors[zz]->GetSensorPin() == LINE_PIN_1)Block::robot->Write_EEPROM_INT(EEPROM_L1_BORDER_ADDR,Block::robot->LineSensors[zz]->GetLogicBorder());
                      if(Block::robot->LineSensors[zz]->GetSensorPin() == LINE_PIN_2)Block::robot->Write_EEPROM_INT(EEPROM_L2_BORDER_ADDR,Block::robot->LineSensors[zz]->GetLogicBorder());
                      if(Block::robot->LineSensors[zz]->GetSensorPin() == LINE_PIN_3)Block::robot->Write_EEPROM_INT(EEPROM_L3_BORDER_ADDR,Block::robot->LineSensors[zz]->GetLogicBorder());
                      #ifdef ESP_H 
                      EEPROM.commit(); 
                      #endif
                      delay(100);
                    }
                    if(!Block::robot->user_config){
                      EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
                      Block::robot->user_config = true;
                      #ifdef ESP_H 
                      EEPROM.commit(); 
                      #endif
                    }
              }
            break;
             case 2 : 
                Block::robot->TurnLEDOn(0,255,0);
            break;
             case 4 : 
                Block::robot->TurnLEDOn(255,0,255);
            break;
             case 5 : 
                if(!Block::robot->config_mode){
                  Block::robot->TurnLEDOn(255,255,255);
                }else{
                   Serial.println("Config mode operation!");
                   for(int zz = 0; zz < Block::robot->NLineSensors ; zz++){
                      Block::robot->TurnLEDOn(255,255,255);
                      Block::robot->LineSensors[zz]->No_Line_Readout();
                      Block::robot->TurnLEDOn(0,0,0);
                      Block::BH->active_wait(100,5);
                      if(Block::robot->LineSensors[zz]->GetSensorPin() == LINE_PIN_1)Block::robot->Write_EEPROM_INT(EEPROM_L1_BORDER_ADDR,Block::robot->LineSensors[zz]->GetLogicBorder());
                      if(Block::robot->LineSensors[zz]->GetSensorPin() == LINE_PIN_2)Block::robot->Write_EEPROM_INT(EEPROM_L2_BORDER_ADDR,Block::robot->LineSensors[zz]->GetLogicBorder());
                      if(Block::robot->LineSensors[zz]->GetSensorPin() == LINE_PIN_3)Block::robot->Write_EEPROM_INT(EEPROM_L3_BORDER_ADDR,Block::robot->LineSensors[zz]->GetLogicBorder());
                    }
                }
            break;
             case 3 : 
                Block::robot->TurnLEDOn(184, 255, 3);
            break;
            default:

            break;
          }
           Block::BH->active_wait(10,10,interrupted,&action_with_no_interrupt);
        break;
    case 13:
        Block::robot->TurnLEDOff();
        break;
    case 14:
          robot->LED_Matrixes[SPI_PORT_2]->SetBitmap(0,font[used_blocks[0]->get_output()]);
          
          robot->LED_Matrixes[SPI_PORT_2]->Update();

          Block::BH->active_wait(10,5,interrupted,&action_with_no_interrupt);
        break;
        /*
    case 94:
        UserFunction_5(used_blocks[0]->get_output(),used_blocks[1]->get_output());
        break;
    case 95:
        used_blocks[1]->set_output(UserFunction_4());
        break;
    case 96:
        used_blocks[1]->set_output(UserFunction_3(used_blocks[0]->get_output()));
        break;
    case 97:
        UserFunction_2(used_blocks[0]->get_output());
        break;
    case 98:
        UserFunction_1();
        break; 
        */
    case 99:
        Block::robot->RawRotorMove(used_blocks[0]->get_output(),used_blocks[1]->get_output());
        Block::robot->Remote_block_used = true;
        break;
    case 101:
        used_blocks[1]->set_output(Block::robot->ReadDistSensor(used_blocks[0]->get_output()));
        break;
    case 102:
        used_blocks[1]->set_output(Block::robot->ReadLineSensor(used_blocks[0]->get_output()));
        break;
    case 255:
      //Saved for loops and ifs
      break;
    default :
        break;

  }
  if(interrupted && action_with_no_interrupt)interrupted = false;
}

bool Block::set_next(Block* blockList[],int blockList_N) {
          if(getNextID() != 0){
            for(int jj = 0 ; jj <  blockList_N ; jj++){
                if(getNextID() == blockList[jj]->getID()){
                  next = blockList[jj];
                  return(true);
                }
            }
            return(false);
          }else{
            return(true);
          }
}



bool Block::set_used_block(Block* blockList[],int blockList_N){
          byte usedBlockAttached = 0;
          used_blocks = new Block*[used_blocks_N];
          for(byte uu = 0;uu<used_blocks_N;uu++) used_blocks[uu] = NULL;
          if(used_blocks_N !=0){
              for(byte tt = 0; tt < used_blocks_N; tt++){
                Serial.print("Searching for block id:");
                Serial.println(used_blocksIDs[tt]);
                for(int jj = 0 ; jj <  blockList_N ; jj++){
                  if(used_blocksIDs[tt] == blockList[jj]->getID()){
                    Serial.println("Found!");
                    used_blocks[tt] = blockList[jj];
                    break;
                  }
              }
            }  
          }else{
            return(true);
          }
          if(usedBlockAttached == used_blocks_N){
            return(true);
          }else{
            return(false);
          }
}

Block* Block::get_next() {
  return(next);
}

int32_t Block::get_output() {
  return output;
}

String Block::get_string_output(){
  return("");
}

void Block::setRobot(Skribot *_robot){
  Block::robot = _robot;
}

void Block::setBlockHandler(BlockHandler *bh){
  Block::BH = bh;
}


void Block::set_output(int x){
  output = x;
}

bool Block::get_bool_output(){
  return(false);
}

