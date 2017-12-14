#include <SkriBlocks.h>
#define Too_Long_Code 3
#define   NUMPIXELS 3
//#define DEBUG_MODE
BlockHandler BH;
SkriBot *robot;
bool runCode = true;
bool transmision_recieved = false;

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
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  pinMode(EDU_BT_STATE_PIN,INPUT);
  #if ENABLED(DEBUG_MODE)
    Serial.begin(9600);
  #endif
   Serial3.begin(9600);
  Serial3.setTimeout(50);
  robot = new SkriBot("EDU_SHIELD");
  Block::setRobot(robot);
   
  robot->SetSpeed(250); 
  String messagetmp;
  char asci[30];
  char ascitmp;
  robot->Stop();

  while(true){
   if(digitalRead(3) == LOW)BTLOST();
    while(Serial3.available()){
    robot->TurnLEDOn(0,0,255);
    ascitmp = Serial3.read();
    #if ENABLED(DEBUG_MODE)
      Serial.print(ascitmp);
    #endif
    BH.AllMessage[BH.messageLength] = ascitmp;
    BH.messageLength++;
      robot->TurnLEDOn(0,0,0);
     if(BH.messageLength > 399)break;
    if(BH.messageLength > 4 
      && BH.AllMessage[BH.messageLength-5] == 'R' 
      && BH.AllMessage[BH.messageLength-4] == 'U' 
      && BH.AllMessage[BH.messageLength-3] == 'N' 
      ){
    BH.Mcursor = 6;
    Serial3.println("ack");
    int flag =0;
    while(freeRam() > 190){
      robot->TurnLEDOn(0,0,0);
      flag = BH.Handle_Msg(); 
      if(flag ==0){
       robot->TurnLEDOn(0,255,0);
       while(BH.doBlock(true)){
           if(Serial3.available()){
           while(Serial3.available()){
            messagetmp = Serial3.readString();
             #if ENABLED(DEBUG_MODE)
              Serial.print(messagetmp);
             #endif
            }
            if(messagetmp == "END\n"){
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
            }
              if(digitalRead(EDU_BT_STATE_PIN) == LOW)BTLOST();
        }
        BH.clear();
        break;
      }else if(flag == 1){
         robot->TurnLEDOn(255,255,255);
        while(BH.doBlock(false)){
           if(Serial3.available()){
           while(Serial3.available()){
            messagetmp = Serial3.readString();
             #if ENABLED(DEBUG_MODE)
              Serial.print(messagetmp);
             #endif
            }
            if(messagetmp == "END\n"){
              BH.clear();
              robot->Stop();
               robot->OpenClaw();
               robot->Put_Down();
              robot->TurnLEDOn(255,255,255);
              break;
            }
            }
              if(digitalRead(EDU_BT_STATE_PIN) == LOW)BTLOST();
        }
        BH.clear();
        runCode = true;
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
    
    robot->TurnLEDOn(0,0,0);
    transmision_recieved = true;
    
    }
    if(transmision_recieved == true){
      Serial3.println("ack");
       #if ENABLED (DEBUG_MODE)
          Serial.println("Sending Confirmation!");
      #endif
      transmision_recieved = false;
    }
    #if ENABLED(DEBUG_MODE)
        Serial.println("Ending Block!");
    #endif
  if(digitalRead(EDU_BT_STATE_PIN) == LOW){
     robot->TurnLEDOn(0,0,0); 
     robot->Stop();
  }else{
     robot->TurnLEDOn(255,255,255); 
  }


  }
  
}

void loop() {
  digitalWrite(Too_Long_Code,HIGH);
}

