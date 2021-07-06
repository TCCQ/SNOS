/* SNOS 
 * author: Thomas Ulmer 
 *
 * This file contains the implimentations of the addressing styles declared in optcodes.h
 * implied, accumulator, and stack are not listed, as they would not return an address 
 *  in the same way that the other modes will.
 * Those modes will be implimented either in the instruction function or in the optcode function 
 */

#include "address.h"

//boolean for size of index X/Y reg.s 1 if 8b, 0 if 16b
static inline BYTE getX() {
  return P & 0x10;
}

/* NOTE ABOUT REGISTER SIZE 
 *
 * All of the addressing modes that use X/Y registers must check 
 * 8b or 16b current size 
 * This means there is an inherent branch operation. 
 * This seems ineffiecient, but I can't see any way around it
 */

ADDRESS absolute (BYTE i, BYTE j) { //a
  return ((ADDRESS)DB << 16) | (j << 8) | (i);
}

ADDRESS absoluteLong (BYTE i, BYTE j, BYTE k) { //al
  return ((ADDRESS)k << 16) | (j << 8) | (i);
}

ADDRESS direct (BYTE i) { //d
  return (((ADDRESS)D) + i) & 0xFFFF;
}

ADDRESS directIndirectIndexed (BYTE i) { //(d),y
  return (((((ADDRESS)D) + i) & 0xFFFF ) | (DB << 16) ) + ((getX())? (Y & 0xFF):(Y)); //8b/16b switch.
}

ADDRESS directIndirectIndexedLong (BYTE i) { //[d],y
  return ((((ADDRESS)D) + i) & 0xFFFF ) + ((getX())? (Y & 0xFF):(Y));
}

ADDRESS directIndexedIndirect (BYTE i) { //(d,x)
  return (((((ADDRESS)D) + i) + ((getX())? (X & 0xFF):(X)) )  & 0xFFFF ) | (DB << 16);
}

ADDRESS directIndexedWithX (BYTE i) { //d,x
  return ((((ADDRESS)D) + i) + ((getX())? (X & 0xFF):(X))) & 0xFFFF;
}

ADDRESS directIndexedWithY (BYTE i) { //d,y
  return ((((ADDRESS)D) + i) + ((getX())? (Y & 0xFF):(Y))) & 0xFFFF;
}

ADDRESS absoluteIndexedWithX (BYTE i, BYTE j) { //a,x
  return ((((ADDRESS)D) << 16) | (j << 8) | i) + ((getX())? (X & 0xFF):(X));
}

ADDRESS absoluteIndexedWithY (BYTE i, BYTE j) { //a,y
  return ((((ADDRESS)D) << 16) | (j << 8) | i) + ((getX())? (Y & 0xFF):(Y));
}

ADDRESS absoluteLongIndexedWithX (BYTE i, BYTE j, BYTE k) { //al,x
  return ((((ADDRESS)k) << 16) | (j << 8) | i) + ((getX())? (X & 0xFF):(X));
}

//PC inc happens BEFORE this 
ADDRESS programCounterRelative (BYTE i) { //r
  return (ADDRESS)(PC + ((signed)i)); //cast later to avoid changing 0x00FF0000 byte 
}

//again, AFTER inc.ed PC 
ADDRESS programCounterRelativeLong (BYTE i, BYTE j) { //rl
  return (ADDRESS)(PC + (((WORD)j) << 8) + i); //this one is not signed 
}

ADDRESS absoluteIndirect (BYTE i, BYTE j) { //(a)
  return  *((ADDRESS*)(&memory[(((ADDRESS)j) << 16 ) | i]) ) & 0xFFFFFF; 
  //deref passed ptr in bank 00, take 24 bits from that location 
}

ADDRESS directIndirect (BYTE i) { //(d)
  return (*((ADDRESS*)(&memory[(((ADDRESS)D) + i) & 0xFFFF]) ) & 0xFFFF) | (((ADDRESS)DB) << 16); 
  //construct ptr in bank 0, deref, take 16b and return in DB
}

//this is above but but pull all 3 bytes, not just 2 + DB
ADDRESS directIndirectLong (BYTE i) { //[d]
  return *((ADDRESS*)(&memory[(((ADDRESS)D) + i) & 0xFFFF]) ) & 0xFFFFFF; 
}

//TODO confirm if I need to deref this location. see mode 20
ADDRESS absoluteIndexedIndirect (BYTE i, BYTE j) { //(a,x)
  return *((ADDRESS*)&memory[(((((ADDRESS)j) << 8) | i ) & 0xFFFF + ((getX())? (X & 0xFF):(X)) ) & 0xFFFF]) & 0xFFFF;
}

//TODO check stack conventions for SNES 
ADDRESS stackRelative (BYTE i) { //d,s
  return (((ADDRESS)S) + i) & 0xFFFF; 
  //i is unsigned, can only reference things above the stack ptr. 
}

//TODO confirm deref? 
ADDRESS stackRelativeIndirectIndexed (BYTE i) { //(d,s),y
  return ((*((ADDRESS*)&memory[(((ADDRESS)S) + i) & 0xFFFF]) & 0xFFFF) | (((ADDRESS)DB) << 16)) + ((getX())? (Y & 0xFF):(Y));
}

//TODO confirm DB assignment 
ADDRESS[2] blockSourceBankDestinationBank (BYTE i, BYTE j) { //xyc
  ADDRESS out[2];
  out[1] = ((ADDRESS)((getX())? (X & 0xFF):(X))) | (((ADDRESS)j) << 16); //source 
  //NEEDS TO ASSIGN TO DB, should I do that here? 
  DB = i;
  out[0] = ((ADDRESS)((getX())? (Y & 0xFF):(Y))) | (((ADDRESS)DB) << 16); //destination 
  return out;
}
