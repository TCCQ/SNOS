/* 
 * author: Thomas Ulmer
 *
 * File contains third fourth of the optcodes
 */

#include "optcodes.h"
#include "address.h"
#include "memory.h"

//BNE PC relative 
void opt80 (BYTE i) {
  BNE(programCounterRelative(i));
}

//STA direct indexed indirect X 
void opt81 (BYTE i) {
  STA(directIndexedIndirect(i));
}

//BRL PC relative long
void opt82 (BYTE i, BYTE j) {
  BRL(programCounterRelativeLong(i,j));
}

//STA stack relative 
void opt83 (BYTE i) {
  STA(stackRelative(i));
}

//STY direct 
void opt84 (BYTE i) {
  STY(direct(i));
}

//STA direct 
void opt85 (BYTE i) {
  STA(direct(i));
}

//STX direct 
void opt86 (BYTE i) {
  STX(direct(i));
}

//STA direct indirect long 
void opt87 (BYTE i) {
  STA(directIndirectLong(i));
}

//DEY impied
void opt88 (void) {
  DEY();
}

//BIT immediate 
void opt89 (BYTE i, BYTE j) {
  setZ(0);
  setN(0);

  if (getM()) {
    BYTE tmp = *AL & i; 

    if (!(tmp)) setZ(1);
    if (tmp & 0x80) setN(1);
  } else {
    WORD tmp = A & (((WORD)j << 8) | i); 
    
    if (!tmp) setZ(1);
    if (tmp & 0x8000) setN(1);
  }
}

//TXA implied 
void opt8A (void) {
  TXA();
}

//PHB stack :
void opt8B (void) {
  PHB();
}

//STY absolute
void opt8C (BYTE i, BYTE j) {
  STY(absolute(i,j));
}

//STA absolute 
void opt8D (BYTE i, BYTE j) {
  STA(absoltue(i,j));
}

//STX absoltue
void opt8E (BYTE i, BYTE j) {
  STX(absolute(i,j));
}

//STA absolute long 
void opt8F (BYTE i, BYTE j, BYTE k) {
  STA(absoluteLong(i,j,k));
}

//BCC program counter relative
void opt90 (BYTE i) {
  BCC(programCounterRelative(i));
}

//STA direct indirect indexed Y 
void opt91 (BYTE i) {
  STA(directIndirectIndexed(i));
}

//STA direct indirect 
void opt92 (BYTE i) {
  STA(directIndirect(i));
}

//STA stack realtive indirect indexed Y 
void opt93 (BYTE i) {
  STA(stackRelativeIndirectIndexed(i));
}

//STY direct indexed X 
void opt94 (BYTE i) {
  STY(directIndexedWithX(i));
}

//STA direct indexed X 
void opt95 (BYTE i) {
  STA(directIndexedWithX(i));
}

//STX direct indexed Y 
void opt96 (BYTE i) {
  STX(directIndexedWithY(i));
}

//STA direct indirect long indexed Y 
void opt97 (BYTE i) {
  STA(directIndirectIndexedLong(i));
}

//TYA implied 
void opt98 (void) {
  TYA();
}

//STA absolute indexed Y 
void opt99 (BYTE i) {
  STA(absoluteIndexedWithY(i));
}

//TXS impled 
void opt9A (void) {
  TXS();
}

//TXY implied 
void opt9B (void) {
  TXY();
}

//STZ absolute 
void opt9C (BYTE i, BYTE j) {
  STZ(absolute(i,j));
}

//STA Absolute indexed X 
void opt9D (BYTE i, BYTE j) {
  STA(absoluteIndexedWithX(i));
}

//STZ absolute indexed X
void opt9E (BYTE i, BYTE j) {
  STZ(absoluteIndexedWithX(i));
}

//STA absolute long indexed X 
void opt9F (BYTE i, BYTE j, BYTE k) {
  STA(absoluteLongIndexedWithX(i,j,k));
}

