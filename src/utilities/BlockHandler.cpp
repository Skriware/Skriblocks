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
   millis_left_from_interrupt = 0;
   transfereBlocks = false;
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

	void BlockHandler::addBlock(int id,  int _nextBlockID,int _actionID,byte* _usedBlocksIDs,byte _NusedBlocks){
		Block *block = new Block(id,_nextBlockID,_actionID,_usedBlocksIDs,_NusedBlocks);
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

void BlockHandler::addConst(int id, int32_t *value,byte N){
    ConstBlock *cblock = new ConstBlock(id,value,N);
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

  void BlockHandler::addInterrupt(byte type,byte input,byte trigger,byte _priority,byte _starting_block_id,int value){
    Interrupts[interrupts_N]= new InterruptHandler(type,input,trigger,_priority,_starting_block_id);
    Interrupts[interrupts_N]->setValue(value);
    interrupts_N++;
  }

  bool BlockHandler::checkForInterrupts(){
    if(interrupt_running == MAX_INTERRUPTS && !interruped_precesed){
    byte interrupts_triggered = 0;
    byte triggered_interrupt_ids[MAX_INTERRUPTS];
    for(byte ii = 0; ii < interrupts_N; ii++){
      if(Interrupts[ii]->Check_for_interrupt()){
        triggered_interrupt_ids[interrupts_triggered] = ii;
        interrupts_triggered++;
      }
    }
    if(interrupts_triggered == 1){
      interrupt_running = triggered_interrupt_ids[0];
      return(true);
    }else if(interrupts_triggered ==0){
      interrupt_running = MAX_INTERRUPTS;
      return(false);
    }else if(interrupts_triggered >1){
      byte highest_priority_id = triggered_interrupt_ids[0];
      for(byte rr = 1; rr < interrupts_triggered;rr++){
        if(Interrupts[highest_priority_id]->get_priority() < Interrupts[triggered_interrupt_ids[rr]]->get_priority()){
          highest_priority_id = triggered_interrupt_ids[rr];
        }
      }
      interrupt_running = highest_priority_id;
      return(true);
    }
  }
  return(false);
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
			 if(!blockList[kk]->set_used_block(blockList,blockList_N))return(false);  
			}

			#ifdef DEBUG_MODE
        Serial.println("Used blocks OK");
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
      Serial.println("interrupts OK");
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
          Serial.print("current:");
		   		Serial.println(current->getID());
    #endif

    Block::robot->wait_And_Check_BLE_Connection(5,2);      
    current->do_action();

    #ifdef DEBUG_MODE
       Serial.print("next:");
			 Serial.println(current->getNextID());
    #endif

    checkForInterrupts();  //do not check interrupts if you are in one alerady
    if(interrupt_running !=MAX_INTERRUPTS && !interruped_precesed){
      current->interrupted = true;
      Interrupts[interrupt_running]->set_interrupted_block(current); 
      current = Interrupts[interrupt_running]->get_starting_Block();
      interruped_precesed = true;
    }else{
      current = current->get_next(); 
    }
        if (current == NULL){
          if(interrupt_running !=MAX_INTERRUPTS){
            current = Interrupts[interrupt_running]->get_interrupted_block();
            interrupt_running = MAX_INTERRUPTS;
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
    //Serial.println(out*sign);
    #endif
    Mcursor += nDigits+1;
    return(out*sign);
}


int32_t* BlockHandler::readMultipleInts32(byte *N){
  int start_cursor_pos = Mcursor;
  byte intsToRead = 0;
  while(AllMessage[Mcursor-1] != '\n'){
    readInt();
    intsToRead++;
  }
if(intsToRead !=0){
  int32_t *tmp = new int32_t[intsToRead];
  Mcursor = start_cursor_pos;
  for(byte yy = 0; yy < intsToRead; yy++)tmp[yy] = readInt();
    *N = intsToRead;
    return(tmp);
}else{
  *N = 0;
  return(NULL);
}
  *N = 0;
  return(NULL);
}

byte* BlockHandler::readMultipleInts(byte *N){
  int start_cursor_pos = Mcursor;
  byte intsToRead = 0;
  while(AllMessage[Mcursor-1] != '\n'){
    readInt();
    intsToRead++;
  }
if(intsToRead !=0){
  byte *tmp = new byte[intsToRead];
  Mcursor = start_cursor_pos;
  for(byte yy = 0; yy < intsToRead; yy++)tmp[yy] = readInt();
    *N = intsToRead;
    return(tmp);
}else{
  *N = 0;
  return(NULL);
}
  *N = 0;
  return(NULL);
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
          byte input;
          byte next;
          byte nextTrue,nextFalse,logicBlock;
          byte input_left,input_right, compareOperation;
          byte type,trigger,priority, starting_block;
          byte *tmp_b;
          int32_t *tmp_32;
          int *tmp_int;
          Serial.println(AllMessage[Mcursor]);
  switch(AllMessage[Mcursor]){
    case 'L':
          Mcursor += 2;
          endBlockID = readInt();
          countID = readInt();
          startBlockID  = readInt();
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
            tmp_32 = readMultipleInts32(&input);
            if(input == 1){
            addConst(id,tmp_32[0]);
            }else if(input >1){
            addConst(id,tmp_32,input);
            }
            break;
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
          addBlock(id,next,69);
       break;
      case 'A':
          Mcursor +=2;
          actionID = readInt();
          next  = readInt();
          tmp_b = readMultipleInts(&input);
          addBlock(id,next,actionID,tmp_b,input);
          break;
      case 'V':
          Mcursor +=2;
          type = readInt();
          input = readInt();
          trigger = readInt();
          value = readInt();
          priority = readInt();
          starting_block = readInt();
          addInterrupt(type,input,trigger,priority,starting_block,value);
          break;
      default:
            #if ENABLED(DEBUG_MODE)
              Serial.println("Got sth stragne!");          
            #endif
        break;
  }
  return(2);    
}

void BlockHandler::active_wait(uint32_t ms, int interval,bool interrupted,bool *interrupt_info){
    if(interrupted)ms = millis_left_from_interrupt;
    int loop_iterator = ms/interval;
    int ms_left_befor_loop = ms%interval;
    if(millis_left_from_interrupt !=0 && interrupt_running == MAX_INTERRUPTS){
      loop_iterator = millis_left_from_interrupt/interval;
      ms_left_befor_loop = millis_left_from_interrupt%interval;
      millis_left_from_interrupt = 0;
    }
    delay(ms_left_befor_loop);
    for(int yy = 1; yy < loop_iterator; yy++){
          if(Block::robot->using_BLE_Connection && !Block::robot->connection_Break_Reported && Block::robot->BLE_checkConnection() == false){
          Block::robot->connection_Break_Reported = true;
          #ifdef DEBUG_MODE
          if(Block::robot->connection_Break_Reported)Serial.println("Connection LOST!");
          #endif
        }else if(Block::robot->using_BLE_Connection && !Block::robot->program_End_Reported && Block::robot->BLE_dataAvailable() > 0){
            char tmp;
            while(Block::robot->BLE_dataAvailable() > 0){
              tmp = Block::robot->BLE_read();
              delay(5);                     // to be sure that next char will be recieved
              if((tmp == 'E' && Block::robot->BLE_read() == 'N' && Block::robot->BLE_read() == 'D') || (tmp == 'B' && Block::robot->BLE_read() == 'E' && Block::robot->BLE_read() == 'G')){
                Block::robot->program_End_Reported = true;
              }
              #ifndef ESP_H && _VARIANT_BBC_MICROBIT_
              if(tmp != 'B')serialFlush();
              #endif
            }
            if(Block::robot->program_End_Reported || Block::robot->connection_Break_Reported)break;
          
      }
      if(checkForInterrupts()){
       Serial.println("Interrupts!");
        millis_left_from_interrupt = (loop_iterator - yy)*interval;
        if(interrupt_info != NULL) *interrupt_info = false;
        Serial.print("millis left:");
        Serial.println(millis_left_from_interrupt);
        break;
      }
      if(Block::robot->stausLEDused)Block::robot->BaterryCheck();
      delay(interval);
    } 
}

  void BlockHandler::AddToMessage(char x){
    AllMessage[messageLength] = x;
    messageLength++;
    #ifdef DEBUG_MODE
    Serial.print(x);
    #endif
  }

  void BlockHandler::CheckLongCodes(char *asciTmp){
    if(*asciTmp == 'B'){                                                  //Baptised case 
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              AddToMessage(tmp2);
              if(tmp2 == 'A'){
                *asciTmp = 'b';
                break; 
              }else if(tmp2 == 'E'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
              }
            }
          }
        }else if(*asciTmp == 'R'){
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              AddToMessage(tmp2);
              tmp2 = Block::robot->BLE_read();
              AddToMessage(tmp2);
              if(tmp2 == 'S'){
                *asciTmp = 'r';
                break; 
              }else if(tmp2 == 'N'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
              }
            }
          }
        }
  }

  bool BlockHandler::CheckForTimeout(){
            bool tmp = false;
            long last_message_time = millis();
            long last_ack_send = last_message_time;
            Block::robot->BLE_write("ack\n\r\n");
            while((Block::robot->BLE_dataAvailable() == 0)){
              if(millis() - last_message_time > MESSAGE_TIMEOUT){
                tmp = true;
                break;
              }else if(millis() - last_ack_send > ACK_RESEND_TIME){
                Block::robot->BLE_write("ack\n\r\n");
                last_ack_send = millis();
              }
            }
            return(tmp);
  }

  byte BlockHandler::readMessageLine(){
      char MainAsci,asciTmp;
      asciTmp = '0';
      if(Block::robot->BLE_dataAvailable()){
        MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
        AddToMessage(MainAsci);
        CheckLongCodes(&MainAsci);
        if(MainAsci == INVALID_MSG_ERROR_CODE) return(INVALID_MSG_ERROR_CODE);
        while(asciTmp != '\n'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
            AddToMessage(asciTmp);
          }else{
           if(CheckForTimeout())return(TIMEOUT_ERROR_CODE);
          }
        }
        return(MainAsci);
      }else{
        return(NO_MSG_CODE);
      }
  }
  byte BlockHandler::readCodeLine(){
    char MainAsci,asciTmp;
    asciTmp = '0';
    if(Block::robot->BLE_dataAvailable()){
      MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
      AddToMessage(MainAsci);
      if(MainAsci == 'R'){
        //while(Block::robot->BLE_dataAvailable())AddToMessage(Block::robot->BLE_read());
        return(CODE_COMPLETE);
      }
      asciTmp = MainAsci;
    while(asciTmp != '\n'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
            AddToMessage(asciTmp);
          }else{
            if(CheckForTimeout())return(TIMEOUT_ERROR_CODE);
          }
        }
    }else{
      Block::robot->BLE_write("ack\n\r\n");
      return(NO_MSG_CODE);
    }
    transfereBlocks = false;
    return(CODE_PASSED);
  }
  void BlockHandler::processMessageLine(byte LineCode){
        char tmp;
        char tmpNameArray[32] = {' '};
        int tmpCounter;
        switch(LineCode){
          case BAPTISED:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("Named");
                #endif
                  Block::robot->sendNameInfo();
                  clear();
          break;
          case RENAME:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("NewName");
                #endif
                tmp = Block::robot->BLE_read();
                tmpNameArray[32] = {' '};
                tmpCounter = 0;
                while(tmp != '\n'){
                  if(Block::robot->BLE_dataAvailable()){
                    tmpNameArray[tmpCounter] = tmp;
                    tmpCounter++;
                    tmp = Block::robot->BLE_read();
                  }
                }
                Block::robot->BLE_write("OK\n");
                #if ENABLED(DEBUG_MODE)
                  Serial.println(tmpNameArray);
                #endif  
                Block::robot->BLE_changeName(tmpNameArray);
                clear();
          break;
          case VERSION:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("Version Request");
                #endif
                Block::robot->BLE_write("1");
                clear();
          break;
          case RESET:
              #if ENABLED(DEBUG_MODE)
                Serial.println("RESET BLE");
              #endif
                Block::robot->BLE_write("OK\n");
                Block::robot->BLE_reset();
                clear();
          break;
          case END:
                clear();
                Block::robot->Stop();
                #ifndef _VARIANT_BBC_MICROBIT_
                Block::robot->OpenClaw();
                Block::robot->Put_Down();
                #endif
          break;
          case BEGIN:
                transfereBlocks = true;
          break;
          case NO_MSG_CODE:
          break;
          default:
                clear();
          break;
        }
  } 

  