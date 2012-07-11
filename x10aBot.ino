/**
*Included the Flash Library http://arduiniana.org/libraries/flash/ 
*A Library to Ease Accessing Flash-based (PROGMEM) Data
*This was used to store the example Bytecode from the LEGO bytecode documentation
**/
#include <Flash.h> 

/**
* The bytecode below is stored in flash at compile time, it will eventually
* be placed on an SDCard so it can be loaded on during program execution after the 
* Arduino firmware has been compiled
**/
//char my_bytecode[]="4D696E6473746F726D734E5854000005030016000C000F0005000A00000000000C00010007000601000006000400010008008813000010C000A000100FFFFFFFFFF0000000000800000010002002A60FFFFFFF0F";

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
/*
byte flash[10][10]={{0x4D, 0x69, 0x6E, 0x64, 0x73, 0x74, 0x6F, 0x72, 0x6D, 0x73},
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
long sram[4][50];

long * sram_ptr;



//Dataspace table of contents entry size
#define  DSTOC_ENTRY 4

//The PC register
long program_counter = 0;
/**
* Dataspace Headers
**/
int dsh_dstoc_count; //Number of recordss in the DSTOC
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
  byte byte_slice[47];
  

void setup() {
  /**
  * Testing code functionality below
  **/
  
  // initialize serial:
  Serial.begin(9600);
  

  
  
  checkVersion();
  getDataspaceHeaders();
  initialiseDataspace();
}

void loop() {
  // put your main code here, to run repeatedly: 
}

/**
* Get the LEGO Mindstorms version CODE
**/
byte* checkVersion(){
  //Print "Mindstorms" as the first bytes
  Serial.println("__________");
  Serial.write(getByte(0,9, byte_slice ),10);
  //printArray(getByte(0,9));
  Serial.println("");
  
  //Print the Letter 's', last letter of Mindstorms
  //Serial.write(getByte(9));
  
  
  program_counter += 16;
  return NULL;
}

/**
*Get the values to be assigned to the dataspace header variables
**/

void getDataspaceHeaders(){
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
    long dstoc_pc = program_counter;
    int dstoc_type[]={0,1,1,2,2,4,4,0,0,0};

    //skip the DSTOC
    program_counter += (dsh_dstoc_count*DSTOC_ENTRY);
    
    //get data type
    byte type = getByte(dstoc_pc);
    //Serial.println(type);
   // byte*  data[10];
    long endrange;
    endrange = program_counter+dstoc_type[type]-1;
    //getByte(program_counter, endrange);
    /*Serial.println("-----");
    Serial.print("Endrange:");
    Serial.println(endrange);
    Serial.print("PC2:   ");
    Serial.println(program_counter);
    Serial.println("*----------------");*/
    printArray(getByte(program_counter, endrange, byte_slice));
    //Serial.write(getByte(program_counter, endrange, byte_slice),5);
    //Serial.write(getByte(0,9,byte_slice),10);
    //Serial.println("**----------------");
    //dataspace segment offset which stores all the variables
    //int ds_segment = program_counter;
    //SRAM*/
   

}

byte* getLongInstruction(){return NULL;}
byte* getShortInstruction(){return NULL;}
byte* getClumpRecords(){return NULL;}
void executeCodespace(){}

void getDSTOCValue(int index){

}

void printArray(byte * array){
   for (int x = 0; x < sizeof(array); x++) {
        Serial.println(array[x], HEX); 
    }
}

/**
* Set a single byte at he end of free RAM
**/
void setByte(long data){
  
}

/**
* Access any byte in the Bytecode Set by its offset from the top of the file.
* Offsets begin at zero(0)
**/
byte getByte(long offset){
  //number of columns in the byte layout
  int cols = 10;
  
  //find the row with the requested byte
  long row = (offset/cols);
  
  //find the column with the requested byte
  int col = offset%cols;
  
  //access the byte co-ordinates
  return bytecode[row][col];
  //return flash[row][col];
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
  }else if(offset_end<=offset_start){
    //offset_end = offset_start + 1;
    byte_range = 1;
  }
   
 //Create an array to store output byte range   
 //byte byte_slice[byte_range];
 //Serial.println("*******");
 //Serial.println(byte_range);
 //Loop through the bytecode and extract the requested slice
 for(long y=0; y<=byte_range; y++){ 
   byte_slice[y] = getByte(y+offset_start);
   //Serial.println(byte_slice[y], HEX);
 }
 
 /** 
 * Return an array with the requested slice which is the
 * length of the range not greater than 47 or less than 1
  **/ 
 return byte_slice;
}
