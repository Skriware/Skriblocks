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

void BTLOST(){
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
  #ifdef ESP_H
     robot = new Skribot("SKRIBRAIN");
  #else
    robot = new Skribot("EDU_SHIELD");
  #endif
  robot->BLE_Setup();
  Block::setRobot(robot); 
  BT_state = false; 
  #if ENABLED(DEBUG_MODE)
    Serial.println(freeRam());
  #endif 
    UserFunctions_Setup();
    BTLOST();
    robot->status->BLINK_OK();
    #ifdef ESP_H
      robot->status->TurnOn(YELLOW,2);
    #endif
}

void loop() {
   if(!robot->BLE_checkConnection() && BT_state){
    BT_state = !BT_state;
    BTLOST();
    }
    robot->BaterryCheck();
    if(robot->BLE_dataAvailable()){
      while(robot->BLE_dataAvailable()){
        ascitmp = robot->BLE_read();
        #if ENABLED(DEBUG_MODE)
        Serial.print(ascitmp);
        #endif
        BH.AllMessage[BH.messageLength] = ascitmp;
        BH.messageLength++;
        if(BH.messageLength > 1600){
          #if ENABLED(DEBUG_MODE)
            Serial.println("Too Long Code");
          #endif 
          break;
        }
        //#ifdef ESP_H
          if(BH.messageLength > 4
                  && BH.AllMessage[BH.messageLength-4] == 'A' 
                  && BH.AllMessage[BH.messageLength-3] == 'M' 
                  && BH.AllMessage[BH.messageLength-2] == 'E' 
                  && BH.AllMessage[BH.messageLength-1] == '\n'
                ){
                  #if ENABLED(DEBUG_MODE)
                  Serial.println("Setting new name");
                  #endif
                  char tmp = ' ';
                  char tmpNameArray[32] = {' '};
                  int tmpCounter = 0;
                while(true){
                  if(robot->BLE_dataAvailable()){
                    tmp = robot->BLE_read();
                    if(tmp == '\n')break;
                    tmpNameArray[tmpCounter] = tmp;
                    tmpCounter++; 
                  }
                }
                  robot->BLE_write("OK\n");
                  #if ENABLED(DEBUG_MODE)
                  Serial.println(tmpNameArray);
                  #endif  
                  robot->BLE_changeName(tmpNameArray);
                  BH.clear();
                  break;
                }
       // #endif
      }
    if(BH.messageLength > 3
      && BH.AllMessage[BH.messageLength-4] == 'E' 
      && BH.AllMessage[BH.messageLength-3] == 'N' 
      && BH.AllMessage[BH.messageLength-2] == 'D'
      ){
      BH.clear();
      robot->Stop();
      #ifndef _VARIANT_BBC_MICROBIT_
        robot->OpenClaw();
        robot->Put_Down();
      #endif
      //break;
    }
    if(BH.messageLength > 6 
      && BH.AllMessage[BH.messageLength-7] == 'V' 
      && BH.AllMessage[BH.messageLength-6] == 'E' 
      && BH.AllMessage[BH.messageLength-5] == 'R' 
      ){
      #if ENABLED(DEBUG_MODE)
      Serial.println("Version");
      #endif
        robot->BLE_write(softVersion);
        BH.clear();
    }else if
      (BH.messageLength > 6
      && BH.AllMessage[BH.messageLength-4] == 'A' 
      && BH.AllMessage[BH.messageLength-3] == 'M' 
      && BH.AllMessage[BH.messageLength-2] == 'E' 
      && BH.AllMessage[BH.messageLength-1] == '\n'
    ){
      #if ENABLED(DEBUG_MODE)
      Serial.println("NewName");
      #endif
      char tmp = robot->BLE_read();
      char tmpNameArray[32] = {' '};
      int tmpCounter = 0;
    while(tmp != '\n'){
      if(robot->BLE_dataAvailable()){
        tmpNameArray[tmpCounter] = tmp;
        tmpCounter++;
        tmp = robot->BLE_read();
      }
    }
      robot->BLE_write("OK\n");
      #if ENABLED(DEBUG_MODE)
      Serial.println(tmpNameArray);
      #endif  
      robot->BLE_changeName(tmpNameArray);
      BH.clear();

    }else if (BH.messageLength > 6 
      && BH.AllMessage[BH.messageLength-3]  == 'Z' 
      && BH.AllMessage[BH.messageLength-2]  == 'E' 
      && BH.AllMessage[BH.messageLength-1]  == 'D'){
      #if ENABLED(DEBUG_MODE)
      Serial.println("Named");
      #endif
      robot->sendNameInfo();
      BH.clear();
    }else if (BH.messageLength > 4 
      && BH.AllMessage[BH.messageLength-3]  == 'E' 
      && BH.AllMessage[BH.messageLength-2]  == 'T' 
      && BH.AllMessage[BH.messageLength-1]  == 'X'){
      #if ENABLED(DEBUG_MODE)
      Serial.println("RESET BLE");
      #endif
      robot->BLE_write("OK\n");
      robot->BLE_reset();
      BH.clear();

    }else if(BH.messageLength > 4 
      && BH.AllMessage[BH.messageLength-5] == 'R' 
      && BH.AllMessage[BH.messageLength-4] == 'U' 
      && BH.AllMessage[BH.messageLength-3] == 'N' 
      ){
    BH.Mcursor = 6;
    robot->BLE_write("ack\n");
    int flag; 
    while(freeRam() > 190){
      flag = BH.Handle_Msg(); 
      if(flag != 2) {
        Connection_Break = false;
       #ifdef ESP_H
        robot->status->TurnOn(GREEN,2);
       #endif
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
                BTLOST();
                Connection_Break = true;
                break;
            }
        }
        BH.clear();
        robot->TurnLEDOn(255,255,255);
       
        if(!Connection_Break){
          robot->ProgramENDRepotred();
          robot->wait_And_Check_BLE_Connection(500,50);
          robot->BLE_write("DONE\n");
           #ifdef ESP_H
          robot->status->TurnOn(BLUE,2);
           #endif

        }
        #if ENABLED(DEBUG_MODE)
          Serial.println("CONFIRMING END OF CODE");
        #endif
        break;
      }
    }
    if(freeRam() < 190){
      while(true){
         robot->TurnLEDOn(255,0,0); 
         delay(1000);
         robot->TurnLEDOff();        
         delay(500);
      }
    }
    
    }
    transmision_recieved = true;   
  }
    if(transmision_recieved == true){
      robot->BLE_write("ack\n");
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
     #ifdef ESP_H
      robot->status->TurnOn(BLUE,2);
     #endif
  }
}

