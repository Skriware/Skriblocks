	#include "BlockHandler.h"

	BlockHandler::BlockHandler(){
		init();
	}

	void BlockHandler::init(){
		runCode = false;
		for(int tt = 0; tt < blockList_MAX; tt++){
			if(tt < IfblockList_MAX)IfblockList[tt] = NULL;
			if(tt < LoopblockList_MAX)LoopblockList[tt] = NULL;
			if(tt < LogicblockList_MAX)LogicblockList[tt] = NULL;
			if(tt < AritmeticblockList_MAX)AritmeticblockList[tt] = NULL;
      if(tt < MAX_INTERRUPTS)Interrupts[tt] = NULL;	
			blockList[tt] = NULL;
		}
	current = NULL;
  StartBlock = NULL;
	 blockList_N 		= 0;
	 IfblockList_N 	 = 0;
	 LoopblockList_N 	= 0;
	 LogicblockList_N 	= 0;
	 AritmeticblockList_N 	= 0;
	 Mcursor = 0;
	 messageLength = 0;
   interrupt_running = MAX_INTERRUPTS;
   interrupts_N = 0;
   interruped_precesed = false;
	}

  void BlockHandler::clear(){
    runCode = false;
    for(int tt = 0; tt < blockList_MAX; tt++){
      delete blockList[tt];
    }
    for(byte yy = 0; yy <MAX_INTERRUPTS;yy++)delete Interrupts[yy];
   init();
  }
	void BlockHandler::addLoop(int id,	int startBlockID,	int endBlockID,		int count){
		 Loop *l = new Loop(id,startBlockID,endBlockID,count);
      	 blockList[blockList_N] = l;
      	 blockList_N++;
      	 LoopblockList[LoopblockList_N] = l;
      	 LoopblockList_N++;
	}

	void BlockHandler::addBlock(int id,	int _nextBlockID,int _actionID,int _intInput,int _intOutput){
		Block *block = new Block(id,_nextBlockID,_actionID,_intInput,_intOutput);
      	blockList[blockList_N] = block;
      	blockList_N++;
      	Serial.println(blockList_N);
	}

	void BlockHandler::addIf(int id,int _next_true, int _next_false, int _logic_block){
		Conditional *condition = new Conditional(id,_next_true,_next_false, _logic_block);
      	blockList[blockList_N] = condition;
      	blockList_N++;
      	IfblockList[IfblockList_N] = condition;
      	IfblockList_N++;
      	
	}

	void BlockHandler::addLogic(int id,int logicOperation,int _input_left, int _input_right){
		LogicBlock *lblock = new LogicBlock(id,logicOperation,_input_left,_input_right);
		blockList[blockList_N] = lblock;
    blockList_N++;
		LogicblockList[LogicblockList_N] =  lblock;
		LogicblockList_N++;
		
	}

	void BlockHandler::addLogicCompare(int id,int logicOperation,int _input_left, int _input_right){
		LogicCompare *lblock = new LogicCompare(id,logicOperation,_input_left,_input_right);
		blockList[blockList_N] = lblock;
      	blockList_N++;
		LogicblockList[LogicblockList_N] =  lblock;
		LogicblockList_N++;
	
	}


	void BlockHandler::addConst(int id, int32_t value){
		ConstBlock *cblock = new ConstBlock(id,value);
		blockList[blockList_N] = cblock;
      	blockList_N++;
      
	}

	void BlockHandler::addAritmeticBlock(int id,int _operation,int _left,int _right){
		AritmeticBlock *ablock = new AritmeticBlock(id,_operation,_left,_right);
		blockList[blockList_N] = ablock;
		AritmeticblockList[AritmeticblockList_N] = ablock;
		blockList_N++;
		AritmeticblockList_N++;
	}

  void BlockHandler::addInterrupt(byte type,byte input,byte trigger,byte _priority,byte _starting_block_id){
    Interrupts[interrupts_N]= new InterruptHandler(type,input,trigger,_priority,_starting_block_id);
    interrupts_N++;
  }

  byte BlockHandler::checkForInterrupts(){
    Serial.println("Checking for interrupts!");
    byte interrupts_triggered = 0;
    byte triggered_interrupt_ids[8];
    for(byte ii = 0; ii < interrupts_N; ii++){
      Serial.print("Interrupt:");

      if(Interrupts[ii]->Check_for_interrupt()){
         Serial.println("OK");
        triggered_interrupt_ids[interrupts_triggered] = ii;
        interrupts_triggered++;
      }
    }
    if(interrupts_triggered == 1){
      return(triggered_interrupt_ids[0]);
    }else if(interrupts_triggered ==0){
      return(MAX_INTERRUPTS);
    }else if(interrupts_triggered >1){
      byte highest_priority_id = triggered_interrupt_ids[0];
      for(byte rr = 1; rr < interrupts_triggered;rr++){
        if(Interrupts[highest_priority_id]->get_priority() < Interrupts[triggered_interrupt_ids[rr]]->get_priority()){
          highest_priority_id = triggered_interrupt_ids[rr];
        }
      }
      return(highest_priority_id);
    }
    return(MAX_INTERRUPTS);
  }


	bool BlockHandler::MakeConections(){
		#if ENABLED(DEBUG_MODE)
      Serial.println("Making connections!");          
    #endif
  
		for(int ii = 0 ; ii <  blockList_N ; ii++){
			 if(!blockList[ii]->set_next(blockList,blockList_N))return(false);         
		  }
      #ifdef DEBUG_MODE
      Serial.println("Next OK");
      #endif

			for(int kk = 0 ; kk <  blockList_N ; kk++){
			 if(!blockList[kk]->set_input(blockList,blockList_N))return(false);  
			}

			#ifdef DEBUG_MODE
        Serial.println("Input OK");
        #endif

      for(int tt = 0 ; tt <  blockList_N ; tt++){
			 if(!blockList[tt]->set_output_block(blockList,blockList_N))return(false);  
			 }
#ifdef DEBUG_MODE
       Serial.println("Output OK");
       #endif
  		for(int kk = 0 ; kk < LogicblockList_N ; kk++){
  			if(!LogicblockList[kk]->set_logics(blockList,blockList_N))return(false);
  		}
#ifdef DEBUG_MODE
      Serial.println("Logic OK");
      #endif
  		for(int kk = 0 ; kk < IfblockList_N ; kk++){
  			if(!IfblockList[kk]->set_logics(blockList,blockList_N,LogicblockList,LogicblockList_N))return(false);
  		}
    #ifdef DEBUG_MODE
      Serial.println("IF OK");
    #endif
  		for(int jj = 0 ; jj < LoopblockList_N ; jj++){
  			if(!LoopblockList[jj]->set_connections(blockList,blockList_N))return(false);
  		}
      #ifdef DEBUG_MODE
      Serial.println("LOOP OK");
      #endif
  		for(int ll = 0; ll < AritmeticblockList_N ; ll++){
  			if(!AritmeticblockList[ll]->set_connections(blockList,blockList_N))return(false);
  		}
       #ifdef DEBUG_MODE
      Serial.println("Aritmetic OK");
      #endif
  		 for(int jj = 0 ; jj <  blockList_N ; jj++){
          if(1 == blockList[jj]->getID()){
            StartBlock = blockList[jj];
            break;
          }
		  }
      if(StartBlock == NULL)return(false);
		  current = StartBlock;

    #ifdef DEBUG_MODE
      Serial.println("Start OK");
    #endif
      for(byte tt = 0; tt < interrupts_N;tt++){
        if(!Interrupts[tt]->set_start_block(blockList,blockList_N))return(false);
      }
      
    #if ENABLED(DEBUG_MODE)
      Serial.println("Connections done!");          
    #endif

      return(true);
	}
	bool BlockHandler::doBlock(bool loopmode){
    #ifdef DEBUG_MODE
		   		Serial.println(current->getID());
    #endif

    Block::robot->wait_And_Check_BLE_Connection(5,2);      
    current->do_action();

    #ifdef DEBUG_MODE
			 Serial.println(current->getNextID());
    #endif

    if(interrupt_running == 8 && !interruped_precesed)interrupt_running = checkForInterrupts();  //do not chack interrupts if you are in one alerady
		
    Serial.println(interrupt_running);
    if(interrupt_running !=8 && !interruped_precesed){
      Interrupts[interrupt_running]->set_interrupted_block(current); 
      Serial.print("Interrupted block:");
      Serial.println(current->getID());
      current = Interrupts[interrupt_running]->get_starting_Block();
      interruped_precesed = true;
    }else{
      current = current->get_next(); 
    }
        if (current == NULL){
          if(interrupt_running !=8){
            Serial.println("END of interrupt!");
            current = Interrupts[interrupt_running]->get_interrupted_block();
            interrupt_running = 8;
            interruped_precesed = false;
          }else if(loopmode){
            current = StartBlock;
            return(true);
          }else{
            return(false);
          }
        }

        return(true);
    }


