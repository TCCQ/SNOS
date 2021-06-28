/* SNOS 
 * author: Thomas Ulmer 
 *
 * This file contains the implimentations of the 93 instructions. 
 * Most opt-codes will merely call the relevant instruction function with the proper addressing style. 
 */

//registers are included in this header 
#include "optcodes.h"
#include <string.h> //need for memcpy 

//reference for P flag register 
//N V M X/B D I Z C E 

//utility functions 

static inline BYTE getN() {
  return (P & 0x80) >> 7;
}

static inline void setN(BYTE i) {
  P &= ~0x80;
  P |= (i << 7);
}

static inline BYTE getV() {
  return (P & 0x40) >> 6; 
}

static inline void setV (BYTE i) {
  P &= ~0x40;
  P |= (i << 6);
}

static inline BYTE getM() {
  return (P & 0x20) >> 5;
}

static inline BYTE setM (BYTE i) {
  P &= ~0x20;
  P |= (i << 5);
}

static inline BYTE getX() {
  return (P & 0x10) >> 4;
}

static inline void setX (BYTE i) {
  P &= ~0x10;
  P |= (i << 4);
}

static inline BYTE getD() {
  return (P & 0x80) >> 3;
}

static inline void setD (BYTE i) {
  P &= ~0x08;
  P |= (i << 3);
}

static inline BYTE getI() {
  return (P & 0x04) >> 2;
}

static inline void getI (BYTE i) {
  P &= ~0x04;
  P |= (i << 2);
}

static inline BYTE getZ() {
  return (P & 0x02) >> 1;
}

static inline void setZ (BYTE i) {
  P &= ~0x02;
  P |= (i << 1);
}

static inline BYTE getC() {
  return P & 0x01;
}

static inline void setC() {
  P &= ~0x01;
  P |= (i);
}


//ignoring emulation mode for now 
void pushByte (BYTE i) {
  memory[map[S]] = i;
  S -= 1;
}

BYTE popByte () {
  return memory[map[++S]];
}

void pushWord (WORD i) {
  memory[map[S-1]] = i & 0xFF;
  memory[map[S]] = (i & 0xFF00) >> 8; 
  S -= 2;
}

WORD popWord() {
  WORD out = 0;
  out |= memory[map[--S]];
  out |= ((WORD)memory[map[--S]]) << 8;
  return out;
}

/*
 * NOTE ABOUT IMMEDIATE ADDRESSING
 * some instructions take either an addressed memory location or a constant,
 * I will be handling the constant variations on the optcode level, just do the memory versions here 
 */

/* 
 * NOTE ABOUT ACCUMULATOR ADDRESSING 
 * some instructions take either a mem. location or operate on the A register.
 * this file will contain the memory location implimentations, and the A reg. version will be 
 *  hardcoded in the optcode function 
 */

/* NOTE ABOUT THE PROGRAM COUNTER 
 * the program counter points to the NEXT instruction,
 * so barring a jump, PC in incremented BEFORE the current optcode is executed 
 * a jump is just setting the PC/PB accordingly.
 */


//TODO software side interupts, remember to push PB and then PC 

//TODO add cycle counts. (maybe do on optcode and not on instruction?)

//TODO set flags (use util functions) 

void ADC (ADDRESS i) { //Add Memory to Accumulator with Carry
  if (getM()) {
    A += memory[map[i]] + (getC())? 1:0;
  } else {
    A += (((WORD)memory[map[i+1]]) << 8 | memory[map[i]]) + (getC())? 1:0;
  }
}

void AND (ADDRESS i) { //"AND" Memory with Accumulator
  *AL &= memory[map[i]];
  *AH &= ~(getM()-1) | memory[map[i+1]]; //top byte if M = 0
}

//do memory only 
//size of shifted area depends on M flag 
void ASL (ADDRESS i) { //Shift One Bit Left, Memory or Accumulator
  if (getM()) {
    memory[map[i]] = memory[map[i]] >> 1;
  } else {
    WORD o = (((WORD)memory[map[i+1]] << 8) | memory[map[i]]);
    o = o >> 1;
    memory[map[i]] = o & 0xFF;
    memory[map[i+1]] = (o & 0xFF00) >> 8;
  }
}

/* NOTE ABOUT BRANCHES
 * branch instructions **CANNOT** move across banks 
 * changes ONLY PC, and not PB
 * code can be in any bank, but must be moved to with a full jump.
 */
void BCC (ADDRESS i) { //Branch on Carry Clear (Pe = O)
  if (!getC()) {
    PC = i & 0xFFFF; 
  }
}

