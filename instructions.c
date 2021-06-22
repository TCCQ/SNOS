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


/*
 * NOTE ABOUT IMMEDIATE ADDRESSING
 * some instructions take either an addressed memory location or a constant,
 * so even constants will be passed as an address 
 * TODO wait is immediate already an offset from PC? am I using these terms wrong? That seems likely 
 * I am just gonna use the word "constant" cause it doesn't have ambiguity 
 */

/* 
 * NOTE ABOUT ACCUMULATOR ADDRESSING 
 * some instructions take either a mem. location or operate on the A register.
 * this file will contain the memory location implimentations, and the A reg. version will be 
 *  hardcoded in the optcode function 
 */

//utility func 
inline WORD* tow (ADDRESS i) {
  return (WORD*)(memory + i); 
}

//TODO 8b/16b switches for registers 
//TODO 8b/16b constants 

//TODO add cycle counts. (maybe do on optcode and not on instruction?)

//TODO set flags 

void ADC (ADDRESS i) { //Add Memory to Accumulator with Carry
  WORD* m = tow(i);
  A = A + *m + 1;
}

void AND (ADDRESS i) { //"AND" Memory with Accumulator
  A &= *(tow(i));
}

//for things that are mem or accu, I am just gonna write the accu side in the optcode, and do memory here. 
void ASL (ADDRESS i) { //Shift One Bit Left, Memory or Accumulator
  WORD* ptr = tow(i);
  *ptr = (*ptr) << 1; //right kind of shift? 
}

void BCC (ADDRESS i) { //Branch on Carry Clear (Pe = O)
  
}

void BCS (ADDRESS i) { //Branch on Carry Set (Pe = 1)

}

void BEQ (ADDRESS i) { //Branch if Equal (Pz = 1)

}

void BIT (ADDRESS i) { //Bit Test

}

void BMI (ADDRESS i) { //Branch if Result Minus (PN = 1)

}

void BNE (ADDRESS i) { //Branch if Not Equal (Pz = 0)

}

void BPL (ADDRESS i) { //Branch if Result Plus (PN = 0)

}

void BRA (ADDRESS i) { //Branch Always

}

void BRK (ADDRESS i) { //Force Break                         !!!!!!!!! not sure if this takes an address or not
  //TODO find the correct vector to jump to 
}

void BRL (ADDRESS i) { //Branch Always Long

}

void BVC (ADDRESS i) { //Branch on Overflow Clear (Pv = 0)

}

void BVS (ADDRESS i) { //Branch on Overflow Set (Pv = 1)

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

void DEC (ADDRESS i) { //Decrement Memory or Accumulator by One  
  WORD* ptr = tow(i);
  *ptr -= 1;
}

void DEX (void) { //Decrement Index X by One               
  X--;
}

void DEY (void) { //Decrement Index Y by One
  Y--;
}

void EOR (ADDRESS i) { //Exclusive "OR" Memory with Accumulator 
  A ^= *(tow(i));
}

//as above, only memory is implimented here 
void INC (ADDRESS i) { //Increment Memory or Accumulator by One     
  WORD* ptr = tow(i);
  *ptr += 1;
}

void INX (void) { //Increment Index X by One
  X++;
}

void INY (void) { //Increment Index Y by One
  Y++;
}

//TODO check this for off by one error. depends on WHEN PC gets incremented. 
void JML (ADDRESS i) { //Jump Long
  PC = i; 
}

void JMP (ADDRESS i) { //Jump to New Location
  PC = i;
}

//TODO check stack format 
void JSL (ADDRESS i) { //Jump Subroutine Long
  //save address on the stack 
}

void JSR (ADDRESS i) { //Jump to New Location Saving Return Address
  //same as above 
}

void LDA (ADDRESS i) { //Load Accumulator with Memory
  A = *(tow(i));
}

void LDX (ADDRESS i) { //Load Index X with Memory
  X = *(tow(i));
}

void LDY (ADDRESS i) { //Load Index Y with Memory                  
  Y = *(tow(i));
}

void LSR (ADDRESS i) { //Shift One Bit Right (Memory or Accumulator) 
  WORD* ptr = tow(i);
  *ptr = (*ptr) >> 1; //right kind of shift? 
}

void MVN (ADDRESS i[2]) { //Block Move Negative              !!! see xyc, block address
  memcpy(memory + i[1], memory + i[0], A+1);
  X += A;
  Y += A;
  A = 0xFFFF; 
  //address are bottom of the block area 
}

void MVP (ADDRESS i[2]) { //Block Move Positive
  memcpy(memory + i[1] - A+1, memory + i[0] + A+1, A+1);
  X -= A;
  Y -= A;
  A = 0xFFFF;
  //addresses are the top of the block area 
}

void NOP (void) { //No Operation                          
  //self explanatory 
}

void ORA (ADDRESS i) { //"OR" Memory with Accumulator 
  A |= *(tow(i));
}

//TODO stack stuff 
void PEA (ADDRESS i) { //Push Effective Absolute Address on Stack (or Push Immediate Data on Stack)
  
}

void PEI (ADDRESS i) { //Push Effective Indirect Address on Stack (add one cycle if DL f 0)

}

void PER (ADDRESS i) { //Push Effective Program Counter Relative Address on Stack

}

void PHA (void) { //Push Accumulator on Stack

}

void PHB (void) { //Push Data Bank Register on Stack

}

void PHD (void) { //Push Direct Register on Stack

}

void PHK (void) { //Push Program Bank Register on Stack

}

void PHP (void) { //Push Processor Status on Stack

}

void PHX (void) { //Push Index X on Stack

}

void PHY (void) { //Push index Y on Stack

}

void PLA (void) { //Pull Accumulator from Stack

}

void PLB (void) { //Pull Data Bank Register from Stack

}

void PLD (void) { //Pull Direct Register from Stack

}

void PLP (void) { //Pull Processor Status from Stack

}

void PLX (void) { //Pull Index X from Stack

}

void PLY (void) { //Pull Index Y form Stack

}

void REP (ADDRESS i) { //Reset Status Bits                         
  P &= ~(memory[i]); //always 1 byte, even when regs. are 16b
}

void ROL (ADDRESS i) { //Rotate One Bit Left (Memory or Accumulator)     
  WORD* ptr = tow(i);
  *ptr = *ptr << 1;
  *ptr |= (P & 0x01); //carry flag 
}

void ROR (ADDRESS i) { //Rotate One Bit Right (Memory or Accumulator)
  WORD* ptr = tow(i);
  *ptr = *ptr >> 1;
  *ptr |= (P & 0x01); //carry flag 
}

//uses stack return addr I think 
void RTI (void) { //Return from Interrupt                     

}

void RTL (void) { //Return from Subroutine Long

}

void RTS (void) { //Return from Subroutine

}

//constants? ??? return to. 
//need both a immediate and address mode 
//  solve with # addr function. 
void SBC (ADDRESS i) { //Subtract Memory from Accumulator with Borrow
  
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
void SEP (ADDRESS i) { //Set Processor Status Bits              
  
}

void STA (ADDRESS i) { //Store Accumulator in Memory
  *(tow(i)) = A;
}

//TODO clock interaction 
void STP (void) { //Stop the Clock

}

void STX (ADDRESS i) { //Store Index X in Memory
  *(tow(i)) = x;
}

void STY (ADDRESS i) { //Store Index Y in Memory
  *(tow(i)) = Y;
}

void STZ (ADDRESS i) { //Store Zero in Memory
  *(tow(i)) = 0;
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

//flags I guess? not sure what this does 
void TRB (ADDRESS i) { //Test and Reset Bit
  
}

//see above 
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

