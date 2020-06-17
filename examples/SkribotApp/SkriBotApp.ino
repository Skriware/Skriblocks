//#define DEBUG_MODE
#include <Skribot_mobile.h>
BlockHandler BH;
Skribot *robot;
bool transmision_recieved = false;
bool BT_state;
bool Connection_Break = false;
char ascitmp;
char softVersion[] = "1.0.0";
long last_BT_check = 0;
bool LED_STATE = true;

void ENTER_TO_IDLE(){
  BH.clear();
  robot->Stop();
  robot->TurnLEDOff(); 
  robot->OpenClaw();
  robot->Put_Down();
  robot->TurnLEDOn(255,255,255);

  for (int i = 0; i < 5; i++)
  {
    auto *buzzer = robot->Buzzers[i];
    if (buzzer != nullptr)
      buzzer->StopNote();
  }

}

void Blink(){
  if (millis()-last_BT_check > 500){
    if(LED_STATE){
      robot->TurnLEDOn(255,255,255);
    }else{
      robot->TurnLEDOff();
    }
      last_BT_check = millis();
      LED_STATE = !LED_STATE;
  }
}

int freeRam() 
{
  #ifndef ESP_H
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
  #else
  return(500);
  #endif
}

void setup() {
  #if ENABLED(DEBUG_MODE)
    Serial.begin(115200);
    Serial.println("DEBUG_MODE");
  #endif
  #if ENABLED(DEBUG_MODE_1)
    Serial.begin(115200);
    Serial.println("DEBUG_MODE");
  #endif
  #ifdef ESP_H
     robot = new Skribot("RAW_SKRIBRAIN");
  #else
    robot = new Skribot("EDU_SHIELD");
  #endif
  robot->ConfigureBoardEEPROM();
  robot->BLE_Setup();
  Block::setRobot(robot); 
  Block::setBlockHandler(&BH);
  BT_state = false; 
  #if ENABLED(DEBUG_MODE)
    Serial.println(freeRam());
  #endif 
    UserFunctions_Setup();
    ENTER_TO_IDLE();
    robot->status->BLINK_OK();
    #ifdef ESP_H
      robot->status->TurnOn(YELLOW,2);
    #endif
    Block::robot->ClearHardware();
}

void loop() {
   if(!robot->BLE_checkConnection() && BT_state){
    BT_state = !BT_state;
    ENTER_TO_IDLE();
    }
    robot->BaterryCheck();
    byte tmp = BH.readMessageLine();
    BH.processMessageLine(tmp);
    
    if(BH.transfereBlocks){
      byte codeinfo;
      while(true){                  //read code
        codeinfo = BH.readCodeLine();
        if(codeinfo == TIMEOUT_ERROR_CODE)break;
        if(codeinfo == CODE_COMPLETE)break;
      }
      if(codeinfo == CODE_COMPLETE){
        Serial.println("BEGIN COMPILATION!");
        byte succes = CompileCode();        //make blocks connections
        Serial.println("COMPILATION ENDED");
        if(succes == 1){                    //chceck compiler errors
            Connection_Break = false;
            if(!robot->Remote_block_used)robot->BLE_write("ack\n\r\n");
            ExecuteCode();                  //Here robot runs the code
            SendCodeEndMEssage();

        }else if(succes ==3){
        SendErrorMsg("CODE NOT VALID");
      }

      }else if(codeinfo == TIMEOUT_ERROR_CODE){
        SendErrorMsg("TIMEOUT ERROR");
      }  

  }
  idle_connectioncheck();
}

int CompileCode(){
     int flag; 
  while(freeRam() > 190){
      flag = BH.Handle_Msg(); 
      if(flag != 2)return(flag);
    }
}

void idle_connectioncheck(){
   if(!robot->BLE_checkConnection()){
     Blink();
     if(BT_state){
      robot->Stop();
      BT_state = !BT_state;
      #ifdef ESP_H
      robot->status->TurnOn(YELLOW,2);
      #endif
     }
  }else if(robot->BLE_checkConnection() && !BT_state){ 
     robot->TurnLEDOn(255,255,255); 
     BT_state = !BT_state;
     BH.clear();
     #ifdef ESP_H
      robot->status->TurnOn(BLUE,2);
     #endif
  }
} 

void SendCodeEndMEssage(){
        BH.clear();
        robot->TurnLEDOn(255,255,255);
        if(!Connection_Break){
          robot->ProgramENDRepotred();
          if(!robot->Remote_block_used){
            robot->wait_And_Check_BLE_Connection(500,20);
            robot->BLE_write("DONE\n");
          }else{
            robot->Remote_block_used = false;
          }
        }
        robot->connection_Break_Reported = false;
        #if ENABLED(DEBUG_MODE_1)
          Serial.println("CONFIRMING END OF CODE");
        #endif
        ENTER_TO_IDLE();
}
void ExecuteCode(){
while(BH.doBlock()){
           robot->BaterryCheck();
           if(robot->ProgramENDRepotred()){
              #if ENABLED(DEBUG_MODE)
              Serial.println("Stopping the robot!");
              #endif
                BH.clear();
                robot->Stop();
                robot->Put_Down();
                robot->OpenClaw();
                robot->TurnLEDOn(255,255,255);
              break;  
            }else if(!robot->BLE_checkConnection()){
                ENTER_TO_IDLE();
                Connection_Break = true;
                break;
            }
        }
}

void SendErrorMsg(char *msg){
            robot->BLE_write("DONE\n");
            ENTER_TO_IDLE();
            #ifdef ESP_H        
            robot->status->TurnOn(RED,2);
            #endif
            #if ENABLED(DEBUG_MODE_1)
            Serial.println(msg);
            #endif
}