void BCS (ADDRESS i) { //Branch on Carry Set (Pe = 1)
  if (getC()) {
    PC = i & 0xFFFF; 
  }
}

void BEQ (ADDRESS i) { //Branch if Equal (Pz = 1)
  if (getZ()) {
    PC = i & 0xFFFF; 
  }
}

//TODO 
void BIT (ADDRESS i) { //Bit Test
  //return to when doing flags 
}

void BMI (ADDRESS i) { //Branch if Result Minus (PN = 1)
  if (getN()) {
    PC = i & 0xFFFF;
  }
}

void BNE (ADDRESS i) { //Branch if Not Equal (Pz = 0)
  if (!getZ()) {
    PC = i & 0xFFFF;
  }
}

void BPL (ADDRESS i) { //Branch if Result Plus (PN = 0)
  if (!getN()) {
    PC = i & 0xFFFF;
  }
}

//basically just an in-bank jump 
void BRA (ADDRESS i) { //Branch Always
  PC = i & 0xFFFF 
}

//implied addr 
void BRK (void) { //Force Break         
  //vector is $FFE6-$FFE7 in native mode 
}

//confirm that this does not change PB 
void BRL (ADDRESS i) { //Branch Always Long
  PC = i & 0xFFFF;
}

void BVC (ADDRESS i) { //Branch on Overflow Clear (Pv = 0)
  if (!getV()) {
    PC = i & 0xFFFF;
  }
}

void BVS (ADDRESS i) { //Branch on Overflow Set (Pv = 1)
  if (getV()) {
    PC = i & 0xFFFF;
  }
}

void CLC (void) { //Clear Carry Flag                       
  P &= 0xFE; //clear lowest bit 
}

void CLD (void) { //Clear Decimal Mode
  P &= 0xF7; //clear 4th lowest bit 
}

void CLI (void) { //Clear Interrupt Disable Bit
  P &= 0xFB; 
}

void CLV (void) { //Clear Overflow Flag
  P &= 0xBF; 
}

//TODO when doing flags 
void CMP (ADDRESS i) { //Compare Memory and Accumulator
  //flags 
}

void COP (ADDRESS i) { //Coprocessor                                
  //some vector 
}

void CPX (ADDRESS i) { //Compare Memory and Index X
  //flags 
}

void CPY (ADDRESS i) { //Compare Memory and Index Y
  //flags 
}

//follows M flag 
//memory only version 
void DEC (ADDRESS i) { //Decrement Memory or Accumulator by One  
  if (getM()) {
    memory[map[i]]--;
  } else {
    WORD o = (((WORD)memory[map[i+1]] << 8) | memory[map[i]]);
    o--;
    memory[map[i]] = o & 0xFF;
    memory[map[i+1]] = (o & 0xFF00) >> 8;
  }
}

void DEX (void) { //Decrement Index X by One               
  if (getX()) {
    *XL--;
  } else {
    X--;
  }
}

void DEY (void) { //Decrement Index Y by One
  if (getX()) {
    *YL--;
  } else {
    Y--;
  }
}

void EOR (ADDRESS i) { //Exclusive "OR" Memory with Accumulator 
  *AL ^= memory[map[i]];
  if (!getM()) *AH ^= memory[map[i+1]];
}

//see DEC 
void INC (ADDRESS i) { //Increment Memory or Accumulator by One     
  if (getM()) {
    memory[map[i]]++;
  } else {
    WORD o = (((WORD)memory[map[i+1]] << 8) | memory[map[i]]);
    o++;
    memory[map[i]] = o & 0xFF;
    memory[map[i+1]] = (o & 0xFF00) >> 8;
  }
}

void INX (void) { //Increment Index X by One
  if (getX()) {
    *XL++;
  } else {
    X++;
  }
}

void INY (void) { //Increment Index Y by One
  if (getX()) {
    *YL++;
  } else {
    Y++;
  }
}

//"the only instructions that affect the program bank register are: RTI, RTL, JML, JSL, JMP Absolute Long 
void JML (ADDRESS i) { //Jump Long
  PC = i; 
  PB = (i & 0xFF0000) >> 16;
}

void JMP (ADDRESS i) { //Jump to New Location
  PC = i;
}

void JSL (ADDRESS i) { //Jump Subroutine Long
  pushByte(PB);
  pushWord(PC);
  PC = i;
  PB = (i & 0xFF0000) >> 16;
}

