/**
*Included the Flash Library http://arduiniana.org/libraries/flash/ 
*A Library to Ease Accessing Flash-based (PROGMEM) Data
*This was used to store the example Bytecode from the LEGO bytecode documentation
**/
//#include <Flash.h> 

/***
* The bytecode below is stored in flash at compile time, it will eventually
* be placed on an SDCard so it can be loaded on during program execution after the 
* Arduino firmware has been compiled
**/
const char * bytecode_serialized =
//"4D696E6473746F726D734E5854000006040016000C000F0005000A00000000000C00010007000600000006010400010108000100090088130000010C000A000100FFFFFFFFFF0000000000800100030000002A60FFFFFFFF";
//"4D696E6473746F726D734E5854000005030016000C000F0005000A00000000000C000100070006010000060004000100080088130000010C000A000100FFFFFFFFFF0000000000800000010002002A60FFFFFFFF";
//"4D696E6473746F726D734E5854000006040016000C000F0005000A00000000000C00010007000600000006010400010108000100090088130000010C000A000100FFFFFFFFFF0000000001800100030000002A60FFFFFFFF";
"4D696E6473746F726D734E5854000006050016000C000F0005000A00000000000C0001000F0006000000060104000401080001010A0001000B0088130000010C000A000100FFFFFFFFFF000000000080010004000000008000000000040001800200010004002A60FFFFFFFF";
long len_bc = strlen(bytecode_serialized);// 
//long len_bc = (sizeof(bytecode_serialized));
/*
FLASH_TABLE(byte, bytecode, 10, 
{0x4D, 0x69, 0x6E, 0x64, 0x73, 0x74, 0x6F, 0x72, 0x6D, 0x73},
{0x4E, 0x58, 0x54, 0x00, 0x00, 0x05, 0x03, 0x00, 0x16, 0x00},
{0x0C, 0x00, 0x0F, 0x00, 0x05, 0x00, 0x0A, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x07, 0x00, 0x06, 0x01},
{0x00, 0x00, 0x06, 0x00, 0x04, 0x00, 0x01, 0x00, 0x08, 0x00},
{0x88, 0x13, 0x00, 0x00, 0x10, 0xC0, 0x00, 0xA0, 0x00, 0x10},
{0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00},
{0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x20, 0x02, 0xA6, 0x0F},
{0xFF, 0xFF, 0xFF, 0x0F}); 
*/
byte flash[100];
/*{{0x4D, 0x69, 0x6E, 0x64, 0x73, 0x74, 0x6F, 0x72, 0x6D, 0x73},
{0x4E, 0x58, 0x54, 0x00, 0x00, 0x05, 0x03, 0x00, 0x16, 0x00},
{0x0C, 0x00, 0x0F, 0x00, 0x05, 0x00, 0x0A, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x07, 0x00, 0x06, 0x01},
{0x00, 0x00, 0x06, 0x00, 0x04, 0x00, 0x01, 0x00, 0x08, 0x00},
{0x88, 0x13, 0x00, 0x00, 0x10, 0xC0, 0x00, 0xA0, 0x00, 0x10},
{0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00},
{0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x20, 0x02, 0xA6, 0x0F},
{0xFF, 0xFF, 0xFF, 0x0F}};
*/
/**
* Manually set SRAM size to manage static and dynamic data
**/
//SRAM width size
#define  SRAMX 4
//SRAM height size
#define	SRAMY 50


long sram[10];

long * sram_ptr = sram;

//Dataspace table of contents entry size
#define	DSTOC_ENTRY 4
//Default dataspace value
#define DEFAULT_VALUE 0
//The PC register
long program_counter = 0;
/**
* Dataspace Headers
**/
int dsh_dstoc_count; //Number of records in the DSTOC
int dsh_initial_size; //Initial size , in, bytes, of both static and dynamic data items of the dataspace in RAM
int dsh_static_size; //Size, in bytes, of the static segement of the dataspace in RAM
int dsh_default_data_size;
int dsh_dynamic_default_offset;
int dsh_dynamic_default_size;
int dsh_memory_manager_head;
int dsh_memory_manager_tail;
int dsh_dope_vector_offset;

//clump related dataspace headers
int dsh_clump_count;
int dsh_code_word_count;

  /**
  * 
  **/
  //byte byte_slice[48];
  

