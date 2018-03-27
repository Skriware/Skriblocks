//#define DEBUG_MODE
#include <SkriBlocks.h>
BlockHandler BH;
SkriBot *robot;
bool runCode = true;
bool transmision_recieved = false;
bool BT_state;
char messagetmp[30];
int messagetmpIterator = 0;
char ascitmp;
char softVersion[] = "1.0.0";

void BTLOST(){
  BH.clear();
  robot->Stop();
  robot->TurnLEDOff(); 
  robot->OpenClaw();
  robot->Put_Down();
  runCode = false;
}

int freeRam() 
{
  #ifndef _VARIANT_BBC_MICROBIT_
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
  #else
  return(500);
  #endif
}


void setup() {
  #if ENABLED(DEBUG_MODE)
    Serial.begin(9600);
    Serial.println("DEBUG_MODE");
  #endif
  robot = new SkriBot("EDU_SHIELD");
  Block::setRobot(robot); 
  BT_state = false; 
  #if ENABLED(DEBUG_MODE)
    Serial.println(freeRam());
  #endif 
}

void loop() {
   if(!robot->BLE_checkConnection() && BT_state){
    BT_state = !BT_state;
    BTLOST();
    }

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
      break;
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

    }else if(BH.messageLength > 6
      && BH.AllMessage[BH.messageLength-4] == 'A' 
      && BH.AllMessage[BH.messageLength-3] == 'M' 
      && BH.AllMessage[BH.messageLength-2] == 'E' 
      && BH.AllMessage[BH.messageLength-1] == 'X'
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
       while(BH.doBlock()){
           while(robot->BLE_dataAvailable()){
           ascitmp = robot->BLE_read();
           messagetmp[messagetmpIterator] = ascitmp;
             #if ENABLED(DEBUG_MODE)
              Serial.print(messagetmp[messagetmpIterator]);
             #endif
              messagetmpIterator++;
            }
            if(    messagetmp[messagetmpIterator-4] == 'E' 
                && messagetmp[messagetmpIterator-3] == 'N' 
                && messagetmp[messagetmpIterator-2] == 'D'){
              #if ENABLED(DEBUG_MODE)
              Serial.println("Stopping the robot!");
              #endif
              BH.clear();
              robot->Stop();
              robot->Put_Down();
              robot->OpenClaw();
              robot->TurnLEDOn(255,255,255);
              break;
              
            }
            messagetmpIterator = 0;
            
              if(!robot->BLE_checkConnection()){
                BTLOST();
                break;
              }
        }
        BH.clear();
        robot->BLE_write("DONE\n");
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
      delay(10);
      robot->BLE_write("ack\n");
      transmision_recieved = false;
    }
  

  if(!robot->BLE_checkConnection() && BT_state){
     robot->TurnLEDOn(0,0,0); 
     robot->Stop();
     BT_state = !BT_state;
  }else if(robot->BLE_checkConnection() && !BT_state){
     robot->TurnLEDOn(255,255,255); 
     BT_state = !BT_state;
  }
}