void JSR (ADDRESS i) { //Jump to New Location Saving Return Address
  pushWord(PC);
  PC = i;
}

void LDA (ADDRESS i) { //Load Accumulator with Memory
  *AL = memory[map[i]];
  if (!getM()) *AH = memory[map[i+1]];
}

void LDX (ADDRESS i) { //Load Index X with Memory
  *XL = memory[map[i]];
  if (!getX()) *XH = memory[map[i+1]];
}

void LDY (ADDRESS i) { //Load Index Y with Memory                  
  *YL = memory[map[i]];
  if (!getX()) *YH = memory[map[i+1]];
}

//see ASL 
void LSR (ADDRESS i) { //Shift One Bit Right (Memory or Accumulator) 
  if (getM()) {
    memory[map[i]] = memory[map[i]] << 1;
  } else {
    WORD o = (((WORD)memory[map[i+1]] << 8) | memory[map[i]]);
    o = o << 1;
    memory[map[i]] = o & 0xFF;
    memory[map[i+1]] = (o & 0xFF00) >> 8;
  }
}

//uses full registers, basically must be in native m=0,x=0 mode. 
//I *think* it's safe not to check P flags for this 
void MVN (ADDRESS i[2]) { //Block Move Negative              !!! see xyc, block address
  while (A-- != 0xFFFF) {
    memory[map[i[1] | Y++]] = memory[map[i[0] | X++]];
  }
  //address are bottom of the block area 
}

//see above 
void MVP (ADDRESS i[2]) { //Block Move Positive
  while (A++ != 0xFFFF) {
    memory[map[i[1] | Y--]] = memory[map[i[0] | X--]];
  }
  //addresses are the top of the block area 
}

void NOP (void) { //No Operation                          
  //self explanatory 
}

void ORA (ADDRESS i) { //"OR" Memory with Accumulator 
  *AL |= memory[map[i]];
  if (!getM()) *AH |= memory[map[i+1]];
}

//TODO this is dummy, and will not be called. can be cleaned later 
//push 16b constant 
void PEA (ADDRESS i) { //Push Effective Absolute Address on Stack (or Push Immediate Data on Stack)
  pushWord(((WORD)memory[map[i+1]]) << 8 | memory[map[i]]);
}

//deref'd memory, (this one not an in code constant)
void PEI (ADDRESS i) { //Push Effective Indirect Address on Stack (add one cycle if DL f 0)
  pushWord(((WORD)memory[map[i+1]]) << 8 | memory[map[i]]);
}

//push address, not deref'd
void PER (ADDRESS i) { //Push Effective Program Counter Relative Address on Stack
  pushWord(((WORD)memory[map[i+1]]) << 8 | memory[map[i]]);
}

//these use size switches, confirm that that is intended.
void PHA (void) { //Push Accumulator on Stack
  if (getM()) {
    pushByte(*AL);
  } else {
    pushWord(A);
  }
}

void PHB (void) { //Push Data Bank Register on Stack
  pushByte(DB);
}

void PHD (void) { //Push Direct Register on Stack
  pushByte(D);
}

void PHK (void) { //Push Program Bank Register on Stack
  pushByte(PB);
}

void PHP (void) { //Push Processor Status on Stack
  pushByte(P);
}

void PHX (void) { //Push Index X on Stack
  if (getX()) {
    pushByte(*XL);
  } else {
    pushWord(X);
  }
}

void PHY (void) { //Push index Y on Stack
  if (getX()) {
    pushByte(*YL);
  } else {
    pushWord(Y);
  }
}

void PLA (void) { //Pull Accumulator from Stack
  if (getM()) {
    *AL = popByte();
  } else {
    A = popWord();
  }
}

void PLB (void) { //Pull Data Bank Register from Stack
  DB = popByte();
}

void PLD (void) { //Pull Direct Register from Stack
  D = popByte();
}

void PLP (void) { //Pull Processor Status from Stack
  P = popByte();
}

void PLX (vo id) { //Pull Index X from Stack
  if (getX()) {
    *XL = popByte();
  } else {
    X = popWord();
  }
}

void PLY (void) { //Pull Index Y form Stack
  if (getX()) {
    *YL = popByte();
  } else {
    Y = popWord();
  }
}

void REP (ADDRESS i) { //Reset Status Bits                         
  P &= ~(memory[map[i]]); //always 1 byte, even when regs. are 16b
}

