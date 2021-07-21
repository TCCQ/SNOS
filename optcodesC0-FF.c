/*
 * author: Thomas Ulmer
 *
 * File contains last fourth of the optcodes
 */

#include "optcodes.h"
#include "address.h"
#include "memory.h"

//CPY immediate
void optC0 (BYTE i, BYTE j);

//CMP direct indexed indirect (X)
void optC1 (BYTE i) {
  CMP(directIndexedIndirect(i));
}

//REP immediate 
void optC2 (BYTE i, BYTE j);

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
void optC9 (BYTE i, BYTE j);

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
//TODO what happens here?
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
//TODO don't have along version? check instructions.c and jmp vs jml
void optDC (BYTE i, BYTE j);

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
void optE0 (BYTE i, BYTE j);

//SBC direct indexed indirect x 
void optE1 (BYTE i) {
  SBC(directIndexedIndirect(i));
}

//SEP immediate 
void optE2 (BYTE i);

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
void optE9 (BYTE i, BYTE j);

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

//PEA (immediate) stack //TODO this is a constant??
void optF4 (BYTE i, BYTE j);

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
