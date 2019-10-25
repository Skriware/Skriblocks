	#include "BlockHandler.h"


  void BlockHandler::AddToMessage(char x){
    AllMessage[messageLength] = x;
    messageLength++;
    #ifdef DEBUG_MODE
    Serial.print(x);
    #endif
  }

  void BlockHandler::CheckLongCodes(char *asciTmp){
    if(*asciTmp == 'B'){                                                  //Baptised case 
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              if(tmp2 == 'A'){
                *asciTmp = 'b';
                break; 
              }else if(tmp2 == 'E'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
              }
            }
          }
        }else if(*asciTmp == 'R'){
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              tmp2 = Block::robot->BLE_read();
              if(tmp2 == 'S'){
                *asciTmp = 'r';
                break; 
              }else if(tmp2 == 'N'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
              }
            }
          }
        }
  }

  bool BlockHandler::CheckForTimeout(){
            bool tmp = false;
            long last_message_time = millis();
            long last_ack_send = last_message_time;
            Block::robot->BLE_write("ack\n\r\n");
            while((Block::robot->BLE_dataAvailable() == 0)){
              if(millis() - last_message_time > MESSAGE_TIMEOUT){
                tmp = true;
                break;
              }else if(millis() - last_ack_send > ACK_RESEND_TIME){
                Block::robot->BLE_write("ack\n\r\n");
                last_ack_send = millis();
              }
            }
            return(tmp);
  }

  byte BlockHandler::readMessageLine(){
      char MainAsci,asciTmp;
      asciTmp = '0';
      if(Block::robot->BLE_dataAvailable()){
        MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
        CheckLongCodes(&MainAsci);
        if(MainAsci == INVALID_MSG_ERROR_CODE) return(INVALID_MSG_ERROR_CODE);
        while(asciTmp != '\n'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
          }else{
           if(CheckForTimeout())return(TIMEOUT_ERROR_CODE);
          }
        }
        return(MainAsci);
      }else{
        return(NO_MSG_CODE);
      }
  }
  byte BlockHandler::readCodeLine(){
    char MainAsci,asciTmp;
    asciTmp = '0';
    if(Block::robot->BLE_dataAvailable()){
      MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
      AddToMessage(MainAsci);
      if(MainAsci == 'R'){
        while(Block::robot->BLE_dataAvailable())AddToMessage(Block::robot->BLE_read());
        transfereBlocks = false;
        return(CODE_COMPLETE);
      }
      asciTmp = MainAsci;
    while(asciTmp != '\n'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
            AddToMessage(asciTmp);
          }else{
            if(CheckForTimeout()){
              transfereBlocks = false;  
              return(TIMEOUT_ERROR_CODE);
            }
          }
        }
    }else{
      Block::robot->BLE_write("ack\n\r\n");
      return(NO_MSG_CODE);
    }
    return(CODE_PASSED);
  }
  void BlockHandler::processMessageLine(byte LineCode){
        char tmp;
        char tmpNameArray[32] = {' '};
        int tmpCounter;
        switch(LineCode){
          case BAPTISED:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("Named");
                #endif
                  Block::robot->sendNameInfo();
                  clear();
          break;
          case RENAME:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("NewName");
                #endif
                tmp = Block::robot->BLE_read();
                tmpNameArray[32] = {' '};
                tmpCounter = 0;
                while(tmp != '\n'){
                  if(Block::robot->BLE_dataAvailable()){
                    tmpNameArray[tmpCounter] = tmp;
                    tmpCounter++;
                    tmp = Block::robot->BLE_read();
                  }
                }
                Block::robot->BLE_write("OK\n");
                #if ENABLED(DEBUG_MODE)
                  Serial.println(tmpNameArray);
                #endif  
                Block::robot->BLE_changeName(tmpNameArray);
                clear();
          break;
          case VERSION:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("Version Request");
                #endif
                Block::robot->BLE_write("1");
                clear();
          break;
          case RESET:
              #if ENABLED(DEBUG_MODE)
                Serial.println("RESET BLE");
              #endif
                Block::robot->BLE_write("OK\n");
                Block::robot->BLE_reset();
                clear();
          break;
          case END:
                clear();
                Block::robot->Stop();
                #ifndef _VARIANT_BBC_MICROBIT_
                Block::robot->OpenClaw();
                Block::robot->Put_Down();
                #endif
          break;
          case BEGIN:
                transfereBlocks = true;
          break;
          case NO_MSG_CODE:
          break;
          default:
                Block::robot->BLE_Flush();
                Block::robot->BLE_write("ack\n\r\n");
                clear();
          break;
        }
  } 
