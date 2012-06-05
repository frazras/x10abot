#include <Flash.h>

  // put your setup code here, to run once:
 char my_bytecode[]="4D696E6473746F726D734E5854000005030016000C000F0005000A00000000000C00010007000601000006000400010008008813000010C000A000100FFFFFFFFFF0000000000800000010002002A60FFFFFFF0F";
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

byte SRAM[10][50];

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

int dsh_clump_count;
int dsh_code_word_count;


void setup() {
 // initialize serial:
  Serial.begin(9600);
 
  //Print "Mindstorms" as the first to bytes
  Serial.println("Byte below:");
  Serial.write(getByte(0,9),10);
  Serial.println("");
  
  //Print the Letter 's', last letter of Mindstorms
  Serial.write(getByte(9));
  
  checkVersion();
}

void loop() {
  // put your main code here, to run repeatedly: 
}

/**
* Get the LEGO Mindstorms version CODE
**/
byte* checkVersion(){
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
}


void  initialiseDataspace(){
    long pc = program_counter;
    int dstoc_type[]={0,8,8,16,16,32,0,0,0};
 
    //4 bytes per DSTOC entry
    program_counter += (dsh_dstoc_count*4);
    
    //dataspace segment offset which stores all the variables
    //int ds_segment = program_counter;
    
   

}

byte* getLongInstruction(){return NULL;}
byte* getShortInstruction(){return NULL;}
byte* getClumpRecords(){return NULL;}
void executeCodespace(){}

void getDSTOCValue(int index){

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
}

/**
* Access any contiguous set of bytes in the Bytecode Set by its offset range
* begin and end points(inclusive) relative to the top of the file.
* Offsets begin at zero(0)
**/
byte* getByte(long offset_start, long offset_end){
  
  //The number of bytes in the range
  int byte_range = offset_end - offset_start;
  
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
 byte byte_slice[byte_range];
 
 //Loop through the bytecode and extract the requested slice
 for(long y=0; y<=byte_range; y++){ 
   byte_slice[y] = getByte(y+offset_start);
 }
 
 /** 
 * Return an array with the requested slice which is the
 * length of the range not greater than 47 or less than 1
  **/ 
 return byte_slice;
}
