/*
 * author: Thomas Ulmer
 *
 * File contains last fourth of the optcodes
 */

#include "optcodes.h"
#include "address.h"
#include "memory.h"

//CPY immediate
void optC0 (BYTE i, BYTE j) {
  setC(0);
  setZ(0);
  setN(0);
  if (getX()) {
    BYTE m = i; 
    BYTE tmp = *YL - m;
    if(!tmp) setZ(1);
    if(tmp & 0x80) setN(1);
    if(*YL < m) setC(1);
  } else {
    WORD m = ((WORD)j << 8) | i;
    WORD tmp = Y - m;
    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
    if (Y < m) setC(1);
  }
}

//CMP direct indexed indirect (X)
void optC1 (BYTE i) {
  CMP(directIndexedIndirect(i));
}

//REP immediate 
void optC2 (BYTE i) {
  P &= ~i;
}

//CMP stack relative 
void optC3 (BYTE i) {
  CMP(stackRelative(i));
}

//CPY direct 
void optC4 (BYTE i) {
  CPY(direct(i));
}

//CMP direct 
void optC5 (BYTE i) {
  CMP(direct(i));
}

//DEC direct 
void optC6 (BYTE i) {
  DEC(direct(i));
}

//CMP direct indirect long
void optC7 (BYTE i) {
  CMP(directIndirectLong(i));
}

//INY implied 
void optC8 (void) {
  INY();
}

//CMP immediate
void optC9 (BYTE i, BYTE j) {
  setC(0);
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE m = i; 
    BYTE tmp = *AL - m;
    if (!tmp) setZ(1);
    if (tmp & 0x80) setN(1);
    if (*AL < m) setC(1);
  } else {
    WORD m = ((WORD)j << 8) | i; 
    WORD tmp = A - m;

    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
    if (A < m) setC(1);
  }
}

//DEX implied
void optCA (void) {
  DEX();
}

//WAI implied //TODO
void optCB (void);

//CPY absolute
void optCC (BYTE i, BYTE j) {
  CPY(absolute(i,j));
}

//CMP absolute
void optCD (BYTE i, BYTE j) {
  CMP(absolute(i,j));
}

//DEC absolute
void optCE (BYTE i, BYTE j) {
  DEC(absolute(i,j));
}

//CMP absolute long
void optCF (BYTE i, BYTE j, BYTE k) {
  CMP(absoluteLong(i,j,k));
}

//BNE PC relative
void optD0 (BYTE i) {
  BNE(programCounterRelative(i));
}

//CMP direct indirect indexed (Y)
void optD1 (BYTE i) {
  CMP(directIndirectIndexed(i));
}

//CMP direct indirect
void optD2 (BYTE i) {
  CMP(directIndirect(i));
}

//CMP stack relative indirect indexed (Y)
void optD3 (BYTE i) {
  CMP(stackRelativeIndirectIndexed(i));
}

//PEI stack (direct indirect)
void optD4 (BYTE i) {
  PEI(directIndirect(i));
}

//CMP direct indexed (X)
void optD5 (BYTE i) {
  CMP(directIndexedWithX(i));
}

//DEC direct indexed (X)
void optD6 (BYTE i) {
  DEC(directIndexedWithX(i));
}

//CMP direct indirect long indexed (Y)
void optD7 (BYTE i) {
  CMP(directIndirectIndexedLong(i));
}

//CLD implied
void optD8 (void) {
  CLD();
}

//CMP abolsute indexed (Y)
void optD9 (BYTE i, BYTE j) {
  CMP(absoluteIndexedWithY(i,j));
}

//PHX stack 
void optDA (void) {
  PHX();
}

//STP implied //TODO
void optDB (void);

//JMP absolute indirect long
//opt6C but jml and not jmp
//address.c return w/ PB included
void optDC (BYTE i, BYTE j) {
  JML(absoluteIndirect(i,j));
}

//CMP absolute indexed (X)
void optDD (BYTE i, BYTE j) {
  CMP(absoluteIndexedWithX(i,j));
}

//DEC absolute indexed (X)
void optDE (BYTE i, BYTE j) {
  DEC(absoluteIndexedWithX(i,j));
}

//CMP absolute long indexed (X)
void optDF (BYTE i, BYTE j, BYTE k) {
  CMP(absoluteLongIndexedWithX(i,j,k));
}