void setup() {
  /**
  * Testing code functionality below
  **/
  //Serial.print("SRAM PTR X 0");Serial.println(*sram_ptr);
  // initialize serial:
  Serial.begin(9600);
     
    Serial.println("Starting...");
    
    initialise_bytecode(); 
    checkVersion();
    getDataspaceHeaders();
    initialiseDataspace();
    getClumpRecords();
    executeCodespace();
    /**
    * getLongInstruction();
    * getShortInstruction();
    * ;
    **/
}

void loop() {
  // put your main code here, to run repeatedly: 
}

/**
* Get the LEGO Mindstorms version CODE
**/
byte* checkVersion(){
  //return getByte variable for print
  byte byte_slice[10];
  
  //Print "Mindstorms" as the first bytes
  Serial.println("Checking Version...");
  Serial.write(getByte(0,9, byte_slice ),10);
  //printArray(getByte(0,9));
  Serial.println("");

  program_counter += 16;
  return NULL;
}

/**
*Get the values to be assigned to the dataspace header variables
**/

void getDataspaceHeaders(){
    Serial.println("Getting DSPACE Headers...");
    dsh_dstoc_count = getByte(program_counter);
    dsh_initial_size = getByte(program_counter += 2);
    dsh_static_size = getByte(program_counter += 2);
    dsh_default_data_size = getByte(program_counter += 2);
    dsh_dynamic_default_offset = getByte(program_counter += 2);
    dsh_dynamic_default_size = getByte(program_counter += 2);
    dsh_memory_manager_head = getByte(program_counter += 2);
    dsh_memory_manager_tail = getByte(program_counter += 2);
    dsh_dope_vector_offset = getByte(program_counter += 2);
    dsh_clump_count = getByte(program_counter += 2);
    dsh_code_word_count = getByte(program_counter += 2);
    program_counter += 2;
}


void  initialiseDataspace(){
  Serial.println("Initializing DATASPACE...");
    
    //dstoc_pc used to traverse items in the dstoc
    long dstoc_pc = program_counter;
    // the number of bytes in each datatype in the dstoc
    int dstoc_type[]={0,1,1,2,2,4,4,0,0,0};
    
    
    //skip the DSTOC
    program_counter += (dsh_dstoc_count*DSTOC_ENTRY);
    
    /**
    * Loops through all elements of the DSTOC and populates SRAM
    **/
    for(int d=0; d<dsh_dstoc_count; d++){
        //get data type
        byte type = getByte(dstoc_pc);
        //get the flag
        byte flag = getByte(dstoc_pc+1);
        
        /**
        * If the flag is present, fill the currenLong with the DEFAULT VALUE
        otherwise, read and populate the actual value from the bytecode.
        **/
        if(!flag){
        //calculate the range of bits to extract from the bytecode based on the length of the data
        byte byte_slice[33];
        long endrange;
        endrange = program_counter+dstoc_type[type]-1;
        getByte(program_counter, endrange, byte_slice);
       
        
        /**
        * Convert the retrieved bytes into a set of 32 bit longs
        **/
        //varibale that stores the addressed long to be stored in sram
        long currentLong;
        int sizeOfB = sizeof byte_slice / sizeof(byte);
        int sizeOfL = sizeOfB / 4;
        if(sizeOfB % 4 != 0) ++sizeOfL;

            for(int i = 0; i < sizeOfB; i+=4){
                currentLong = 0;
                if(byte_slice[i]=='\0'){break;}
                if(i + 3 < sizeOfB) currentLong = (currentLong << 8) + byte_slice[i+3];
                if(i + 2 < sizeOfB) currentLong = (currentLong << 8) + byte_slice[i+2];
                if(i + 1 < sizeOfB) currentLong = (currentLong << 8) + byte_slice[i+1];
                currentLong = (currentLong << 8) + byte_slice[i+0];
                *sram_ptr++ = currentLong;            
            }
            //point program counter to next Static data entry in bytecode
            program_counter+=dstoc_type[type];
        }
        else{
          *sram_ptr++ = DEFAULT_VALUE;
        }
        //increment to the next dstoc entry
        dstoc_pc+=DSTOC_ENTRY;
    }
    /**
    * For now we are going to assume there is no dynamic data in the bytecode. 
    * This is only for testing purposes, so we will skip the default dope vector
    * array (of 10 bytes) at the end of the dataspace
    **/
    program_counter+=10;
    //skip the padding 
        program_counter+=1;
    
    
    //output current SRAM contents
    Serial.println("DATASPACE Initialized!");
    Serial.println("-----DS-----");
    printArray(sram);
    Serial.println("-----/DS-----");
    
}
    
