#ifndef DEFINE_INTERRUPT_H
#define DEFINE_INTERRUPT_H
#include "Arduino.h"
#include <Skribot.h>
#include "Block.h"

#define BUTTON_INTERRUPT 0			//interrupt types
#define DISTANCE_INTERRUPT 1
#define LINE_INTERRUPT 2
#define TIME_INTERRUPT 3

#define DIST_D1 	0					//interrupt inputs
#define DIST_D2 	1
#define LINE_L1 	2
#define LINE_L2 	3
#define LINE_L3 	4
#define TIME    	5
#define BUTTON_1	6
#define BUTTON_2	7
#define BUTTON_3	8					

										//interrupt triggers
#define DISTANCE_GRATER_THEN 0
#define DISTANCE_LESS_THEN 1
#define LINE_DETECTED 2
#define NO_LINE_DETECTED 3
#define BUTTON_HIGH 4
#define BUTTON_LOW 5


class InterruptHandler {
public:
	InterruptHandler(byte int_type,
					  byte int_input,
					  byte trigger,
					  byte _priority,
					  byte _starting_block_id);
	bool Check_for_interrupt();
	bool Condition_saniti_check();
	void Set_Trig_Distance(byte dist);
	void Set_Trig_Time(int T);
	byte get_start_block_id();
	Block* get_starting_Block();
	bool set_start_block(Block* blockList[],int blockList_N);

protected:
	byte interrupt_type;
	byte starting_block_id;
	byte priority;
	byte trigger_type;
	byte trig_distance;
	byte input;
	int time_period;
	Block *starting_block;
	Skribot *robot;
	long last_interrupt_time;
	};

#endif