int BlockHandler::freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

int32_t BlockHandler::readInt(){
  int nDigits = 0;
  int sign = 1;
  while((AllMessage[Mcursor + nDigits] != ' ') && (AllMessage[Mcursor + nDigits] != '\n')){
      if(AllMessage[Mcursor + nDigits] == '-')sign = -1;
      nDigits++;
  }
  if(sign==-1){
    Mcursor++;
    nDigits--;
  }
  int32_t out = 0;
  int32_t power = 1;
    for(int ii = nDigits-1; ii > -1; ii--){
      if(ii != nDigits-1) power *=10;
      int32_t add = cti(AllMessage[Mcursor + ii])*power;
      out += add;
    }
    #ifdef DEBUG_MODE
    Serial.println(out*sign);
    #endif
    Mcursor += nDigits+1;
    return(out*sign);
}

int BlockHandler::cti(char x){
  int y = x - '0';
  return(y);
}

int BlockHandler::Handle_Msg(){
  int id;
  bool codeOk;
  if(AllMessage[Mcursor] == 'R'){
             codeOk = MakeConections();
              if(AllMessage[Mcursor+4] == 'C'){
              	if(codeOk){
                  return(0);
                }else{
                  return(3);
                }
              }else{
                if(codeOk){
              	 return(1);
                }else{
                  return(3);
                }
               }
  }
          id = readInt();
          byte startBlockID;
          byte endBlockID; 
          int countID; 
          int32_t value;
          byte actionID;
          byte input,output;
          byte next;
          byte nextTrue,nextFalse,logicBlock;
          byte input_left,input_right, compareOperation;
  switch(AllMessage[Mcursor]){
    case 'L':
          Mcursor += 2;
          countID = readInt();
          startBlockID  = readInt();
          endBlockID = readInt();
          addLoop(id,startBlockID,endBlockID,countID);
          break;
     case 'C':
          Mcursor += 2;
          if(AllMessage[Mcursor] == 'I'){
            value = -1;
            Mcursor += 2;
          }else if(AllMessage[Mcursor] == 'T'){
            value = 1;
            Mcursor += 2;
          }else if(AllMessage[Mcursor] == 'F'){
            value = 0;
            Mcursor += 2;
          }else{
            value = readInt();
          }
            addConst(id,value);
          break;
      case 'I':
          Mcursor += 2;
          logicBlock = readInt();
          nextTrue = readInt();
          nextFalse = readInt();
          addIf(id,nextTrue,nextFalse,logicBlock);
      break;

      case 'O':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	addAritmeticBlock(id,compareOperation,input_left,input_right);
      break;
      case 'K':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	addLogic(id,compareOperation,input_left,input_right);
      break;

      case 'U':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	addLogicCompare(id,compareOperation,input_left,input_right);
      break;
      case 'J':
          Mcursor += 2;
          next = readInt();
          Serial.print("Starting Block ID:");
          Serial.println(next);
          addBlock(id,next,69,0,0);
       break;
      case 'A':
          Serial.print("ACTION:");
           Mcursor +=2;
          Serial.print("ActionID: ");
          actionID = readInt();
          input = readInt();
          output = readInt();
          next  = readInt();
          addBlock(id,next,actionID,input,output);
          break;
      default:
            #if ENABLED(DEBUG_MODE)
              Serial.println("Got sth stragne!");          
            #endif
        break;

  }

  return(2);    
}