/**
* Organise the clump record segment by finding particular variables
* for now we will skip this.
**/
void getClumpRecords(){
  //replace with (clump count X clump record data size)
  Serial.println("Getting CLUMP Records...");
  program_counter+=4;
  }
  
  
  
long * charA2LongA(byte * byte_slice, long * ret_array){
  /**
   * Convert the retrieved bytes into a set of 32 bit longs
   **/
   //varibale that stores the addressed long to be stored in sram
   long currentLong;
   int sizeOfB = sizeof byte_slice / sizeof(byte);
   int sizeOfL = sizeOfB / 4;
   if(sizeOfB % 4 != 0) ++sizeOfL;
   //Serial.print("CALLED:  ");
       for(int i = 0; i < sizeOfB; i+=4){
           currentLong = 0;
           if(byte_slice[i]=='\0'){break;}
           if(i + 3 < sizeOfB)currentLong = (currentLong << 8) + byte_slice[i+3]; //Serial.print("ByteSlice 3:  ");Serial.println(byte_slice[i+3]);Serial.println(currentLong);
           if(i + 2 < sizeOfB) currentLong = (currentLong << 8) + byte_slice[i+2]; //Serial.print("ByteSlice 2:  ");Serial.println(byte_slice[i+2]);Serial.println(currentLong);
           if(i + 1 < sizeOfB) currentLong = (currentLong << 8) + byte_slice[i+1]; //Serial.print("ByteSlice 1:  ");Serial.println(byte_slice[i+1]);Serial.println(currentLong);
           currentLong = (currentLong << 8) + byte_slice[i+0]; //Serial.print("ByteSlice 0:  ");Serial.println(byte_slice[i+0]);Serial.println(currentLong);
           ret_array[i] = currentLong;  //Serial.print("CurrentLong:  "); //Serial.println(currentLong);Serial.println(currentLong);
       }
       //pointer to the retrn array in the parent scope
       return ret_array;
}
  
void grabCodeSpace(long * source1, long * source2){
  byte destinationB[4];
  byte src0B[4];
  byte src1B[4];

  long destinationL[1]={0};
  long src0L[1]={0};
  long src1L[1]={0};
  Serial.println("Do SOMN");

  getByte(program_counter+2,program_counter+3,destinationB); //destination
  getByte(program_counter+4,program_counter+5,src0B); //source0
  getByte(program_counter+6,program_counter+7,src1B); //source1
  
  //program_counter+=8;
  charA2LongA(destinationB, destinationL);
  charA2LongA(src0B, src0L);
  charA2LongA(src1B, src1L);
  
  Serial.print("DEST: ");Serial.println(destinationL[0]);
  Serial.print("SRC1: ");Serial.println(src0L[0]);
  Serial.print("SRC2: ");Serial.println(src1L[0]);
  
  long *sram_start = sram;
  Serial.print("SRMSTRT: ");Serial.println((long)sram_start);
  sram_ptr=(long *)(destinationL[0]+sram_start);
  source1 = (long *)(src0L[0]+sram_start);
  Serial.print("Source 1: ");Serial.println(*source1);
  
  source2 = (long *)(src1L[0]+sram_start);
  
}
void getLongInstruction(){
  
 
  
  byte opcode = getByte(program_counter);
  switch (opcode) {
      case 0x00:{
        //OP_ADD
        long * source1=0;
        long * source2;
        grabCodeSpace(source1, source2);
        Serial.print("Source 1B: ");Serial.println(*source1);
        *sram_ptr= *source1+*source2;
        break;
        }
      case 0x01:{
        Serial.println("Do Subtraction");
        /*getByte(program_counter+2,program_counter+3,destinationB); //destination

        getByte(program_counter+4,program_counter+5,src0B); //source0
        getByte(program_counter+6,program_counter+7,src1B); //source1
        printArray(src1B);
        //program_counter+=8;
        charA2LongA(destinationB, destinationL);
        charA2LongA(src0B, src0L);
        charA2LongA(src1B, src1L);

        Serial.print("DEST: ");Serial.println(destinationL[0]);
        Serial.print("SRC1: ");Serial.println(src0L[0]);
        Serial.print("SRC2: ");Serial.println(src1L[0]);

        long *sram_start = sram;
        sram_ptr=(long *)(destinationL[0]+sram_start);
        long * source1 = (long *)(src0L[0]+sram_start);
        long * source2 = (long *)(src1L[0]+sram_start);

        *sram_ptr= *source2-*source1;*/
        break;
        }
      default:{
        Serial.println("Do Nothing");
        // if nothing else matches, do the default
        // default is optional
      }
    }
  }
  
