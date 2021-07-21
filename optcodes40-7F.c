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
void opt42 (BYTE i);

//EOR Stack relative
void opt43 (BYTE i) {
  EOR(stackRelative(i));
}

//MVP block move 
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
void opt49 (BYTE i, BYTE j);

//LSR accumulator
void opt4A (void);

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

//ROR direct //TODO fix, takes 1 byte
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
void opt69 (BYTE i);

//ROR accumulator 
void opt6A (void);

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