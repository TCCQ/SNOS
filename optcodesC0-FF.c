/*
 * author: Thomas Ulmer
 *
 * File contains last fourth of the optcodes
 */

#include "optcodes.h"
#include "address.h"
#include "memory.h"

//CPY immediate
void optC0 (BYTE, BYTE);
//CMP direct indexed indirect (X)
void optC1 (BYTE);
//REP immediate 
void optC2 (BYTE, BYTE);
//CMP stack relative 
void optC3 (BYTE);
//CPY direct 
void optC4 (BYTE);
//CMP direct 
void optC5 (BYTE);
//DEC direct 
void optC6 (BYTE);
//CMP direct indirect long
void optC7 (BYTE);
//INY implied 
void optC8 (void);
//CMP immediate
void optC9 (BYTE, BYTE);
//DEX implied
void optCA (void);
//WAI implied 
void optCB (void);
//CPY absolute
void optCC (BYTE, BYTE);
//CMP absolute
void optCD (BYTE, BYTE);
//DEC absolute
void optCE (BYTE, BYTE);
//CMP absolute long
void optCF (BYTE, BYTE, BYTE);

//BNE PC relative
void optD0 (BYTE);
//CMP direct indirect indexed (Y)
void optD1 (BYTE);
//CMP direct indirect
void optD2 (BYTE);
//CMP stack relative indirect indexed (Y)
void optD3 (BYTE);
//PEI stack (direct indirect)
void optD4 (BYTE);
//CMP direct indexed (X)
void optD5 (BYTE);
//DEC direct indexed (X)
void optD6 (BYTE);
//CMP direct indirect long indexed (Y)
void optD7 (BYTE);
//CLD implied
void optD8 (void);
//CMP abolsute indexed (Y)
void optD9 (BYTE, BYTE);
//PHX stack 
void optDA (void);
//STP implied 
void optDB (void);
//JMP absolute indirect long
void optDC (BYTE, BYTE);
//CMP absolute indexed (X)
void optDD (BYTE, BYTE);
//DEC absolute indexed (X)
void optDE (BYTE, BYTE);
//CMP absolute long indexed (X0
void optDF (BYTE, BYTE, BYTE);

//CPX immediate
void optE0 (BYTE, BYTE);
//SBC direct indexed indirect x 
void optE1 (BYTE);
//SEP immediate 
void optE2 (BYTE);
//SBC stack relative 
void optE3 (BYTE);
//CPX direct 
void optE4 (BYTE);
//SBC direct 
void optE5 (BYTE);
//INC direct 
void optE6 (BYTE);
//SBC direct indirect long 
void optE7 (BYTE);
//INX implied 
void optE8 (void);
//SBC immediate 
void optE9 (BYTE, BYTE);
//NOP implied
void optEA (void);
//XBA implied 
void optEB (void);
//CPX absolute
void optEC (BYTE, BYTE);
//SBC absolute 
void optED (BYTE, BYTE);
//INC absolute
void optEE (BYTE, BYTE);
//SBC absolute long 
void optEF (BYTE, BYTE, BYTE);

//BEQ program counter relative
void optF0 (BYTE);
//SBC direct indirect indexed Y
void optF1 (BYTE);
//SBC direct indirect 
void optF2 (BYTE);
//SBC stack relative indirect indexed Y 
void optF3 (BYTE);
//PEA (immediate) stack 
void optF4 (BYTE, BYTE);
//SBC direct indexed X
void optF5 (BYTE);
//INC direct indexed X
void optF6 (BYTE);
//SBC direct indirect long indexed Y
void optF7 (BYTE);
//SED implied 
void optF8 (void);
//SBC absolute indexed Y 
void optF9 (BYTE, BYTE);
//PLX stack 
void optFA (void);
//XCE implied 
void optFB (void);
//JSR absolute indexed indirect 
void optFC (BYTE, BYTE);
//SBC absoluted indexed 
void optFD (BYTE, BYTE);
//INC absolute indexed X
void optFE (BYTE, BYTE);
//SBC absolute long indexed X 
void optFF (BYTE, BYTE, BYTE);