byte* getShortInstruction(){return NULL;}

void executeCodespace(){
  Serial.println("Executing CodeSpace...");
  Serial.print ("PC START...");Serial.println((program_counter*2)+2);
  //while(!(getByte(program_counter)==0xFF)){
  //Serial.println(getByte(program_counter),HEX);
  //byte lshort = getByte(program_counter+1);
  //Serial.println("In WHILOOP...");
  //if((lshort >> 7)==1){
  for(int x = 0; x < 6; x++){
  getLongInstruction();  Serial.println("GOT LONG INSTR...");
  Serial.print ("INCREMENTED PC...");program_counter+=8;Serial.println((program_counter*2)+2);  
  Serial.println("-----DS-----");printArray(sram);Serial.println("-----/DS-----");
  }
  Serial.println("CodeSpace Executed...");
}

void getDSTOCValue(int index){

}

void printArray(byte * array){
  int z = sizeof(array);///sizeof(byte);
  //Serial.println(z);z=3;
   for (int x = 0; x < z; x++) {
        Serial.println(array[x], HEX); 
    }
}
void printArray(char * array){
   for (int x = 0; x < sizeof(array)/sizeof(char); x++) {
        Serial.println(array[x], HEX); 
    }
}
void printArray(long array[50]){
 
 int t = 0;
 long v=array[t];
 for (int x = 0; x < 5; x++){
      Serial.println(t);
     Serial.print("index ");Serial.print(t);Serial.print(": ");Serial.println(v,HEX);
     t++;
      v= array[t];
    }
}

void printArray(long array[][50]){
  Serial.println("YO!S");
   for (int y = 0; y < SRAMY; y++) {
        for (int x = 0; x < SRAMX; x++) {
            if (!array[x][y])break;
            Serial.println(array[x][y], HEX); 
          }
    }
}
void printArray(byte array[][10]){
  Serial.println("bitten");
   for (int y = 0; y < SRAMY; y++) {
        for (int x = 0; x < SRAMX; x++) {
            if (!array[x][y])break;
            Serial.println(array[x][y], HEX); 
          }
    }
}
/**
* Set a single long at the end of free RAM
**/
void initialise_bytecode(){
  
  
  
  //bytecode size
  int bc_size = len_bc;//sizeof(bytecode_serialized)/sizeof(char);
  //number of columns in the sram layout
   int cols = 10;

   //find the row to store the byte
   long row = (bc_size/cols);

   //find the column with the requested byte
   int col = bc_size%cols;

   int z = 0;
   
    byte * dst = flash;
    byte *end = flash + len_bc;
   
   unsigned int u;
   //Serial.println("LOOOP");
   while (dst < end && sscanf(bytecode_serialized, "%2x", &u) == 1)
   {
      
    //   Serial.println(u, HEX);
       
       *dst++ = u;
       bytecode_serialized += 2;
   }

}

/**
* Set a single long at the end of free RAM
**/
void setLong(long data, int& sram_ptr, long sram[][50]){
  
  //number of columns in the sram layout
  int cols = 4;
  
  //find the row to store the long
  long row = (sram_ptr/cols);
  
  //find the column with the requested byte
  int col = sram_ptr%cols;
  
  //set the long at the specified co-ordinates
  sram[row][col] = data;
  
  //increment sram pointer
  sram_ptr++;
}

/**
* Access any byte in the Bytecode Set by its offset from the top of the file.
* Offsets begin at zero(0)
**/
byte getByte(long offset){

  return flash[offset];
}

/**
* Access any contiguous set of bytes in the Bytecode Set by its offset range
* begin and end points(inclusive) relative to the top of the file.
* Offsets begin at zero(0)
**/
byte* getByte(long offset_start, long offset_end, byte * byte_slice){
  
  //The number of bytes in the range
  long byte_range = offset_end - offset_start;
  
  //limit number of bytes in the range to 48
  if ((byte_range)>47){
    //offset_end = offset_start + 47;
    byte_range = 47;
    //prevent single or negative ranges
  }else if(offset_end<offset_start){
    //offset_end = offset_start + 1;
    byte_range = 1;
  }

 //Loop through the bytecode and extract the requested slice
 for(long y=0; y<=byte_range; y++){ 
   byte_slice[y] = getByte(y+offset_start);
   //Serial.println(byte_slice[y], HEX);
   if(y==byte_range){byte_slice[y+1]='\0';}
 }
 
 /** 
 * Return an array with the requested slice which is the
 * length of the range not greater than 47 or less than 1
  **/ 
 return byte_slice;
}
