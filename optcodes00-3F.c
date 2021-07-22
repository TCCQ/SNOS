/*
 * author: Thomas Ulmer
 *
 * File contains implimentations of the first 1/4 of the optcodes
 */

#include "optcodes.h"
#include "address.h"
#include "memory.h"

//BRK stack
void opt00 (BYTE);

//ORA direct indexed indirect X
void opt01 (BYTE i ) {
  ORA(directIndexedIndirect(i));
}

//COP stack
void opt02 (BYTE);

//ORA stack relative 
void opt03 (BYTE i) {
  ORA(stackRelative(i));
}

//TSB direct 
void opt04 (BYTE i) {
  TSB(direct(i));
}

//ORA direct 
void opt05 (BYTE i) {
  ORA(direct(i));
}

//ASL direct
void opt06 (BYTE i) {
  ASL(direct(i));
}

//ORA direct indirect long 
void opt07 (BYTE i) {
  ORA(directIndirectLong(i));
}

//PHP stack 
void opt08 (void) {
  PHP();
}

//ORA immediate 
void opt09 (BYTE i, BYTE j) {
  setZ(0);
  setN(0);
  if (getM()) {
    *AL |= i; 
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A |= ((WORD)j << 8) | i; 
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//ASL accumulator
//copied from instructions.c
void opt0A (void) {

  setZ(0);
  setC(0);
  setN(0);

  if (getM()) {
    if (*AL & 0x80) setC(1); 
    *AL = *AL << 1;

    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    if (o & 0x8000) setC(1);

    A = A << 1;
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
    if (A * 0x01) setC(1);
  }
}

//PHD stack 
void opt0B (void) {
  PHD();
}

//TSB absolute 
void opt0C (BYTE i, BYTE j) {
  TSB(absolute(i,j))
}

//ORA absolute 
void opt0D (BYTE i, BYTE j) {
  ORA(absolute(i,j));  
}

//ASL absolute
void opt0E (BYTE i, BYTE j) {
  ASL(absolute(i,j));
}

//ORA absolute long 
void opt0F (BYTE i, BYTE j, BYTE k) {
  ORA(absoluteLong(i,j,k));
}

//BPL PC relative
void opt10 (BYTE i) {
  BLP(programCounterRelative(i));
}

//ORA direct indirect indexed Y 
void opt11 (BYTE i) {
  ORA(directIndirectIndexed(i));
}

//ORA direct indirect 
void opt12 (BYTE i) {
  ORA(directIndirect(i));
}

//ORA stack relative indriect indexed Y
void opt13 (BYTE i) {
  ORA(stackRelativeIndirectIndexed(i));
}

//TRB direct 
void opt14 (BYTE i) {
  TRB(direct(i));
}

//ORA direct indexed X 
void opt15 (BYTE i) {
  ORA(directIndexedWithX(i));
}

//ASL direct indexed (X) 
void opt16 (BYTE i) {
  ASL(directIndexedWithX(i));
}

//ORA direct indirect long indexed Y 
void opt17 (BYTE i) {
  ORA(directIndirectIndexedLong(i));
}

//CLC implied 
void opt18 (void) {
  CLC();
}

//ORA absolute indexed y
void opt19 (BYTE i, BYTE j) {
  ORA(absoluteIndexedWithY(i,j));
}

//INC accumulator
void opt1A (void) {
  setZ(0);
  setN(0);
  if (getM()) {
    (*AL)++;
    if (!(*AL)) setZ(1);
    if ((*AL) & 0x80) setN(1);
  } else {
    A++;
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//TCS implied 
void opt1B (void) {
  TCS();
}

//TRB absolute 
void opt1C (BYTE i, BYTE j) {
  TRB(absolute(i,j));
}

//ORA Absolute indexed X
void opt1D (BYTE i, BYTE j) {
  ORA(absoluteIndexedWithX(i,j));
}

//ASL absolute indexed (X)
void opt1E (BYTE i, BYTE j) {
  ASL(absoluteIndexedWithY(i,j));
}

//ORA abolsute long indexed X
void opt1F (BYTE i, BYTE j, BYTE k) {
  ORA(absoluteLongIndexedWithX(i,j,k));
}

//JSR absolute
void opt20 (BYTE i, BYTE j) {
  JSR(absolute(i,j));
}

//AND direct indexed indirect (X)
void opt21 (BYTE i) {
  AND(directIndexedIndirect(i));
}

//JSR/JSL absolute long 
void opt22 (BYTE i, BYTE j, BYTE k) {
  JSL(absoluteLong(i,j,k));
}

//AND stack relative
void opt23 (BYTE i) {
  AND(stackRelative(i));
}

//BIT direct 
void opt24 (BYTE i) {
  BIT(direct(i));
}

//AND direct 
void opt25 (BYTE i) {
  AND(direct(i));
}

//ROL direct 
void opt26 (BYTE i) {
  ROL(direct(i));
}

//AND direct indirect long
void opt27 (BYTE i) {
  AND(directIndirectLong(i));
}

//PLP stack 
void opt28 (void) {
  PLP();
}

//AND immediate
void opt29 (BYTE i) {
  setZ(0);
  setN(0);

  if (getM()) {
    *AL &= i; 

    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A &= ((WORD)j << 8) | i; 
    
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//ROL accumulator 
//copied from instructions.c
void opt2A (void) {
  setZ(0);
  setN(0);
  if (getM()) {
    BYTE out = getC();
    setC(0);
    *AL = ((*AL << 1) & 0xFF) | out;

    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
    if (*AL & 0x80) setC(1);
  } else {
    WORD out = getC();
    setC(0);
    A = ((A << 1) & 0xFFFF) | out;
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
    if (A & 0x8000) setC(1);
  }
}

//PLD stack 
void opt2B (void) {
  PLD();
}

//BIT absolute
void opt2C (BYTE i, BYTE j) {
  BIT(absolute(i,j));
}

//AND absolute 
void opt2D (BYTE i, BYTE j) {
  AND(absolute(i,j));
}

//ROL absolute 
void opt2E (BYTE i, BYTE j) {
  ROL(absolute(i,j));
}

//AND absolute long
void opt2F (BYTE i, BYTE j, BYTE k) {
  AND(absoluteLong(i,j,k));
}

//BMI program counter relative
void opt30 (BYTE i) {
  BMI(programCounterRelative(i));
}

//AND direct indirect indexed (Y)
void opt31 (BYTE i) {
  AND(directIndirectIndexed(i));
}

//AND direct indirect
void opt32 (BYTE i) {
  AND(directIndirect(i));
}

//AND stack relative indirect indexed (Y)
void opt33 (BYTE i) {
  AND(stackRelativeIndirectIndexed(i));
}

//BIT direct indexed (X)
void opt34 (BYTE i) {
  BIT(directIndexedWithX(i));
}

//AND direct indexed (X)
void opt35 (BYTE i) {
  AND(directIndexedWithX(i));
}

//ROL direct indexed X 
void opt36 (BYTE i) {
  ROL(directIndexedWithX(i));
}

//AND direct indirect long indexed (Y)
void opt37 (BYTE i) {
  AND(directIndirectIndexedLong(i));
}

//SEC implied 
void opt38 (void) {
  SEC();
}

//AND abolsute indexed (Y)
void opt39 (BYTE i, BYTE j) {
  AND(absoluteIndexedWithY(i,j));
}

//DEC accumulator
void opt3A (void) {
  setZ(0);
  setN(0);
  if (getM()) {
    (*AL)--;
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A--;
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//TSC implied 
void opt3B (void) {
  TSC();
}

//BIT absolute indexed (X)
void opt3C (BYTE i, BYTE j) {
  BIT(absoluteIndexedWithX(i,j));
}

//AND absoltue indexed (X)
void opt3D (BYTE i, BYTE j) {
  AND(absoluteIndexedWithX(i,j));
}

//ROL Absolute indexed X 
void opt3E (BYTE i, BYTE j) {
  ROL(absoluteIndexedWithX(i,j));
}

//AND abolsute long indexed (X)
void opt3F (BYTE i, BYTE j, BYTE k) {
  AND(absoluteLongIndexedWithX(i,j,k));
}
