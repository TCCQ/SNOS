/* SNOS 
 * author: Thomas Ulmer 
 *
 * This file contains the implimentations of the 93 instructions. 
 * Most opt-codes will merely call the relevant instruction function with the proper addressing style. 
 */

//registers are included in this header 
#include "optcodes.h"
#include "memory.h"

//reference for P flag register 
//N V M X/B D I Z C E 
//
//useful links
//https://apprize.best/programming/65816/21.html


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

//TODO add cycle counts. (maybe do on optcode and not on instruction?)

//I believe I have fixed which jmps set PB, but could always use a double check 

/* NOTE ABOUT FLAGS AND ARITHMATIC
 * ok so a few things 
 * carry is set if bit 7 overflows into what would be bit 8 or underflow from bit 0
 * zero is easy
 * overflow flag is similar to carry, but for bit 7 ASSUME THE INPUTS ARE SIGNED, then if there is overflow from bit 6 into 7, set might also need to see if carry is set 
 * negative is set if bit 7 is set. thats it
 *
 * IMPORTANT!!!
 * decimal is more complicated
 * if D is set, then use BCD not regular binary. This seems like a collosal pain, so its on the backburner for now
 * D flag is only checked on ADC and SBC (add/subtract w/ carry)
 */

void ADC (ADDRESS i) { //Add Memory to Accumulator with Carry
  if (getD()) { //BCD addition
    if (getM()) {
      BYTE m = getByte(i);
      BYTE tmp = (*AL & 0x0F) + (((*AL & 0xF0) >> 4)*10);
      tmp += (m & 0x0F) + (((m & 0xF0) >> 4)*10);
      tmp += (getC())? 1:0;

      //value is set
      setN(0);
      setZ(0);
      setV(0);
      setC(0);
      if (tmp & 0x80) setN(1);
      if (!tmp) setZ(1);
      if (tmp > 99) setC(1);
      if (tmp & 0x80) setV(1); //TODO I guess it follows N?
      //use m as scratch here
      m = tmp % 10; //bottom 4 bits
      tmp = (tmp / 10) %10; //high 4b
      tmp <<= 4;
      tmp |= m;
      *AL = tmp;
    } else { //16b
      WORD m = getWord(i);
      WORD tmp = (A & 0x000F) + (((A & 0x00F0) >> 4)*10) + (((A & 0x0F00)>>8)*100) + (((A & 0xF000)>>12)*1000);
      tmp += (m & 0x000F) + (((m & 0x00F0) >> 4)*10) + (((m & 0x0F00)>>8)*100) + (((m & 0xF000)>>12)*1000); 
      tmp += (getC())? 1:0;

      //value is set
      setN(0);
      setZ(0);
      setV(0);
      setC(0);
      if (tmp & 0x8000) setN(1);
      if (!tmp) setZ(1);
      if (tmp > 9999) setC(1);
      if (tmp & 0x8000) setV(1); 
      m = tmp; //copy
      tmp %= 10; //bottom 4 bits
      tmp |= ((m/10)%10) << 4;
      tmp |= ((m/100)%10) << 8;
      tmp |= ((m/1000)%10) << 12;
      A = tmp;
    }
  } else { //normal binary
    if (getM()) {
      WORD tmp = *AL + getByte(i) + (getC())? 1:0; 

      //flags
      setN(0);
      setZ(0);
      setV(0);
      setC(0);
      if (tmp & 0x80) setN(1); //high bit is set 
      if (tmp == 0) setZ(1);
      if (tmp & 0x0100) setC(1); //rolled over 
      if (!((A & 0x80) ^ (getByte(i) & 0x80)) && (tmp & 0x80) ^ (A & 0x80)) setV(1); //if sign of operands are the same and tmp is different than that  

      A = tmp & 0xFF;

    } else {
      WORD m = getWordNoWrap(i); 
      unsigned long int tmp = A + m + (getC())? 1:0;

      setN(0);
      setZ(0);
      setV(0);
      setC(0);
      if (tmp & 0x8000) setN(1);
      if (tmp == 0) setZ(0);
      if (tmp & 0x010000) setC(1);
      if (!((A & 0x8000) ^ (m & 0x8000)) && (tmp & 0x8000) ^ (A & 0x8000)) setV(1);

      A = tmp & 0xFFFF;
    }
  }
}

