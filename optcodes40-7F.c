/*
 * author: Thomas Ulmer
 *
 * File contains the second 1/4th of the optcodes
 */

#include "optcodes.h"
#include "address.h"
#include "memory.h"


//RTI stack 
void opt40 (void) {
  RTI();
}

//EOR direct indexed indirect (X)
void opt41 (BYTE i) {
  EOR(directIndexedIndirect(i));
}

//WDM N/A 
//TODO
void opt42 (BYTE i);

//EOR Stack relative
void opt43 (BYTE i) {
  EOR(stackRelative(i));
}

//MVP block move 
//TODO
void opt44 (BYTE i, BYTE j);

//EOR direct 
void opt45 (BYTE i) {
  EOR(direct(i));
}

//LSR direct 
void opt46 (BYTE i) {
  LSR(direct(i));
}

//EOR direct indirect long
void opt47 (BYTE i) {
  EOR(directIndirectLong(i));
}

//PHA stack 
void opt48 (void) {
  PHA();
}

//EOR immediate
void opt49 (BYTE i, BYTE j) { 
  setZ(0);
  setN(0);
  if (getM()) {
    *AL ^= i; 
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A ^= ((WORD)j << 8) | i; 
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//LSR accumulator
void opt4A (void) {
  setZ(0);
  setN(0);
  setC(0);
  if (getM()) {
    if ((*AL) & 0x01) setC(1);
    *AL >>= 1;

    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    if (A & 0x01) setC(1);
    A >>= 1;

    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//PHK stack 
void opt4B (void) {
  PHK();
}

//JMP absolute
void opt4C (BYTE i, BYTE j) {
  JMP(absolute(i,j));
}

//EOR absolute 
void opt4D (BYTE i, BYTE j) {
  EOR(absolute(i,j));
}

//LSR absolute 
void opt4E (BYTE i, BYTE j) {
  LSR(absolute(i,j));
}

//EOR absolute long
void opt4F (BYTE i, BYTE j, BYTE k) {
  EOR(absoluteLong(i,j,k));
}

//BVC PC relative
void opt50 (BYTE i) {
  BVC(programCounterRelative(i));
}

//EOR direct indirect indexed (Y)
void opt51 (BYTE i) {
  EOR(directIndirectIndexed(i));
}

//EOR direct indirect 
void opt52 (BYTE i) {
  EOR(directIndirect(i));
}

//EOR stack relative indirect indexed Y
void opt53 (BYTE i) {
  EOR(stackRelativeIndirectIndexed(i));
}

//MVN block move 
//TODO
void opt54 (BYTE i, BYTE j);

//EOR direct indexed (X)
void opt55 (BYTE i) {
  EOR(directIndexedWithX(i));
}

//LSR direct indexed X
void opt56 (BYTE i) {
  LSR(directIndexedWithX(i));
}

//EOR direct indirect long indexed (Y)
void opt57 (BYTE i) {
  EOR(directIndirectIndexedLong(i));
}

//CLI implied
void opt58 (void) {
  CLI();
}

//EOR absolute indexed y
void opt59 (BYTE i, BYTE j) {
  EOR(absoluteIndexedWithY(i,j));
}

//PHY stack 
void opt5A (void) {
  PHY();
}

//TCD implied 
void opt5B (void) {
  TCD();
}

//JMP absolute long
void opt5C (BYTE i, BYTE j, BYTE k) {
  JMP(absoluteLong(i,j,k));
}

//EOR absolute indexed X
void opt5D (BYTE i, BYTE j) {
  EOR(absoluteIndexedWithX(i,j));
}

//LSR abolsute indexed X
void opt5E (BYTE i, BYTE j) {
  LSR(absoluteIndexedWithY(i,j));
}

//EOR absolute long indexed X
void opt5F (BYTE i, BYTE j, BYTE k) {
  EOR(absoluteLongIndexedWithX(i,j,k));
}

//RTS stack 
void opt60 (void) {
  RTS();
}

//ADC direct indexed indirect (X)
void opt61 (BYTE i) {
  ADC(directIndexedIndirect(i));
}

//PER stack (PC relative lon)
void opt62 (BYTE i, BYTE j) {
  PER(programCounterRelativeLong(i,j));
}

//ADC stack relative
void opt63 (BYTE i) {
  ADC(stackRelative(i));
}

//STZ direct page 
void opt64 (BYTE i) {
  STZ(direct(i));
}

//ADC Direct
void opt65 (BYTE i) {
  ADC(direct(i));
}

//ROR direct //TODO might not match with declaration
void opt66 (BYTE i) {
  ROR(direct(i));
}

//ADC direct indrect long
void opt67 (BYTE i) {
  ADC(directIndirectLong(i));
}

//PLA stack 
void opt68 (void) {
  PLA();
}

//ADC immediate
//TODO make sure this matches changes from instructions.c version
void opt69 (BYTE i) {
  if (getD()) { //BCD addition
    if (getM()) {
      BYTE tmp = (*AL & 0x0F) + (((*AL & 0xF0) >> 4)*10);
      tmp += (i & 0x0F) + (((i & 0xF0) >> 4)*10);
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
      i = tmp % 10; //bottom 4 bits
      tmp = (tmp / 10) %10; //high 4b
      tmp <<= 4;
      tmp |= i;
      *AL = tmp;
    } else { //16b
      WORD m = ((WORD)j <<8) | i;
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
      WORD tmp = *AL + i + (getC())? 1:0; 

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
      WORD m = ((WORD)j << 8) | i; 
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

//ROR accumulator 
void opt6A (void) {
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE out = getC() << 7;
    setC(0);
    *AL = (*AL >> 1) | out;
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
    if (*AL & 0x80) setC(1);
  } else {
    WORD out = getC() << 15;
    setC(0);
    A = (A >> 1) | out;
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
    if (A & 0x8000) setC(1);
  }
}

//RTL stack 
void opt6B (void) {
  RTL();
}

//JMP absolute indirect
void opt6C (BYTE i, BYTE j) {
  JMP(absoluteIndirect(i,j));
}

//ADC absolute
void opt6D (BYTE i, BYTE j) {
  ADC(absolute(i,j));
}

//ROR absolute 
void opt6E (BYTE i, BYTE j) {
  ROR(absolute(i,j));
}

//ADC abolute long
void opt6F (BYTE i, BYTE j, BYTE k) {
  ADC(absoluteLong(i,j,k));
}

//BVS PC relative
void opt70 (BYTE i) {
  BVS(programCounterRelative(i));
}

//ADC direct indirect indexed (Y)
void opt71 (BYTE i) {
  ADC(directIndirectIndexed(i));
}

//ADC direct indirect
void opt72 (BYTE i) {
  ADC(directIndirect(i));
}

//ADC stack relative indirect indexed (Y)
void opt73 (BYTE i) {
  ADC(stackRelativeIndirectIndexed(i));
}

//STZ direct indexed X 
void opt74 (BYTE i) {
  STZ(directIndexedWithX(i));
}

//ADC direct indexed (X)
void opt75 (BYTE i) {
  ADC(directIndexedWithX(i));
}

//ROR direct indexed X 
void opt76 (BYTE i) {
  ROR(directIndexedWithX(i));
}

//ADC direct indirect long indexed (Y)
void opt77 (BYTE i) {
  ADC(directIndirectIndexedLong(i));
}

//SEI implied 
void opt78 (void) {
  SEI();
}

//ADC absolute indexed (Y)
void opt79 (BYTE i, BYTE j) {
  ADC(absoluteIndexedWithY(i,j));
}

//PLY stack 
void opt7A (void) {
  PLY();
}

//TDC implied 
void opt7B (void) {
  TDC();
}

//JMP absolute indexed indirect 
void opt7C (BYTE i, BYTE j) {
  JMP(absoluteIndexedIndirect(i,j));
}

//ADC absolute indexed (X)
void opt7D (BYTE i, BYTE j) {
  ADC(absoluteIndexedWithX(i,j));
}

//ROR absolute indexed X 
void opt7E (BYTE i, BYTE j) {
  ROR(absoluteIndexedWithX(i,j));
}

//ADC absolute long indexed (X)
void opt7F (BYTE i, BYTE j, BYTE) {
  ADC(absoluteLongIndexedWithX(i,j,k));
}
