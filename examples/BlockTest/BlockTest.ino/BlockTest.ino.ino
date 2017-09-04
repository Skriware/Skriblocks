#include <SkriBlocks.h>
#define ROTOR_SPEED_PIN_L 6
#define ROTOR_DIR_PIN_L 7
#define ROTOR_SPEED_PIN_R 5
#define ROTOR_DIR_PIN_R 4
#define ECHO_PIN_1 10
#define ECHO_PIN_2 12
#define TRIG_PIN_1 9
#define TRIG_PIN_2 11
BlockHandler BH;
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
  SkriBot *robot = new SkriBot();
  Block::setRobot(robot);
  robot->AddDCRotor(ROTOR_SPEED_PIN_L,ROTOR_DIR_PIN_L,"Left");          //adding rotors for movement
  robot->AddDCRotor(ROTOR_SPEED_PIN_R,ROTOR_DIR_PIN_R,"Right");
  robot->AddDistSensor(ECHO_PIN_1,TRIG_PIN_1,1);   //adding Distance Sensors  and naming them "Left and Right";
  robot->AddDistSensor(ECHO_PIN_2,TRIG_PIN_2,0);
  robot->SetSpeed(200); 
  String messagetmp;
  char asci[30];
  Serial.print("Free Ram Memory for code:"); 
  Serial.println(freeRam());
  //Serial.print("DistTest:");
  //Serial.println(robot->ReadDistSensor(1));
  
  while(true){
   if(Serial1.available()){
    while(Serial1.available()){
    messagetmp = Serial1.readString();
    Serial.print(messagetmp);
    messagetmp.toCharArray(asci, 50);
    int NewMessageLength = messagetmp.length();
    for(int j = BH.messageLength; j < BH.messageLength+NewMessageLength; j++){
      BH.AllMessage[j+1] = asci[j-BH.messageLength];
    }
     BH.messageLength+=NewMessageLength;
    if(BH.AllMessage[BH.messageLength-4] == 'R' && BH.AllMessage[BH.messageLength-3] == 'U'){
    BH.Mcursor = 7;
    Serial1.println("ack");
    int flag =0;
    while(freeRam() > 190){
      flag = BH.Handle_Msg(); 
      if(flag == 0){
        BH.startCode(false);
        //robot->Stop();
        BH.clear();
        break;
      }else if(flag == 1){
        while(true){
          BH.startCode(false);
          Serial.println("Continuse mode loop");
           if(Serial1.available()){
           while(Serial1.available()){
            messagetmp = Serial1.readString();
            Serial.print(messagetmp);
            }
            if(messagetmp == "END\n"){
              BH.clear();
              robot->Stop();
              break;
            }
            }
        }
        break;
    }
    }
    
    }else if(BH.AllMessage[0]== 'P' && BH.AllMessage[3] == 'S'){
      Serial.println("Pilot Mode!");
    }
    Serial1.println("ack");
     
    
    
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
  Serial.print("jnjn");
  delay(30000);
}