//follow M flag for size, rotated bit is carry. 
void ROL (ADDRESS i) { //Rotate One Bit Left (Memory or Accumulator)     
  if (getM()) {
    memory[map[i]] = ((memory[map[i]] >> 1) & 0x8F ) | (getC() << 7);
  } else {
    WORD o = (WORD)memory[map[i+1]] << 8 | memory[map[i]];
    o = ((o >> 1) & 0x8F) | (getC() << 7);
    memory[map[i]] = o & 0xFF;
    memory[map[i+1]] = (o & 0xFF00) >> 8;
  }
}

//see above 
void ROR (ADDRESS i) { //Rotate One Bit Right (Memory or Accumulator)
  if (getM()) {
    memory[map[i]] = ((memory[map[i]] << 1) & 0xFE ) | getC();
  } else {
    WORD o = (WORD)memory[map[i+1]] << 8 | memory[map[i]];
    o = ((o << 1) & 0xFE) | getC();
    memory[map[i]] = o & 0xFF;
    memory[map[i+1]] = (o & 0xFF00) >> 8;
  }
}

//next three, caller convention is push PC before jmp
//so to return, just pop PC
void RTI (void) { //Return from Interrupt                     
  PC = popWord();
  PB = popByte(); //not sure if this should be here? but I think so, see JML 
}

void RTL (void) { //Return from Subroutine Long
  PC = popWord();
  PB = popByte(); 
}

void RTS (void) { //Return from Subroutine
  PC = popWord();
}

//follows M flag 
void SBC (ADDRESS i) { //Subtract Memory from Accumulator with Borrow
  if (getM()) {
    *AL = *AL - memory[map[i]] - (getC())? 0:1;
  } else {
    A = A - (((WORD)memory[map[i+1]]) << 8 | memory[map[i]]) - (getC())? 0:1; 
  }
}

void SEC (void) { //Set Carry Flag                        
  P |= 0x01;
}

void SED (void) { //Set Decimal Mode
  P |= 0x08;
}

void SEI (void) { //Set Interrupt Disable Status
  P |= 0x04;
}

//takes constant 
//opposite of clear P bits I think?
void SEP (ADDRESS i) { //Set Processor Status Bits              
  P |= memory[map[i]];
}

void STA (ADDRESS i) { //Store Accumulator in Memory
  memory[map[i]] = *AL;
  if (!getM()) {
    memory[map[i+1]] = *AH;
  }
}

//TODO clock interaction 
void STP (void) { //Stop the Clock

}

void STX (ADDRESS i) { //Store Index X in Memory
  memory[map[i]] = *XL;
  if (!getX()) {
    memory[map[i+1]] = *XH;
  }
}

void STY (ADDRESS i) { //Store Index Y in Memory
  memory[map[i]] = *YL;
  if (!getX()) {
    memory[map[i+1]] = *YH;
  }
}

void STZ (ADDRESS i) { //Store Zero in Memory
  memory[map[i]] = 0;
}

void TAX (void) { //Transfer Accumulator to Index X
  X = A;
}

void TAY (void) { //Transfer Accumulator to Index Y
  Y = A;
}

void TCD (void) { //Transfer Accumulator to Direct Register
  D = A;
}

void TCS (void) { //Transfer Accumulator to Stack Pointer Register
  S = A;
}

void TDC (void) { //Transfer Direct Register to Accumulator
  A = D
}

//TODO 
//flags I guess? not sure what this does 
//and memory and A, reset affected bits in memory, set Z  
void TRB (ADDRESS i) { //Test and Reset Bit
  
}

//see above, but set bits in memory 
void TSB (ADDRESS i) { //Test and Set Bit

}

void TSC (void) { //Transfer Stack Pointer Register to Accumulator
  A = S;
}

void TSX (void) { //Transfer Stack Pointer Register to Index X
  X = S;
}

void TXA (void) { //Transfer Index X to Accumulator
  A = X;
}

void TXS (void) { //Transfer Index X to Stack Polnter Register
  S = X;
}

void TXY (void) { //Transfer Index X to Index Y
  Y = X;
}

void TYA (void) { //Transfer Index Y to Accumulator
  A = Y;
}

void TYX (void) { //Transfer Index Y to Index X
  X = Y;
}

//TODO waiting 
void WAI (void) { //Wait for Interrupt
  
}

void XBA (void) { //Exchange AH and AL
  BYTE tmp = AH;
  AH = AL;
  AL = tmp;
}

void XCE (void) { //Exchange Carry and Emulation Bits      
  BYTE tmp = P & 0x01;
  P &= 0xFE;
  P |= E;
  E = tmp;
}

