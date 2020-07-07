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
            //Block::robot->BLE_write("ack\n");
            while((Block::robot->BLE_dataAvailable() == 0)){
              if(millis() - last_message_time > MESSAGE_TIMEOUT){
                tmp = true;
                break;
              }else if(millis() - last_ack_send > ACK_RESEND_TIME){
                //Block::robot->BLE_write("ack\n");
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
        #ifdef DEBUG_MODE_1
        Serial.print(MainAsci);
        #endif
        if(MainAsci == INVALID_MSG_ERROR_CODE) return(INVALID_MSG_ERROR_CODE);
        while(asciTmp != '\n' && MainAsci != 'H' && MainAsci != 'C'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
            #ifdef DEBUG_MODE_1
            Serial.print(asciTmp);
            #endif
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
      CheckForTimeout();
      return(NO_MSG_CODE);
    }
    return(CODE_PASSED);
  }
  void BlockHandler::processMessageLine(byte LineCode){
        char tmp = 'A';
        char tmpNameArray[32] = {' '};
        int tmpCounter;
        char tmp_tag[10] = {' '};
        bool vaildcommand = true;
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
                sprintf(tmp_tag,"VERSION:%f",1.0);
                Block::robot->BLE_write(tmp_tag);
                clear();
          break;
          case RESET:
              #if ENABLED(DEBUG_MODE)
                Serial.println("RESET BLE");
              #endif
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
          case REMOTE:
          if(Block::robot->NLeftDCRotors ==0)Block::robot->AddDCRotor(SKRIBRAIN_MOTOR_L_DIR2_PIN,SKRIBRAIN_MOTOR_L_DIR1_PIN,"Left");
          if(Block::robot->NRightDCRotors ==0)Block::robot->AddDCRotor(SKRIBRAIN_MOTOR_R_DIR2_PIN,SKRIBRAIN_MOTOR_R_DIR1_PIN,"Right");
          Block::robot->RawRotorMove(readIntDirect(),readIntDirect());
          break;
          case BATTERY:
          sprintf(tmp_tag,"%d",Block::robot->ReadBattery());
          Block::robot->BLE_write(tmp_tag);
          break;
          case PIANO:
              if(Block::robot->Buzzers[SERVO_2] == NULL)Block::robot->AddBuzzer(SERVO_2);
              switch(readIntDirect()){
                      case 0:
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->StopNote();
                        break;
                      case 1:   
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C4");
                        break;
                      case 2:
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C#4");
                        break;
                      case 3: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("D4");
                        break;
                      case 4: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("D#4");
                        break;
                      case 5: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("E4");
                        break;
                      case 6: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("F4");
                        break;
                      case 7: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("F#4");
                        break;
                      case 8: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("G4");
                        break;
                      case 9: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("G#4");
                        break;
                      case 10: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("A4");
                        break;
                      case 11: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("A#4");
                        break;
                      case 12: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("B4");
                        break;
                      case 13: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C5");
                        break;
                      default:
                        break;
              }
          break;
          case HARDWARE_SET:
          Block::robot->ClearHardware();
          tmp = BLE_readwithTIMEOUT();
              while(tmp != '\n'){
                tmp_tag[0] = BLE_readwithTIMEOUT();
                tmp_tag[1] = BLE_readwithTIMEOUT();
                if(tmp_tag[0] == 'H')tmp_tag[0] = BLE_readwithTIMEOUT();
                if(tmp_tag[1] == 'H')tmp_tag[1] = BLE_readwithTIMEOUT();
                Block::robot->AddHardware(tmp_tag);
                tmp = BLE_readwithTIMEOUT();
              }
          Serial.println("HARDWARE SET");
          break;
          case CALIBRATE:
              tmp = BLE_readwithTIMEOUT();
              tmp_tag[0] = BLE_readwithTIMEOUT();
              tmp = BLE_readwithTIMEOUT();
              if(tmp_tag[0] == 'M'){
                Block::robot->left_scale= readIntDirect();
                Block::robot->right_scale= readIntDirect();
                Block::robot->left_invert= readIntDirect();                
                Block::robot->right_invert= readIntDirect();
                      #ifdef DEBUG_MODE
                        Serial.println("User Corrections:");
                        Serial.print("LS: ");
                        Serial.println(Block::robot->left_scale);
                        Serial.print("RS: ");
                        Serial.println(Block::robot->right_scale);
                        Serial.print("LI: ");
                        Serial.println(Block::robot->left_invert);
                        Serial.print("RI: ");
                        Serial.println(Block::robot->right_invert);
                      #endif
              Block::robot->Save_Calibration_Data(CALIB_MOTORS);
              }else if(tmp_tag[0] == 'W'){
                if(Block::robot->NLineSensors == 0){
                            Block::robot->AddLineSensor(LINE_PIN_1, 1);
                            Block::robot->AddLineSensor(LINE_PIN_2, 2);
                            Block::robot->AddLineSensor(LINE_PIN_3, 3);
                }
              Serial.println("Calibrating No Line!");
              Block::robot->Calibrate_sensors_no_Line();
              }else if(tmp_tag[0] == 'B'){
                 Serial.println("Calibrating Line!");
              Block::robot->Calibrate_sensors_Line();
              Block::robot->Save_Calibration_Data(CALIB_LINE_SENSORS);
              }
               Block::robot->BLE_Flush();
          break;
          default:
              Block::robot->BLE_Flush();
              clear();
              vaildcommand = false;
          break;
        }

        if(LineCode == TIMEOUT_ERROR_CODE){
          Serial.println("Timeout Error");
          Block::robot->BLE_write("ERROR:TIMEOUT_ERROR\n");
          return;
        }
        if(LineCode != NO_MSG_CODE && !transfereBlocks && LineCode != HARDWARE_SET){
        if(vaildcommand){
          sprintf(tmp_tag,"%cOK\n",LineCode);
          Block::robot->BLE_write(tmp_tag);
        }else{
          sprintf(tmp_tag,"ERROR:UNKNOWN_COMMAND:%c\n",LineCode);
          Block::robot->BLE_write(tmp_tag);
        }
      }
  } 

  char BlockHandler::BLE_readwithTIMEOUT(){
    char asciTmp;
    if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
          }else{
            if(CheckForTimeout()){
              return(TIMEOUT_ERROR_CODE);
            }else{
              asciTmp = Block::robot->BLE_read();
            }
          }
      return(asciTmp);
}

  int32_t BlockHandler::readIntDirect(){
  int nDigits = 0;
  int sign = 1;
  byte cursor = 0;
  char tmp;
  char msg[10];
  tmp = BLE_readwithTIMEOUT();
  while(tmp != ' ' && tmp != '\n'){
    msg[cursor+nDigits] = tmp;
    tmp = BLE_readwithTIMEOUT();
    nDigits++;
  }
  
  if(msg[cursor] == '-')sign = -1;
  if(sign==-1){
    cursor++;
    nDigits--;
  }
  int32_t out = 0;
  int32_t power = 1;
    for(int ii = nDigits-1; ii > -1; ii--){
      if(ii != nDigits-1) power *=10;
      int32_t add = cti(msg[cursor + ii])*power;
      out += add;
    }
    return(out*sign);
}



          