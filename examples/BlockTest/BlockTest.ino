                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                #include <SkriBlocks.h>
#include <Adafruit_NeoPixel.h>
#define ROTOR_SPEED_PIN_R 6
#define ROTOR_DIR_PIN_R 7
#define ROTOR_SPEED_PIN_L 5
#define ROTOR_DIR_PIN_L 4
#define ECHO_PIN_1 10
#define ECHO_PIN_2 12
#define TRIG_PIN_1 9
#define TRIG_PIN_2 11
#define LED_DATA_PIN 13
#define BT_STATE_PIN 3

#define Too_Long_Code 3
#define   NUMPIXELS 3
BlockHandler BH;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);
SkriBot *robot;
bool runCode = true;
bool transmision_recieved = false;

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
  pinMode(BT_STATE_PIN,INPUT);
  pinMode(A0,INPUT_PULLUP);
  Serial.begin(9600);
   Serial2.begin(9600);
  //Serial2.setTimeout(50);
  robot = new SkriBot();
  Block::setRobot(robot);
  robot->AddDCRotor(ROTOR_SPEED_PIN_L,ROTOR_DIR_PIN_L,"Left");          //adding rotors for movement
  robot->AddDCRotor(ROTOR_SPEED_PIN_R,ROTOR_DIR_PIN_R,"Right");
  robot->AddDistSensor(ECHO_PIN_1,TRIG_PIN_1,1);   //adding Distance Sensors  and naming them "Left and Right";
  robot->AddDistSensor(ECHO_PIN_2,TRIG_PIN_2,0);
  robot->AddLineSensor(1,0);
  robot->SetSpeed(250); 
  String messagetmp;
  char asci[30];
  char ascitmp;
  Serial.print("Free Ram Memory for code:"); 
  Serial.println(freeRam());
  pixels.begin();
  MainLEDup();
  
  if(digitalRead(A0) == LOW){
    robot->MoveForward();
    while(true){}
  }
  robot->Stop();
  pixels.setPixelColor(0, pixels.Color(255,0,0)); 
         pixels.show();
         delay(1000);
         pixels.setPixelColor(0, pixels.Color(0,0,0)); 
         pixels.show();
         delay(500);
  
  while(true){
   // if(digitalRead(3) == LOW)BTLOST();
    while(Serial2.available()){
    ascitmp = Serial2.read();
    SecondLEDup();
    MainLEDup();
    //messagetmp = Serial2.readString();
    SecondLEDdown();
    //Serial.print(BH.messageLength);
    //Serial.print(":");
    Serial.print(ascitmp);
    //messagetmp.toCharArray(asci, 50);
    //int NewMessageLength = messagetmp.length();
    //for(int j = BH.messageLength; j < BH.messageLength+NewMessageLength; j++){
    //BH.AllMessage[j+1] = asci[j-BH.messageLength];
    //}
      //BH.messageLength += NewMessageLength;
    BH.AllMessage[BH.messageLength] = ascitmp;
     BH.messageLength++;
     if(BH.messageLength > 399)break;
    if(BH.messageLength > 4 
      && BH.AllMessage[BH.messageLength-5] == 'R' 
      && BH.AllMessage[BH.messageLength-4] == 'U' 
      && BH.AllMessage[BH.messageLength-3] == 'N' 
      ){
    BH.Mcursor = 6;
    Serial2.println("ack");
    int flag =0;
    while(freeRam() > 190){
      //SecondLEDdown();
      flag = BH.Handle_Msg(); 
      if(flag ==0){
        pixels.setPixelColor(0, pixels.Color(0,255,0)); 
        pixels.show();
       while(BH.doBlock(true)){
           if(Serial2.available()){
           while(Serial2.available()){
            messagetmp = Serial2.readString();
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
              if(digitalRead(BT_STATE_PIN) == LOW)BTLOST();
        }
        BH.clear();
        break;
      }else if(flag == 1){
         SecondLEDdown();
        while(BH.doBlock(false)){
           if(Serial2.available()){
           while(Serial2.available()){
            messagetmp = Serial2.readString();
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
         pixels.setPixelColor(0, pixels.Color(255,0,0)); 
         pixels.show();
         delay(1000);
         pixels.setPixelColor(0, pixels.Color(0,0,0)); 
         pixels.show();
         delay(500);
      }
    }
    
    }
    
    SecondLEDdown();
    transmision_recieved = true;
    
    }
    if(transmision_recieved == true){
      Serial2.println("ack");
      Serial.println("Sending Cnofirmation!");
      transmision_recieved = false;
    }
    //Serial.println("Ending Block!");
  if(digitalRead(BT_STATE_PIN) == LOW){
     pixels.setPixelColor(0, pixels.Color(0,0,0)); 
     pixels.show();
     robot->Stop();
     Serial.println("NO BT!");
  }else{
     pixels.setPixelColor(0, pixels.Color(255,255,255)); 
     pixels.show();
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

