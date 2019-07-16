#include "InterruptHandler.h"

	InterruptHandler::InterruptHandler(byte int_type,byte int_input,byte trigger,byte _priority,byte _starting_block_id){
	interrupt_type = int_type;
	starting_block_id = _starting_block_id;
	priority = _priority;
	trigger_type = trigger;
	input = int_input;
	}
	bool InterruptHandler::Check_for_interrupt(){
		if(!Condition_saniti_check())return(false);
		byte distance;
		bool line_trig;
		switch(interrupt_type){
			case BUTTON_INTERRUPT:
			
				if(trigger_type == BUTTON_HIGH){
					return(digitalRead(input) == HIGH);
				}
 			break;
 			case DISTANCE_INTERRUPT:
 				if(input == DIST_D1){
 					distance = robot->ReadDistSensor(1);
 				}else if(input == DIST_D2){
 					distance = robot->ReadDistSensor(2);
 				}else{
 					return(false);
 				}

 				switch(trigger_type){
 					case DISTANCE_GRATER_THEN:
 						return(distance > trig_distance);
 					break;
 					case DISTANCE_LESS_THEN:
 						return(distance < trig_distance);
 					break;
 					default:
 						return(false);
 					break;
 				}

 			break; 
 			case LINE_INTERRUPT:
 				if(trigger_type == LINE_DETECTED){
 					line_trig = true;
 				}else if(trigger_type == NO_LINE_DETECTED){
 					line_trig = false;
 				}else{
 					return(false);
 				}
 				return(robot->ReadLineSensor(input - 1) == line_trig);
 			break; 
 			case TIME_INTERRUPT :
 				if(millis()-last_interrupt_time > time_period){
 					last_interrupt_time = millis();
 					return(true);
 				}else{
 					return(false);
 				}
 			break;
 			default:
 				return(false);
 			break;

		}
	}
	bool InterruptHandler::Condition_saniti_check(){
		if(input == BUTTON_INTERRUPT){
			if(input == BUTTON_1 || input == BUTTON_2 || input == BUTTON_3){
				if(trigger_type == BUTTON_HIGH || trigger_type == BUTTON_LOW){
					return(true);
				}
			}
		}else if(input == DISTANCE_INTERRUPT){
			if(input == DIST_D1 || input == DIST_D2){
				if(trigger_type == DISTANCE_GRATER_THEN || trigger_type == DISTANCE_LESS_THEN){
					return(true);
				}
			}
		}else if(input == LINE_INTERRUPT){
			if(input == LINE_L1 || input == LINE_L2 || input == LINE_L3){
				if(trigger_type == NO_LINE_DETECTED || trigger_type == LINE_DETECTED){
					return(true);
				}
			}
		}else if(input == TIME_INTERRUPT){
			if(input == 0 && trigger_type == 0){
				return(true);
			}
		}
		return(false);
	}
	void InterruptHandler::Set_Trig_Distance(byte dist){
		trig_distance = dist;
	}
	void InterruptHandler::Set_Trig_Time(int T){
		time_period = T;
	}
	byte InterruptHandler::get_start_block_id(){
		return(starting_block_id);
	}
	Block* InterruptHandler::get_starting_Block(){
		return(starting_block);
	}

	bool InterruptHandler::set_start_block(Block* blockList[],int blockList_N){
		robot = Block::robot;
		if(starting_block_id != 0){
            for(int jj = 0 ; jj <  blockList_N ; jj++){
                if(starting_block_id == blockList[jj]->getID()){
                   starting_block = blockList[jj];
                  return(true);
                }
            }
            return(false);
          }else{
            return(true);
          }
	}