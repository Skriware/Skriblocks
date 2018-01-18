#include <SkriBlocks.h>
#define Too_Long_Code 3
#define   NUMPIXELS 3
//#define DEBUG_MODE
BlockHandler BH;
SkriBot *robot;
bool runCode = true;
bool transmision_recieved = false;
<<<<<<< HEAD
bool BT_state;
=======
<<<<<<< HEAD
bool BT_state;
=======
>>>>>>> 876baa1f380589233194e795f5a46d0941599938
>>>>>>> 0cf487d7b31ff738fe724041f3c126c51155b874
String messagetmp;
char asci[30];
char ascitmp;

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
  robot = new SkriBot("EDU_SHIELD");
  pinMode(EDU_BT_STATE_PIN,INPUT);
  #if ENABLED(DEBUG_MODE)
    Serial.begin(9600);
  #endif
  Serial3.begin(9600);
  Serial3.setTimeout(50);
<<<<<<< HEAD
  Block::setRobot(robot); 
  BT_state = false; 
}

void loop() {
   if(digitalRead(3) == LOW && BT_state){
    BT_state = !BT_state;
    BTLOST();
    }
=======
<<<<<<< HEAD
  Block::setRobot(robot); 
  BT_state = false; 
}

void loop() {
   if(digitalRead(3) == LOW && BT_state){
    BT_state = !BT_state;
    BTLOST();
    }
=======
  Block::setRobot(robot);  
}

void loop() {
   if(digitalRead(3) == LOW)BTLOST();
>>>>>>> 876baa1f380589233194e795f5a46d0941599938
>>>>>>> 0cf487d7b31ff738fe724041f3c126c51155b874
    while(Serial3.available()){
    ascitmp = Serial3.read();
    #if ENABLED(DEBUG_MODE)
      Serial.print(ascitmp);
    #endif
    BH.AllMessage[BH.messageLength] = ascitmp;
    BH.messageLength++;
<<<<<<< HEAD
      
=======
<<<<<<< HEAD
      
=======
      robot->TurnLEDOn(0,0,0);
<<<<<<< HEAD
>>>>>>> 876baa1f380589233194e795f5a46d0941599938
>>>>>>> 0cf487d7b31ff738fe724041f3c126c51155b874
     if(BH.messageLength > 800)break;
    if(BH.messageLength > 3
      && BH.AllMessage[BH.messageLength-4] == 'E' 
      && BH.AllMessage[BH.messageLength-3] == 'N' 
      && BH.AllMessage[BH.messageLength-2] == 'D'
=======
     if(BH.messageLength > 399)break;
    if(BH.messageLength > 4
      && BH.AllMessage[BH.messageLength-5] == 'E' 
      && BH.AllMessage[BH.messageLength-4] == 'N' 
      && BH.AllMessage[BH.messageLength-3] == 'D'
>>>>>>> parent of d076a91... Stoping on END command
      ){
      BH.clear();
      robot->Stop();
      robot->OpenClaw();
      robot->Put_Down();
      break;
    }
    if(BH.messageLength > 4 
      && BH.AllMessage[BH.messageLength-5] == 'R' 
      && BH.AllMessage[BH.messageLength-4] == 'U' 
      && BH.AllMessage[BH.messageLength-3] == 'N' 
      ){
    BH.Mcursor = 6;
    Serial3.println("ack");
    int flag; 
    while(freeRam() > 190){
      //robot->TurnLEDOn(0,0,0);
      flag = BH.Handle_Msg(); 
      if(flag == 0){
<<<<<<< HEAD
       //robot->TurnLEDOn(0,255,0);
=======
<<<<<<< HEAD
       //robot->TurnLEDOn(0,255,0);
=======
       robot->TurnLEDOn(0,255,0);
>>>>>>> 876baa1f380589233194e795f5a46d0941599938
>>>>>>> 0cf487d7b31ff738fe724041f3c126c51155b874
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
              if(digitalRead(EDU_BT_STATE_PIN) == LOW){
                BTLOST();
                break;
              }
        }
        BH.clear();
        Serial3.println("DONE");
        break;
      }else if(flag == 1){
         //robot->TurnLEDOn(255,255,255);
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
              if(digitalRead(EDU_BT_STATE_PIN) == LOW){
                BTLOST();
                break;
              }
        }
        BH.clear();
        runCode = true;
        Serial3.println("DONE");
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

<<<<<<< HEAD
    //robot->TurnLEDOn(0,0,0);
=======
<<<<<<< HEAD
    //robot->TurnLEDOn(0,0,0);
=======
    robot->TurnLEDOn(0,0,0);
>>>>>>> 876baa1f380589233194e795f5a46d0941599938
>>>>>>> 0cf487d7b31ff738fe724041f3c126c51155b874
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
  if(digitalRead(EDU_BT_STATE_PIN) == LOW && BT_state){
     robot->TurnLEDOn(0,0,0); 
     robot->Stop();
     BT_state = !BT_state;
  }else if(digitalRead(EDU_BT_STATE_PIN) == HIGH && !BT_state){
     robot->TurnLEDOn(255,255,255); 
     BT_state = !BT_state;
  }

}

