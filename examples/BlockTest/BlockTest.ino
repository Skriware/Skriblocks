#include <SkriBlocks.h>
#include <Adafruit_NeoPixel.h>
#define ROTOR_SPEED_PIN_L 6
#define ROTOR_DIR_PIN_L 7
#define ROTOR_SPEED_PIN_R 5
#define ROTOR_DIR_PIN_R 4
#define ECHO_PIN_1 10
#define ECHO_PIN_2 12
#define TRIG_PIN_1 9
#define TRIG_PIN_2 11
#define Too_Long_Code 3
#define   NUMPIXELS 3
BlockHandler BH;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 13, NEO_GRB + NEO_KHZ800);
SkriBot *robot;
bool runCode = true;

void BTLOST(){
  BH.clear();
  robot->Stop();
  pixels.setPixelColor(0, pixels.Color(255,0,0)); 
  pixels.show();
  runCode = false;
  Serial.println("BT LOST!");
}
void MainLEDup(){

    pixels.setPixelColor(0, pixels.Color(255,255,255)); 
    pixels.show(); 
                                                        
}

void MainLEDdown(){
    pixels.setPixelColor(0, pixels.Color(0,0,0)); 
    pixels.show(); 
}

void SecondLEDup(){
    pixels.setPixelColor(2, pixels.Color(0,0,255)); 
    pixels.show();
}

void SecondLEDdown(){
    pixels.setPixelColor(2, pixels.Color(0,0,0)); 
    pixels.show();
}
int freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.setTimeout(50);
  robot = new SkriBot();
  Block::setRobot(robot);
  robot->AddDCRotor(ROTOR_SPEED_PIN_L,ROTOR_DIR_PIN_L,"Left");          //adding rotors for movement
  robot->AddDCRotor(ROTOR_SPEED_PIN_R,ROTOR_DIR_PIN_R,"Right");
  robot->AddDistSensor(ECHO_PIN_1,TRIG_PIN_1,1);   //adding Distance Sensors  and naming them "Left and Right";
  robot->AddDistSensor(ECHO_PIN_2,TRIG_PIN_2,0);
  robot->SetSpeed(250); 
  String messagetmp;
  char asci[30];
  char ascitmp;
  Serial.print("Free Ram Memory for code:"); 
  Serial.println(freeRam());
  pixels.begin();
  MainLEDup();
  pinMode(3,INPUT_PULLUP);
 

  //Serial.print("DistTest:");
  //Serial.println(robot->ReadDistSensor(1));
  
  while(true){
   // if(digitalRead(3) == LOW)BTLOST();
   if(Serial1.available()){
    SecondLEDup();
    MainLEDup();
    while(Serial1.available()){
    
    //ascitmp = Serial1.read();
    messagetmp = Serial1.readString();
    SecondLEDdown();
    //Serial.print(BH.messageLength);
    Serial.print(messagetmp);
    //Serial.print(ascitmp);
    messagetmp.toCharArray(asci, 50);
    int NewMessageLength = messagetmp.length();
    for(int j = BH.messageLength; j < BH.messageLength+NewMessageLength; j++){
    BH.AllMessage[j+1] = asci[j-BH.messageLength];
    }
      BH.messageLength += NewMessageLength;
    //BH.AllMessage[BH.messageLength] = ascitmp;
     //BH.messageLength++;
     if(BH.messageLength > 399)break;
    if(BH.messageLength > 4 
      && BH.AllMessage[BH.messageLength-4] == 'R' 
      && BH.AllMessage[BH.messageLength-3] == 'U' 
      && BH.AllMessage[BH.messageLength-2] == 'N' 
      ){
    BH.Mcursor = 7;
    Serial1.println("ack");
    int flag =0;
    while(freeRam() > 190){
      //SecondLEDdown();
      flag = BH.Handle_Msg(); 
      if(flag == 0){
        BH.startCode(false);
        //robot->Stop();
        BH.clear();
        
        break;
      }else if(flag == 1){
         pixels.setPixelColor(0, pixels.Color(0,255,0)); 
         pixels.show();
         SecondLEDdown();
        while(runCode){
          BH.startCode(false);
          Serial.println("Continuse mode loop");
           if(Serial1.available()){
           while(Serial1.available()){
            messagetmp = Serial1.readString();
            Serial.print(messagetmp);
            }
            if(messagetmp == "END\n"){
              Serial.println("Stopping the robot!");
              BH.clear();
              robot->Stop();
              MainLEDup();
              break;
            }
            }
              if(digitalRead(3) == LOW)BTLOST();
        }
        detachInterrupt(digitalPinToInterrupt(3));
        runCode = true;
        break;
    }
    }
    if(freeRam() < 190){
      while(true){
         pixels.setPixelColor(0, pixels.Color(255,0,0)); 
         pixels.show();
         delay(1000);
         pixels.setPixelColor(0, pixels.Color(0,0,0)); 
         pixels.show();
         delay(500);
      }
    }
    
    }else if(BH.AllMessage[0]== 'P' && BH.AllMessage[3] == 'S'){
      Serial.println("Pilot Mode!");
    }
    Serial1.println("ack");
    SecondLEDdown();
    
    
    }

  
 
}else{
  if(digitalRead(3) == LOW){
     pixels.setPixelColor(0, pixels.Color(0,0,0)); 
     pixels.show();
     robot->Stop();
  }else{
     pixels.setPixelColor(0, pixels.Color(255,255,255)); 
     pixels.show();
  }
}

  }
  
}

void loop() {
 /* for(int i = 0; i < 10; i++){
    Block tmp = Block(0,0,0,0);
    table[i] = tmp;
    Serial.println(i);
    delay(200);
  }

  */
  // put your main code here, to run repeatedly:
  digitalWrite(Too_Long_Code,HIGH);
}

