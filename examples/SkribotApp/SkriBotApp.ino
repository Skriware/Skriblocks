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
     robot = new Skribot("SKRIBRAIN_B2C_TESTS");
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
}

void loop() {
   if(!robot->BLE_checkConnection() && BT_state){
    BT_state = !BT_state;
    ENTER_TO_IDLE();
    }
    
    byte tmp = BH.readMessageLine();
    
    BH.processLine(tmp);
   
    transmision_recieved = true;   

    if(transmision_recieved == true){
      robot->BLE_write("ack\n\r\n");
      transmision_recieved = false;
    }
  
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

void CompileCode(){
   int flag; 
    while(freeRam() > 190){
      flag = BH.Handle_Msg(); 
      if(flag != 2 && flag != 3) {
        Connection_Break = false;
        if(!robot->Remote_block_used)robot->BLE_write("ack\n\r\n");
        #ifdef DEBUG_MODE_1
        Serial.print("Starting program at: ");
        Serial.println(millis());
        #endif
        ExecuteCode();
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
        #if ENABLED(DEBUG_MODE_1)
          Serial.println("CONFIRMING END OF CODE");
        #endif
        break;
      }else if(flag == 3){
        BH.clear();
        #ifdef ESP_H        
          robot->status->TurnOn(RED,2);
        #endif
        robot->Stop();
        #if ENABLED(DEBUG_MODE_1)
          Serial.println("CODE NOT VALID");
        #endif
        robot->BLE_write("DONE\n");
        break;

      }
    }
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