//CPX immediate
void optE0 (BYTE i, BYTE j){
  setC(0);
  setZ(0);
  setN(0);
  if (getX()) {
    BYTE m = i; 
    BYTE tmp = *YL - m;
    if(!tmp) setZ(1);
    if(tmp & 0x80) setN(1);
    if(*YL < m) setC(1);
  } else {
    WORD m = ((WORD)j << 8) | i; 
    WORD tmp = Y - m;
    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
    if (Y < m) setC(1);
  }
}

//SBC direct indexed indirect x 
void optE1 (BYTE i) {
  SBC(directIndexedIndirect(i));
}

//SEP immediate 
void optE2 (BYTE i) {
  P |= i;
}

//SBC stack relative 
void optE3 (BYTE i) {
  SBC(stackRelative(i));
}

//CPX direct 
void optE4 (BYTE i) {
  CMP(direct(i));
}

//SBC direct 
void optE5 (BYTE i) {
  SBC(direct(i));
}

//INC direct 
void optE6 (BYTE i) {
  INC(direct(i));
}

//SBC direct indirect long 
void optE7 (BYTE i) {
  SBC(directIndirectLong(i));
}

//INX implied 
void optE8 (void) {
  INX();
}

//SBC immediate 
void optE9 (BYTE i, BYTE j) {
  setN(0);
  setZ(0);
  setV(0);
  if (getD()) {//BCD
    BYTE carryHolder = 0;
    if (getM()) {
      BYTE m = i;
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
      WORD m = ((WORD)j << 8) | i;
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
      BYTE m = i; 
      BYTE tmp = *AL - m - (getC())? 0:1; //sub if carry is clear 
      setC(0);
      if (!tmp) setZ(1);
      if (tmp & 0x80) setN(1);
      if (m <= *AL) setC(1); //set if borrow not required 
      if ((0x80 & *AL) ^ (0x80 & m) && (0x80 & *AL) ^ (0x80 & tmp)) setV(1); //if operands have diff. signs & output and first op differ 
      *AL = tmp;
    } else {
      WORD m = ((WORD)j << 8) | i; 
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

//NOP implied
void optEA (void) {
  NOP();
}

//XBA implied 
void optEB (void) {
  XBA();
}

//CPX absolute
void optEC (BYTE i, BYTE j) {
  CPX(absolute(i,j));
}

//SBC absolute 
void optED (BYTE i, BYTE j) {
  SBC(absolute(i,j));
}

//INC absolute
void optEE (BYTE i, BYTE j) {
  INC(absolute(i.j));
}

//SBC absolute long 
void optEF (BYTE i, BYTE j, BYTE k) {
  SBC(absoluteLong(i,j,k));
}

//BEQ program counter relative
void optF0 (BYTE i) {
  BEQ(programCounterRelative(i));
}

//SBC direct indirect indexed Y
void optF1 (BYTE i) {
  SBC(directIndirectIndexed(i));
}

//SBC direct indirect 
void optF2 (BYTE i) {
  SBC(directIndirect(i));
}

//SBC stack relative indirect indexed Y 
void optF3 (BYTE i) {
  SBC(stackRelativeIndirectIndexed(i));
}

//PEA (immediate) stack 
//this is just pushing a 16b constant to the stack
void optF4 (BYTE i, BYTE j) {
  BYTE tmp = ((WORD)j << 8) | i;
  setWordNoWrap((ADDRESS)S - 1, tmp);
  S -= 2;
}

//SBC direct indexed X
void optF5 (BYTE i) {
  SBC(directIndexedWithX(i));
}

//INC direct indexed X
void optF6 (BYTE i) {
  INC(directIndexedWithX(i));
}

//SBC direct indirect long indexed Y
void optF7 (BYTE i) {
  SBC(directIndirectIndexedLong(i));
}

//SED implied 
void optF8 (void) {
  SED();
}

//SBC absolute indexed Y 
void optF9 (BYTE i, BYTE j) {
  SBC(absoluteIndexedWithY(i,j));
}

//PLX stack 
void optFA (void) {
  PLX();
}

//XCE implied 
void optFB (void) {
  XCE();
}

//JSR absolute indexed indirect 
void optFC (BYTE i, BYTE j) {
  JSR(absoluteIndexedIndirect(i,j));
}

//SBC absoluted indexed X
void optFD (BYTE i, BYTE j) {
  SBC(absoluteIndexedWithX(i,j));
}

//INC absolute indexed X
void optFE (BYTE i, BYTE j) {
  INC(absoluteIndexedWithX(i,j));
}

//SBC absolute long indexed X 
void optFF (BYTE i, BYTE j, BYTE k) {
 SBC(absoluteLongIndexedWithX(i,j,k)); 
}
