/* 
 * author: Thomas Ulmer
 *
 * File contains third fourth of the optcodes
 */

#include "optcodes.h"
#include "address.h"
#include "memory.h"

//BNE PC relative 
void opt80 (BYTE);
//STA direct indexed indirect X 
void opt81 (BYTE);
//BRL PC relative long
void opt82 (BYTE, BYTE);
//STA stack relative 
void opt83 (BYTE);
//STY direct 
void opt84 (BYTE);
//STA direct 
void opt85 (BYTE);
//STX direct 
void opt86 (BYTE);
//STA direct indirect long 
void opt87 (BYTE);
//DEY impied
void opt88 (void);
//BIT immediate 
void opt89 (BYTE, BYTE);
//TXA implied 
void opt8A (void);
//PHB stack :
void opt8B (void);
//STY absolute
void opt8C (BYTE, BYTE);
//STA absolute 
void opt8D (BYTE, BYTE);
//STX absoltue
void opt8E (BYTE, BYTE);
//STA absolute long 
void opt8F (BYTE, BYTE, BYTE);

//BCC program counter relative
void opt90 (BYTE);
//STA direct indirect indexed Y 
void opt91 (BYTE);
//STA direct indirect 
void opt92 (BYTE);
//STA stack realtive indirect indexed Y 
void opt93 (BYTE);
//STY direct indexed X 
void opt94 (BYTE);
//STA direct indexed X 
void opt95 (BYTE);
//STX direct indexed Y 
void opt96 (BYTE);
//STA direct indirect long indexed Y 
void opt97 (BYTE);
//TYA implied 
void opt98 (void);
//STA absolute indexed Y 
void opt99 (BYTE);
//TXS impled 
void opt9A (void);
//TXY implied 
void opt9B (void);
//STZ absolute 
void opt9C (BYTE, BYTE);
//STA Absolute indexed X 
void opt9D (BYTE, BYTE);
//STZ absolute indexed X
void opt9E (BYTE, BYTE);
//STA absolute long indexed X 
void opt9F (BYTE, BYTE, BYTE);

//LDY immediate
void optA0 (BYTE, BYTE);
//LDA direct indexed indirect X
void optA1 (BYTE);
//LDX immediate
void optA2 (BYTE, BYTE);
//LDA stack relative 
void optA3 (BYTE);
//LDY direct 
void optA4 (BYTE);
//LDA direct 
void optA5 (BYTE);
//LDX direct 
void optA6 (BYTE);
//LDA direct indirect long 
void optA7 (BYTE);
//TAY implied 
void optA8 (void);
//LDA immediate 
void optA9 (BYTE, BYTE);
//TAX implied 
void optAA (void);
//PLB stack 
void optAB (void);
//LDY absolute
void optAC (BYTE, BYTE);
//LDA absolute 
void optAD (BYTE, BYTE);
//LDX absolute 
void optAE (BYTE, BYTE);
//LDA absolute long 
void optAF (BYTE, BYTE, BYTE);

//BCS program counter relative
void optB0 (BYTE);
//LDA direct indirect indexed Y
void optB1 (BYTE);
//LDA direct indirect 
void optB2 (BYTE);
//LDA stack relative indirect indexed Y
void optB3 (BYTE);
//LDY direct indexed X
void optB4 (BYTE);
//LDA direct indexed X
void optB5 (BYTE);
//LDX direct indexed Y
void optB6 (BYTE);
//direct indirect long indexed Y
void optB7 (BYTE);
//CLV implied
void optB8 (void);
//LDA absolute indexed Y
void optB9 (BYTE, BYTE);
//TSX implied 
void optBA (void);
//TYX implied 
void optBB (void);
//LDY absolute indexed X
void optBC (BYTE, BYTE);
//LDA absolute indexed X
void optBD (BYTE, BYTE);
//LDX abolsute indexed Y
void optBE (BYTE, BYTE);
//LDA absolute long indexed X
void optBF (BYTE, BYTE, BYTE);