//LDY immediate
void optA0 (BYTE i, BYTE j){
  setZ(0);
  setN(0);
  if (getX()) {
    *YL = i; 
    if (!(*YL)) setZ(1);
    if (*YL & 0x80) setN(1);
  } else {
    Y = ((WORD)j << 8) | i; 
    if (!Y) setZ(1);
    if (Y & 0x8000) setN(1);
  }
}

//LDA direct indexed indirect X
void optA1 (BYTE i) {
  LDA(directIndexedIndirect(i));
}

//LDX immediate
void optA2 (BYTE i, BYTE j) {
  setZ(0);
  setN(0);
  if (getX()) {
    *XL = i; 
    if (!(*XL)) setZ(1);
    if (*XL & 0x80) setN(1);
  } else {
    X = ((WORD)j << 8) | i; 
    if (!X) setZ(1);
    if (X & 0x8000) setN(1);
  }
}

//LDA stack relative 
void optA3 (BYTE i) {
  LDA(stackRelative(i));
}

//LDY direct 
void optA4 (BYTE i) {
  LDY(direct(i));
}

//LDA direct 
void optA5 (BYTE i) {
  LDA(direct(i));
}

//LDX direct 
void optA6 (BYTE i) {
  LDX(direct(i));
}

//LDA direct indirect long 
void optA7 (BYTE i) {
  LDA(directIndirectLong(i));
}

//TAY implied 
void optA8 (void) {
  TAY();
}

//LDA immediate 
void optA9 (BYTE i, BYTE j) {
  setZ(0);
  setN(0);
  if (getM()) {
    *AL = i; 
    if (!(*AL)) setZ(1);
    if (*AL & 0x80) setN(1);
  } else {
    A = ((WORD)j << 8) | i; 
    if (!A) setZ(1);
    if (A & 0x8000) setN(1);
  }
}

//TAX implied 
void optAA (void) {
  TAX();
}

//PLB stack 
void optAB (void) {
  PLB();
}

//LDY absolute
void optAC (BYTE i, BYTE j) {
  LDY(absolute(i,j));
}

//LDA absolute 
void optAD (BYTE i, BYTE j) {
  LDA(absolute(i,j));
}

//LDX absolute 
void optAE (BYTE i, BYTE j) {
  LDX(absolute(i,j));
}

//LDA absolute long 
void optAF (BYTE i, BYTE j, BYTE k) {
  LDA(absoluteLong(i,j,k);)
}

//BCS program counter relative
void optB0 (BYTE i) {
  BCS(programCounterRelative(i));
}

//LDA direct indirect indexed Y
void optB1 (BYTE i) {
  LDA(directIndirectIndexed(i));
}

//LDA direct indirect 
void optB2 (BYTE i) {
  LDA(directIndirect(i));
}

//LDA stack relative indirect indexed Y
void optB3 (BYTE i) {
  LDA(stackRelativeIndirectIndexed(i));
}

//LDY direct indexed X
void optB4 (BYTE i) {
  LDY(directIndexedWithX(i));
}

//LDA direct indexed X
void optB5 (BYTE i) {
  LDA(directIndexedWithX(i));
}

//LDX direct indexed Y
void optB6 (BYTE i) {
  LDX(directIndexedWithY(i));
}

//LDA direct indirect long indexed Y
void optB7 (BYTE i) {
  LDA(directIndirectIndexedLong(i));
}

//CLV implied
void optB8 (void) {
  CLV();
}

//LDA absolute indexed Y
void optB9 (BYTE i, BYTE j) {
  LDA(absoluteIndexedWithY(i,j));
}

//TSX implied 
void optBA (void) {
  TSX();
}

//TYX implied 
void optBB (void) {
  TYX();
}

//LDY absolute indexed X
void optBC (BYTE i, BYTE j) {
  LDY(absoluteIndexedWithX(i,j));
}

//LDA absolute indexed X
void optBD (BYTE i, BYTE j) {
  LDA(absoluteIndexedWithX(i,j));
}

//LDX abolsute indexed Y
void optBE (BYTE i, BYTE j) {
  LDA(absoluteIndexedWithY(i,j));
}

//LDA absolute long indexed X
void optBF (BYTE i, BYTE j, BYTE k) {
  LDA(absoluteLongIndexedWithX(i,j,k));
}
