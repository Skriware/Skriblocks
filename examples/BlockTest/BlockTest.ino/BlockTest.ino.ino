#include <Skriblocks.h>
#include <Adafruit_NeoPixel.h>
#define ROTOR_SPEED_PIN_R 6
#define ROTOR_DIR_PIN_R 7
#define ROTOR_SPEED_PIN_L 5
#define ROTOR_DIR_PIN_L 4
#define ECHO_PIN_1 10
#define ECHO_PIN_2 12
#define TRIG_PIN_1 9
#define TRIG_PIN_2 11
#define LED_DATA_PIN 2
#define LED_DATA_PIN_1 0
#define BT_STATE_PIN 1
#define CLAW_PIN1 2
#define CLAW_PIN2 8

#define Too_Long_Code 3
#define   NUMPIXELS 3
BlockHandler BH;
SkriBot *robot;
bool runCode = true;
bool transmision_recieved = false;

void BTLOST(){
  BH.clear();
  robot->Stop();
  robot->TurnLEDOn(255,0,0); 

  runCode = false;
  ////Serial.println("BT LOST!");
  
}

int freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  pinMode(BT_STATE_PIN,INPUT);
  pinMode(A0,INPUT_PULLUP);
  //Serial.begin(9600);
   Serial3.begin(9600);
  Serial3.setTimeout(50);
  robot = new SkriBot();
  Block::setRobot(robot);
  robot->AddDCRotor(ROTOR_SPEED_PIN_L,ROTOR_DIR_PIN_L,"Left");          //adding rotors for movement
  robot->AddDCRotor(ROTOR_SPEED_PIN_R,ROTOR_DIR_PIN_R,"Right");
  robot->AddDistSensor(ECHO_PIN_1,TRIG_PIN_1,1);   //adding Distance Sensors  and naming them "Left and Right";
  robot->AddDistSensor(ECHO_PIN_2,TRIG_PIN_2,0);
  robot->AddLED(LED_DATA_PIN_1,1);
  robot->AddLED(LED_DATA_PIN,0);
  robot->AddClaw(CLAW_PIN1,CLAW_PIN2);
  
  
  
  robot->SetSpeed(250); 
  String messagetmp;
  char asci[30];
  char ascitmp;
  if(digitalRead(A0) == LOW){
    robot->MoveForward();
    while(true){}
  }
  robot->Stop();

  while(true){
   if(digitalRead(3) == LOW)BTLOST();
    while(Serial3.available()){
    robot->TurnLEDOn(0,0,255);
    ascitmp = Serial3.read();
    //Serial.print(ascitmp);
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
            //Serial.print(messagetmp);
            }
            if(messagetmp == "END\n"){
              ////Serial.println("Stopping the robot!");
              BH.clear();
              robot->Stop();
              robot->TurnLEDOn(255,255,255);
              break;
            }
            }
              if(digitalRead(BT_STATE_PIN) == LOW)BTLOST();
        }
        BH.clear();
        break;
      }else if(flag == 1){
         robot->TurnLEDOn(255,255,255);
        while(BH.doBlock(false)){
           if(Serial3.available()){
           while(Serial3.available()){
            messagetmp = Serial3.readString();
            ////Serial.print(messagetmp);
            }
            if(messagetmp == "END\n"){
              ////Serial.println("Stopping the robot!");
              BH.clear();
              robot->Stop();
              robot->TurnLEDOn(255,255,255);
              break;
            }
            }
              if(digitalRead(BT_STATE_PIN) == LOW)BTLOST();
        }
        BH.clear();
        //detachInterrupt(digitalPinToInterrupt(3));
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
      ////Serial.println("Sending Cnofirmation!");
      transmision_recieved = false;
    }
    //////Serial.println("Ending Block!");
  if(digitalRead(BT_STATE_PIN) == LOW){
     robot->TurnLEDOn(0,0,0); 
     robot->Stop();
     ////Serial.println("NO BT!");
  }else{
     robot->TurnLEDOn(255,255,255); 
  }


  }
  
}

void loop() {
 /* for(int i = 0; i < 10; i++){
    Block tmp = Block(0,0,0,0);
    table[i] = tmp;
    ////Serial.println(i);
    delay(200);
  }

  */
  // put your main code here, to run repeatedly:
  digitalWrite(Too_Long_Code,HIGH);
}

