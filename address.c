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
 * This means there is an inherent conditional. 
 * This seems ineffiecient, but I can't see any way around it
 */
//as far as I can tell, all memory calls in all other files should be NoWrap variants.

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
  return (((ADDRESS)DB << 16) | getWordNoWrap(direct(i))) + (getX())? *YL:Y;
}

ADDRESS directIndirectIndexedLong (BYTE i) { //[d],y
  return getAddressNoWrap(direct(i)) + (getX())?*YL:y;
}

ADDRESS directIndexedIndirect (BYTE i) { //(d,x)
  return getWordNoWrap(directIndexedWithX(i)) | ((ADDRESS)DB << 16);
}

ADDRESS directIndexedWithX (BYTE i) { //d,x
  return ((direct(i) + (getX())?*XL:X)) & 0xFFFF;
}

ADDRESS directIndexedWithY (BYTE i) { //d,y
  return ((direct(i) + (getX())?*YL:Y)) & 0xFFFF;
}

ADDRESS absoluteIndexedWithX (BYTE i, BYTE j) { //a,x
  return ((((ADDRESS)DB) << 16) | (j << 8) | i) + ((getX())? *XL:X);
}

ADDRESS absoluteIndexedWithY (BYTE i, BYTE j) { //a,y
  return ((((ADDRESS)D) << 16) | (j << 8) | i) + ((getX())? *YL:Y);
}

ADDRESS absoluteLongIndexedWithX (BYTE i, BYTE j, BYTE k) { //al,x
  return (absoluteLong(i,j,k)) + ((getX())? *XL:X);
}

//PC inc happens BEFORE this 
ADDRESS programCounterRelative (BYTE i) { //r
  return (ADDRESS)(PC + ((signed)i)) & 0xFFFF; //cast later to avoid changing 0x00FF0000 byte 
}

//again, AFTER inc.ed PC 
ADDRESS programCounterRelativeLong (BYTE i, BYTE j) { //rl
  return (ADDRESS)(PC + (((WORD)j) << 8) + i) & 0xFFFF; //this one is not signed 
}

//full 24b address, will be split into PB and PC by caller, see JML
//two byte addr in bank 00, then deref word + PB, used for jumps?
ADDRESS absoluteIndirect (BYTE i, BYTE j) { //(a)
  return (ADDRESS)getWordNoWrap(((ADDRESS)j << 8 ) | i) | ((ADDRESS)PB << 16);
}

ADDRESS directIndirect (BYTE i) { //(d)
  return (ADDRESS)getWordNoWrap(direct(i)) | ((ADDRESS)DB << 16) 
}

//this is above but but pull all 3 bytes, not just 2 + DB
ADDRESS directIndirectLong (BYTE i) { //[d]
  return getAddressNoWrap(direct(i));
}

//this returns an 16b value to placing in PC
//derefs a bank 00 location for 16b
//adding X wraps to bank
ADDRESS absoluteIndexedIndirect (BYTE i, BYTE j) { //(a,x)
  return (ADDRESS)getWordWrap(((((ADDRESS)j << 8) | i) + (getX())? *XL:X) & 0xFFFF);
}

//returns 16b (stack is in bank 00) location above the current stack pointer
ADDRESS stackRelative (BYTE i) { //d,s
  return ((ADDRESS)S + i) & 0xFFFF; 
  //i is unsigned, can only reference things above the stack ptr. 
}

//defref stack relative for 16b + DB + Y
ADDRESS stackRelativeIndirectIndexed (BYTE i) { //(d,s),y
  return (((ADDRESS)DB << 16 ) | getWord(stackRelative(i)) ) + (getX())? *YL:Y;
}

