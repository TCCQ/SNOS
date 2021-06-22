/* SNOS 
 * author: Thomas Ulmer 
 *
 * This file contains the implimentations of the addressing styles declared in optcodes.h
 * implied, accumulator, and stack are not listed, as they would not return an address 
 *  in the same way that the other modes will.
 * Those modes will be implimented either in the instruction function or in the optcode function 
 */

#include "optcode.h"


//TODO CONFIRM THAT ALL REGS ARE CORRECT SIZE 

ADDRESS immediate (void) {//# 
  //TODO make 8b/16b switch 
  return (((ADDRESS)PB << 16) | PC) + 1;
}

//check that bitshift without cast is safe 
ADDRESS absolute (BYTE i, BYTE j) { //a
  return (DB << 16) | (j << 8) | (i);
}

ADDRESS absoluteLong (BYTE i, BYTE j, BYTE k) { //al
  return (k << 16) | (j << 8) | (i);
}

ADDRESS direct (BYTE i) { //d
  return (((ADDRESS)D) + i) & 0xFFFF;
}

ADDRESS directIndirectIndexed (BYTE i) { //(d),y
  return (((((ADDRESS)D) + i) & 0xFFFF ) | (DB << 16) ) + Y;
}

ADDRESS directIndirectIndexedLong (BYTE i) { //[d],y
  return ((((ADDRESS)D) + i) & 0xFFFF ) + Y;
}

ADDRESS directIndexedIndirect (BYTE i) { //(d,x)
  return (((((ADDRESS)D) + i) + X) & 0xFFFF ) | (DB << 16);
}

ADDRESS directIndexedWithX (BYTE i) { //d,x
  return ((((ADDRESS)D) + i) + X) & 0xFFFF;
}

ADDRESS directIndexedWithY (BYTE i) { //d,y
  return ((((ADDRESS)D) + i) + Y) & 0xFFFF;
}

ADDRESS absoluteIndexedWithX (BYTE i, BYTE j) { //a,x
  return ((((ADDRESS)D) << 16) | (j << 8) | i) + X;
}

ADDRESS absoluteIndexedWithY (BYTE i, BYTE j) { //a,y
  return ((((ADDRESS)D) << 16) | (j << 8) | i) + Y;
}

ADDRESS absoluteLongIndexedWithX (BYTE i, BYTE j, BYTE k) { //al,x
  return ((((ADDRESS)k) << 16) | (j << 8) | i) + X;
}

//confirm off by one error. PC inc happens BEFORE this 
ADDRESS programCounterRelative (BYTE i) { //r
  return (ADDRESS)(PC + ((signed)i)); //cast later to avoid changing 0x00FF0000 byte 
}

//again, AFTER inc.ed PC 
ADDRESS programCounterRelativeLong (BYTE i, BYTE j) { //rl
  return (ADDRESS)(PC + (((WORD)j) << 8) + i); //this one is not signed 
}

//TODO confirm how this works. see reference txt mode 17. Check another source 
ADDRESS absoluteIndirect (BYTE i, BYTE j) { //(a)
  return  *((ADDRESS*)(&memory[(((ADDRESS)j) << 16 ) | i]) ) & 0xFFFFFF; 
  //deref passed ptr in bank 00, take 24 bits from that location 
}

//same as above, confirm 
ADDRESS directIndirect (BYTE i) { //(d)
  return (*((ADDRESS*)(&memory[(((ADDRESS)D) + i) & 0xFFFF]) ) & 0xFFFF) | (((ADDRESS)DB) << 16); 
  //construct ptr in bank 0, deref, take 16b and return in DB
}

//TODO confirm how this is different from absoluteIndirect 
//I *think* that it is directIndirect but without DB, but I am not sure 
ADDRESS directIndirectLong (BYTE i) { //[d]
  return *((ADDRESS*)(&memory[(((ADDRESS)D) + i) & 0xFFFF]) ) & 0xFFFF; 
  //same as above but in bank 0
}

ADDRESS absoluteIndexedIndirect (BYTE i, BYTE j) { //(a,x)
  return (((((ADDRESS)j) << 8) | i ) + X ) & 0xFFFF;
}

ADDRESS stackRelative (BYTE i) { //d,s
  return (((ADDRESS)S) + i) & 0xFFFF; 
  //i is unsigned, can only reference things above the stack. Which direction does the SNES stack grow?
  //TODO check stack conventions for SNES 
}

ADDRESS stackRelativeIndirectIndexed (BYTE i) { //(d,s),y
  return (((((ADDRESS)S) + i) & 0xFFFF) | (((ADDRESS)DB) << 16)) + Y;
}

ADDRESS[2] blockSourceBankDestinationBank (BYTE i, BYTE j) { //xyc
  ADDRESS out[2];
  out[1] = ((ADDRESS)X) | (((ADDRESS)j) << 16); //source 
  //NEEDS TO ASSIGN TO DB, should I do that here? 
  DB = i;
  out[0] = ((ADDRESS)Y) | (((ADDRESS)DB) << 16); //destination 
  return out;
}
