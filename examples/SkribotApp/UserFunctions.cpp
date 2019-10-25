#include "Skribot_mobile.h"

                                  //put your global variables here if you want to pass some information from one instance of your function to another

void UserFunctions_Setup(){
	
	//Put all setup code here - it will be run onece at the beggining of the program.
}

void UserFunction_1(){
	
//Write your own Arduino based Function here and trigger it in app using block UserFunction_1!
}

void UserFunction_2(int input){       //Example line following Algorithm with one check time input

}

int UserFunction_3(int input){
//Write your own Arduino based Function here and trigger it in app using block UserFunction_3!
/*
   int LeftSensor = Block::robot->ReadDistSensor("D1");                  //Read distance to neares obstacle from both sensors distance. Obtained distance is in cm.
   int RightSensor = Block::robot->ReadDistSensor("D2");       
    if(LeftSensor > RightSensor){                             //checking previous readout
        if(RightSensor > lastRightSensor){        
          Block::robot->FaceRight(input);                               // Turning Right  
        }else{
          Block::robot->FaceLeft(input*2);                                 // We turned Left and now we are turning Right again?! We are probably in the corner lets escape!
        } 
    }
    if(RightSensor > LeftSensor){
      if(LeftSensor > lastLeftSensor ){
        Block::robot->FaceLeft(input);                                 // Turning Left
      }
      else{
        Block::robot->FaceRight(input*2);                                 // We turned Right and now we are turning Left again?! We are probably in the corner lets escape!
      }
    }
    else{
      Block::robot->MoveForward();                                   //Move Forward if there are no obstacles in th way.                                 
    }
    lastRightSensor = RightSensor;
    lastLeftSensor = LeftSensor;                             //Remembering last readouts
*/
return(0);


}

void UserFunction_4(int input_1, int input_2){
  
  //Write your own Arduino based Function here and trigger it in app using block UserFunction_5!
  
}

int UserFunction_5(){
//Write your own Arduino based Function here and trigger it in app using block UserFunction_4!

return(0);
}