void AND (ADDRESS i) { //"AND" Memory with Accumulator
  setZ(0);
  setN(0);

  if (getM()) {
    *AL &= getByte(i); 

    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A &= getWordNoWrap(i); 
    
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//do memory only 
//size of shifted area depends on M flag 
void ASL (ADDRESS i) { //Shift One Bit Left, Memory or Accumulator

  setZ(0);
  setC(0);
  setN(0);

  if (getM()) {
    BYTE m = getByte(i); 
    if (m & 0x80) setC(1); 
    m = m << 1;
    setByte(i,m); 

    if (!m) setZ(1);
    if (m & 0x80) setN(1);

  } else {
    WORD o = getWordNoWrap(i); 

    if (o & 0x8000) setC(1);

    o = o << 1;
    setWordNoWrap(i,o);
    if (!o) setZ(1);
    if (o & 0x8000) setN(1);
    if (o * 0x01) setC(1);
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

//and but it only modifies flags, not A
void BIT (ADDRESS i) { //Bit Test
  setZ(0);
  setN(0);

  if (getM()) {
    BYTE tmp = *AL & getByte(i); 

    if (!(tmp)) setZ(1);
    if (tmp & 0x80) setN(1);
  } else {
    WORD tmp = A & getWordNoWrap(i); 
    
    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
  }
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

//confirm that this does not change PB 
// leaving this here, but looks good
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
  setC(0);
}

void CLD (void) { //Clear Decimal Mode
  setD(0);
}

void CLI (void) { //Clear Interrupt Disable Bit
  setI(0);
}

void CLV (void) { //Clear Overflow Flag
  setV(0);
}

//subtract memory from A, but don't change operands, see BIT 
void CMP (ADDRESS i) { //Compare Memory and Accumulator
  setC(0);
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE m = getByte(i); 
    BYTE tmp = *AL - m;
    if (!tmp) setZ(1);
    if (tmp & 0x80) setN(1);
    if (*AL < m) setC(1);
  } else {
    WORD m = getWordNoWrap(i); 
    WORD tmp = A - m;

    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
    if (A < m) setC(1);
  }
}

void CPX (ADDRESS i) { //Compare Memory and Index X
  setC(0);
  setZ(0);
  setN(0);
  if (getX()) {
    BYTE m = getByte(i); 
    BYTE tmp = *XL - m;
    if(!tmp) setZ(1);
    if(tmp & 0x80) setN(1);
    if(*XL < m) setC(1);
  } else {
    WORD m = getWordNoWrap(i); 
    WORD tmp = X - m;
    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
    if (X < m) setC(1);
  }
}

void CPY (ADDRESS i) { //Compare Memory and Index Y
  setC(0);
  setZ(0);
  setN(0);
  if (getX()) {
    BYTE m = getByte(i); 
    BYTE tmp = *YL - m;
    if(!tmp) setZ(1);
    if(tmp & 0x80) setN(1);
    if(*YL < m) setC(1);
  } else {
    WORD m = getWordNoWrap(i); 
    WORD tmp = Y - m;
    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
    if (Y < m) setC(1);
  }
}

//follows M flag 
//memory only version 
void DEC (ADDRESS i) { //Decrement Memory or Accumulator by One  
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE m = getByte(i); 
    m--;
    setByte(i,m);
    if (!m) setZ(1);
    if (m & 0x80) setN(1);
  } else {
    WORD o = getWordNoWrap(i); 
    o--;
    setWordNoWrap(i,o);
    if (!o) setZ(1);
    if (0 & 0x8000) setN(1);
  }
}

void DEX (void) { //Decrement Index X by One               
  setZ(0);
  setN(0);
  if (getX()) {
    *XL--;
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X--;
    if (!X) setZ(1);
    if(X & 0x8000) setN(1);
  }
}

void DEY (void) { //Decrement Index Y by One
  setZ(0);
  setN(0);
  if (getX()) {
    *YL--;
    if (!(*YL)) setZ(1);
    if (*YL & 0x80) setN(1);
  } else {
    Y--;
    if (!Y) setZ(1);
    if(Y & 0x8000) setN(1);
  }
}

void EOR (ADDRESS i) { //Exclusive "OR" Memory with Accumulator 
  setZ(0);
  setN(0);
  if (getM()) {
    *AL ^= getbyte(i); 
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A ^= getWordNoWrap(i); 
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//see DEC 
void INC (ADDRESS i) { //Increment Memory or Accumulator by One     
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE m = getByte(i); 
    m++;
    setByte(i,m)
    if (!m) setZ(1);
    if (m & 0x80) setN(1);
  } else {
    WORD o = getWordNoWrap(i); 
    o++;
    setWordNoWrap(i,o);
    if (!o) setZ(1);
    if (o & 0x8000) setN(1);
  }
}

void INX (void) { //Increment Index X by One
  setZ(0);
  setN(0);
  if (getX()) {
    *XL++;
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X++;
    if (!X) setZ(1);
    if (X & 0x8000) setN(1);
  }
}

void INY (void) { //Increment Index Y by One
  setZ(0);
  setN(0);
  if (getX()) {
    *YL++;
    if (!(*YL)) setZ(1);
    if (*YL & 0x80) setN(1);
  } else {
    Y++;
    if (!Y) setZ(1);
    if (Y & 0x8000) setN(1);
  }
}

//the only instructions that affect the program bank register are: RTI, RTL, JML, JSL, JMP Absolute Long 
void JML (ADDRESS i) { //Jump Long
  PC = i & 0xFFFF;
  PB = (i & 0xFF0000) >> 16;
}

void JMP (ADDRESS i) { //Jump to New Location
  PC = i & 0xFFFF;
}

void JSL (ADDRESS i) { //Jump Subroutine Long
  pushByte(PB);
  pushWord(PC);
  PC = i & 0xFFFF;
  PB = (i & 0xFF0000) >> 16;
}

void JSR (ADDRESS i) { //Jump to New Location Saving Return Address
  pushWord(PC);
  PC = i & 0xFFFF;
}

void LDA (ADDRESS i) { //Load Accumulator with Memory
  setZ(0);
  setN(0);
  if (getM()) {
    *AL = getByte(i); 
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A = getWordNoWrap(i); 
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

void LDX (ADDRESS i) { //Load Index X with Memory
  setZ(0);
  setN(0);
  if (getX()) {
    *XL = getByte(i); 
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X = getWordNoWrap(i); 
    if (!X) setZ(1);
    if (X & 0x8000) setN(1);
  }
}

void LDY (ADDRESS i) { //Load Index Y with Memory                  
  setZ(0);
  setN(0);
  if (getX()) {
    *YL = getByte(i); 
    if (!(*YL)) setZ(1);
    if (*YL & 0x80) setN(1);
  } else {
    Y = getWordNoWrap(i); 
    if (!Y) setZ(1);
    if (Y & 0x8000) setN(1);
  }
}

//see ASL 
void LSR (ADDRESS i) { //Shift One Bit Right (Memory or Accumulator) 
  setZ(0);
  setN(0);
  setC(0);
  if (getM()) {
    BYTE m = getByte(i); 
    if (m & 0x01) setC(1);
    m = m >> 1;
    setbyte(i,m); 

    if (!m) setZ(1);
    if (m & 0x80) setN(1);
  } else {
    WORD o = getWordNoWrap(i); 
    if (o & 0x01) setC(1);
    o = o << 1;
    setWordNoWrap(i,o);

    if (!o) setZ(1);
    if (o & 0x8000) setN(1);
  }
}

//probably won't be called, but I will leave this here anyway 
void NOP (void) { //No Operation                          
  //self explanatory 
}

void ORA (ADDRESS i) { //"OR" Memory with Accumulator 
  setZ(0);
  setN(0);
  if (getM()) {
    *AL |= getByte(i); 
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A |= getWordNoWrap(i); 
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//deref'd memory, (this one not an in code constant)
//compose addr from optcode (direct indirect)
//pull word from location, then push to stack
void PEI (ADDRESS i) { //Push Effective Indirect Address on Stack (add one cycle if DL f 0)
  pushWord(getWordNoWrap(i)); 
}

//push address, not deref'd
void PER (ADDRESS i) { //Push Effective Program Counter Relative Address on Stack
  pushWord((WORD)i); 
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
  setZ(0);
  setN(0);
  if (getM()) {
    *AL = popByte();
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A = popWord();
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

void PLB (void) { //Pull Data Bank Register from Stack
  setZ(0);
  setN(0);
  DB = popByte();
  if (!DB) setZ(1);
  if (DB & 0x80) setN(1);
}

void PLD (void) { //Pull Direct Register from Stack
  setZ(0);
  setN(0);
  D = popWord();
  if (!D) setZ(1);
  if (D & 0x8000) setN(1);
}

void PLP (void) { //Pull Processor Status from Stack
  P = popByte();
}

void PLX (void) { //Pull Index X from Stack
  setZ(0);
  setN(0);
  if (getX()) {
    *XL = popByte();
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X = popWord();
    if (!X) setZ(1);
    if (X & 0x8000) setN(1);
  }
}

void PLY (void) { //Pull Index Y form Stack
  setZ(0);
  setN(0);
  if (getX()) {
    *YL = popByte();
    if (!(*YL)) setZ(1);
    if (*YL & 0x80) setN(1);
  } else {
    Y = popWord();
    if (!Y) setZ(1);
    if (Y & 0x8000) setN(1);
  }
}

//follow M flag for size, rotated bit is carry. 
//mem only version
void ROL (ADDRESS i) { //Rotate One Bit Left (Memory or Accumulator)     
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE out = getC();
    setC(0);
    out |= (getByte(i) << 1) & 0xFF; 
    setByte(i, out);
    if (!out) setZ(1);
    if (out & 0x80) setN(1);
    if (out & 0x80) setC(1);
  } else {
    WORD out = getC();
    setC(0);
    out |= (getWordNoWrap(i) << 1) & 0xFFFF; 
    setWordNoWrap(i,out);
    if (!out) setZ(1);
    if (out & 0x8000) setN(1);
    if (out & 0x8000) setC(1);
  }
}

//see above 
void ROR (ADDRESS i) { //Rotate One Bit Right (Memory or Accumulator)
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE out = getC() << 7;
    setC(0);
    out |= getByte(i) >> 1; 
    setByte(i, out);
    if (!out) setZ(1);
    if (out & 0x80) setN(1);
    if (out & 0x80) setC(1);
  } else {
    WORD out = getC() << 15;
    setC(0);
    out |= (getWord(i)) >> 1; 
    setWordNoWrap(i, out);
    if (!out) setZ(1);
    if (out & 0x8000) setN(1);
    if (out & 0x8000) setC(1);
  }
}

//next three, caller convention is push PC before jmp
//so to return, just pop PC
void RTI (void) { //Return from Interrupt                     
  //says it should clear I, but P gets restored anyway??? 
  PC = popWord();
  PB = popByte(); //not sure if this should be here? but I think so, see JML
  P = popByte(); //restore status
}

//This and next have to inc by 1
//TODO check above 
//strongly doubt, PC get's inc'd to next instruction before jump pushes return address
void RTL (void) { //Return from Subroutine Long
  PC = popWord() + 1;
  PB = popByte(); 
}

void RTS (void) { //Return from Subroutine
  PC = popWord() + 1;
}

//follows M flag 
//slight doubts about sign/flag issues here? 
void SBC (ADDRESS i) { //Subtract Memory from Accumulator with Borrow
  setN(0);
  setZ(0);
  setV(0);
  if (getD()) {//BCD
    BYTE carryHolder = 0;
    if (getM()) {
      BYTE m = getByte(i);
      m = (m & 0x0F) + ((m & 0xF0)*10);
      BYTE tmp = (*AL & 0x0F) + ((*AL & 0xF0)*10);
      carryHolder = m <= tmp; //carry not required
      tmp -= m;
      tmp -= (getC())? 0:1; //sub if clear
      setC(0);

      //value is set
      if (!tmp) setZ(1);
      if (tmp & 0x80) setN(1);
      if (carryHolder) setC(1);
      if (tmp & 0x80) setV(1); // I am going to have it follow N
      m = tmp % 10; //low 4b
      tmp = ((tmp/10)%10) << 4;
      *AL = tmp;
    } else {
      WORD m = getWordNoWrap(i);
      WORD tmp = (A & 0x000F) + (((A & 0x00F0) >> 4)*10) + (((A & 0x0F00)>>8)*100) + (((A & 0xF000)>>12)*1000);
      carryHolder = m <= tmp; //carry not required
      tmp -= (m & 0x000F) + (((m & 0x00F0) >> 4)*10) + (((m & 0x0F00)>>8)*100) + (((m & 0xF000)>>12)*1000); 
      tmp -= (getC())? 0:1;//sub if clear
      setC(0);
      //value is set
      if (!tmp) setZ(1);
      if (tmp & 0x8000) setN(1);
      if (carryHolder) setC(1);
      if (tmp & 0x8000) setV(1);
      m = tmp; //copy
      tmp %= 10; //bottom 4 bits
      tmp |= ((m/10)%10) << 4;
      tmp |= ((m/100)%10) << 8;
      tmp |= ((m/1000)%10) << 12;
      A = tmp;
    }
  } else {//normal binary
    if (getM()) {
      BYTE m = getByte(i); 
      BYTE tmp = *AL - m - (getC())? 0:1; //sub if carry is clear 
      setC(0);
      if (!tmp) setZ(1);
      if (tmp & 0x80) setN(1);
      if (m <= *AL) setC(1); //set if borrow not required 
      if ((0x80 & *AL) ^ (0x80 & m) && (0x80 & *AL) ^ (0x80 & tmp)) setV(1); //if operands have diff. signs & output and first op differ 
      *AL = tmp;
    } else {
      WORD m = getWordNoWrap(i); 
      WORD tmp = A - m - (getC())? 0:1; 
      setC(0);
      if (!tmp) setZ(1);
      if (tmp & 0x8000) setN(1);
      if (m <= A) setC(1);
      if ((0x8000 & A) ^ (0x80000 & m) && (0x8000 & A) ^ (0x8000 & tmp)) setV(1); //see above 
      A = tmp;
    }
  }
}

void SEC (void) { //Set Carry Flag                        
  setC(1);
}

void SED (void) { //Set Decimal Mode
  setD(1);
}

//set automatically when accepting interupt. 
void SEI (void) { //Set Interrupt Disable Status
  setI(1);
}

void STA (ADDRESS i) { //Store Accumulator in Memory
  setByte(i,*AL); 
  if (!getM()) {
    setbyte(i+1, *AH); 
  }
}

//TODO clock interaction 
void STP (void) { //Stop the Clock

}

void STX (ADDRESS i) { //Store Index X in Memory
  setByte(i,*XL); 
  if (!getX()) {
    setByte(i+1,*XH); 
  }
}

void STY (ADDRESS i) { //Store Index Y in Memory
  setByte(i,*YL); 
  if (!getX()) {
    setByte(i=1,*YH);
  }
}

void STZ (ADDRESS i) { //Store Zero in Memory
  setByte(i,0); 
  if (!getM()) {
    setByte(i+1,0);
  }
}

//size by destination 
void TAX (void) { //Transfer Accumulator to Index X
  setN(0);
  setZ(0);
  if (getX()) {
    *XL = *AL;
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X = A;
    if (!X) setZ(1);
    if (X & 0x8000) setN(1);
  }
}

void TAY (void) { //Transfer Accumulator to Index Y
  if (getX()) {
    *YL = *AL;
    if (!(*YL)) setZ(1);
    if (*YL & 0x80) setN(1);
  } else {
    Y = A;
    if (!Y) setZ(1);
    if (Y & 0x8000) setN(1);
  }
}

//ignores m flag 
void TCD (void) { //Transfer Accumulator to Direct Register
  setZ(0);
  setN(0);
  D = A;
  if (!D) setZ(1);
  if (D & 0x8000) setN(1);
}

//ignores m flag 
//does not set flags 
void TCS (void) { //Transfer Accumulator to Stack Pointer Register
  S = A;
}

//ignores m flag 
void TDC (void) { //Transfer Direct Register to Accumulator
  setZ(0);
  setN(0);
  A = D;
  if (!A) setZ(1);
  if (A & 0x8000) setN(1);
}


//and memory and ~A, reset affected bits in memory, set Z  
void TRB (ADDRESS i) { //Test and Reset Bit
  setZ(0);
  if (getM()) {
    BYTE m = getByte(i); 
    setByte(i,(~(*AL)) & m); 
    if (!(m & (*AL))) setZ(1); //slightly different operation, see BIT
  } else {
    WORD m = getWordNoWrap(i); 
    WORD tmp = (~A) & m;
    setWordNoWrap(i,tmp);
    if (!(m & A)) setZ(1); //slightly different operation, see BIT
  }
}

//see above, but set bits in memory 
void TSB (ADDRESS i) { //Test and Set Bit
  setZ(0);
  if (getM()) {
    BYTE m = getByte(i); 
    setByte(i,(*AL) | m); 
    if (!(m & (*AL))) setZ(1); //slightly different operation, see BIT
  } else {
    WORD m = getWordNoWrap(i); 
    WORD tmp = A | m;
    setWordNoWrap(i,tmp);
    if (!(m & A)) setZ(1); //slightly different operation, see BIT
  }
}

//ignores m flag 
void TSC (void) { //Transfer Stack Pointer Register to Accumulator
  setZ(0);
  setN(0);
  A = S;
  if (!A) setZ(1);
  if (A & 0x8000) setN(1);
}

void TSX (void) { //Transfer Stack Pointer Register to Index X
  setZ(0);
  setN(0);
  if (getX()) {
    *XL = S & 0xFF;
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X = S;
    if (!X) setZ(1);
    if (X & 0x8000) setN(1);
  }
}

//size follows destination 
void TXA (void) { //Transfer Index X to Accumulator
  setN(0);
  setZ(0);
  if (getM()) {
    *AL = *XL;
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A = X;
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//follows X but does not set flags 
void TXS (void) { //Transfer Index X to Stack Polnter Register
  if (getX()) {
    S = *XL; //high byte is 0
  } else {
    S = X; //both bytes 
  }
}

void TXY (void) { //Transfer Index X to Index Y
  setZ(0);
  setN(0);
  if (getX()) {
    *YL = *XL;
    if (!(*YL)) setZ(1);
    if (*YL & 0x80) setN(1);
  } else {
    Y = X;
    if (!Y) setZ(1);
    if (Y & 0x8000) setN(1);
  }
}

//size follows destination 
void TYA (void) { //Transfer Index Y to Accumulator
  setZ(0);
  setN(0);
  if (getM()) {
    *AL = *YL;
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A = Y;
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

void TYX (void) { //Transfer Index Y to Index X
  setZ(0);
  setN(0);
  if (getX()) {
    *XL = *YL;
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X = Y;
    if (!X) setZ(1);
    if (X & 0x8000) setN(1);
  }
}

//TODO waiting 
//if I (ignore int.) is set, then will simply continue to the next instruciton, not jump to int vec. 
void WAI (void) { //Wait for Interrupt
  
}

void XBA (void) { //Exchange AH and AL
  setZ(0);
  setN(0);
  BYTE tmp = *AH;
  *AH = *AL;
  *AL = tmp;
  if (!(*AL)) setZ(1);
  if (*AL & 0x80) setN(1);
}

//emulation mode is not implimented here, so some flag changes will be ignored for the time being 
void XCE (void) { //Exchange Carry and Emulation Bits      
  BYTE tmp = P & 0x01;
  P &= 0xFE;
  P |= E;
  E = tmp;
}

