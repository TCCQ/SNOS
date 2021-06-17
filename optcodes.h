/* SNOS - SNES emulator
 * author: Thomas Ulmer
 *
 * This is a header file for opt-codes and other internal cpu operations 
 */

//         DOUBLE CHECK !!!! I believe the 65c816 is BIG-ENDIAN 
//         This seems to be true as far as I can tell
//TODO emulated/8-bit/non-native mode
//     will require either a check each time each optcode is processed, or two versions of most opt codes
//     not sure where to put this as a priority

//TODO figure out what is going on with B/C registers. Strong connection to emulation mode I think
//looks like C is another name for A when in 16b mode
//B is the name of the upper byte of A when in 8b mode. It is not destroyed, and XBA can swap them
//upper parts of X,Y are wiped when 8b mode starts 

#ifndef BYTE 
typedef unsigned char BYTE;
#endif

#ifndef WORD 
typedef unsigned short int WORD;
#endif

#ifndef ADDRESS 
typedef unsigned long int ADDRESS; //top byte must be 0x00 
#endif 

/* interal registers                               B REGISTER ??
 * A
 * DB (data bank)
 * D (direct, offset for direct addressing)
 * X,Y
 * P (processor status)
 * PB (Program bank)
 * PC (Program counter)
 * S (stack pointer)
 */
//in native mode banks are 8 bits and all others are 16 

WORD A;
WORD X;
WORD Y;

WORD D;
WORD PC;
WORD S;

BYTE P;
BYTE PB;
BYTE DB;

//TODO confim that these point to the right place/ mirror changes properly 
//not sure if this works 
//assumes big endian

BYTE & AH = &(A);
BYTE & AL = &(A+1);  
BYTE & XH = &(X);
BYTE & XL = &(X+1);  
BYTE & YH = &(Y);
BYTE & YL = &(Y+1);  
BYTE & DH = &(D);
BYTE & DL = &(D+1);  
BYTE & PCH = &(PC);
BYTE & PCL = &(PC+1);  
BYTE & SH = &(S);
BYTE & SL = &(S+1);  

//TODO define signals 

/*
 * 24 ways of adressing memory 
 * I believe just addresses the 16MB 0x000000-0xFFFFFF builtin ram
 * comment after declaration is the abbreviation for that address mode 
 */
ADDRESS immediate (BYTE, BYTE); //#       !!!! optcode followed by a constant, in code. maybe pass PC + 1 or something?
ADDRESS absolute (BYTE, BYTE); //a
ADDRESS absoluteLong (BYTE, BYTE, BYTE); //al
ADDRESS direct (BYTE); //d
ADDRESS accumulator (void); //A         !!!! acts on REGISTER A, not on memory  ||  this and below are mostly placeholers
ADDRESS implied (void); //i               !!!! not sure if I need a function for this, don't think it would ever get called
ADDRESS directIndirectIndexed (BYTE); //(d),y
ADDRESS directIndirectIndexedLong (BYTE); //[d],y
ADDRESS directIndexedIndirect (BYTE); //(d,x)
ADDRESS directIndexedWithX (BYTE); //d,x
ADDRESS directIndexedWithY (BYTE); //d,y
ADDRESS absoluteIndexedWithX (BYTE, BYTE); //a,x
ADDRESS absoluteIndexedWithY (BYTE, BYTE); //a,y
ADDRESS absoluteLongIndexedWithX (BYTE, BYTE, BYTE); //al,x
ADDRESS programCounterRelative (BYTE); //r
ADDRESS programCounterRelativeLong (BYTE, BYTE); //rl
ADDRESS absoluteIndirect (BYTE, BYTE); //(a)
ADDRESS directIndirect (BYTE); //(d)
ADDRESS directIndirectLong (BYTE); //[d]
ADDRESS absoluteIndexedIndirect (BYTE, BYTE); //(a,x)
ADDRESS stack (void); //s                  !!!!!not sure, used for push/pull operations
ADDRESS stackRelative (BYTE); //d,s
ADDRESS stackRelativeIndirectIndexed (BYTE); //(d,s),y
ADDRESS[2] blockSourceBankDestinationBank (BYTE, BYTE); //xyc
/*
 * end of addressing mode declarations  
 */


//the plan is for each instruction to have its own function (with an address as an argument for most of them)
//and then have each optcode be a combination of one of the above addressing modes and an instruction function 
//so like optXX would be some hex opt-code which would be implimented as one of the above address calculators with that result passed to the calculation defined by the instruction 
//each instruction needs a function (I guess named by their mnemonic) that takes 0-2 addresses as needed 
//and each optcode needs one function that combines the right addressing style with the right calculation 

//remember to set processor flag bits on relevant instructions. 
//some instructions have constant versions, or change a register OR a memory location
//  for said instructions, I am trying to avoid spliting them, but am not sure what to do
//  maybe pass some out of bounds address? like 0xFF000000 or something, use as an error/special code

void ADC (ADDRESS); //Add Memory to Accumulator with Carry
void AND (ADDRESS); //"AND" Memory with Accumulator
void ASL (ADDRESS); //Shift One Bit Left, Memory or Accumulator
void BCC (ADDRESS); //Branch on Carry Clear (Pe = O)
void BCS (ADDRESS); //Branch on Carry Set (Pe = 1)
void BEQ (ADDRESS); //Branch if Equal (Pz = 1)
void BIT (ADDRESS); //Bit Test
void BMI (ADDRESS); //Branch if Result Minus (PN = 1)
void BNE (ADDRESS); //Branch if Not Equal (Pz = 0)
void BPL (ADDRESS); //Branch if Result Plus (PN = 0)
void BRA (ADDRESS); //Branch Always
void BRK (ADDRESS); //Force Break                         !!!!!!!!! not sure if this takes an address or not
void BRL (ADDRESS); //Branch Always Long
void BVC (ADDRESS); //Branch on Overflow Clear (Pv = 0)
void BVS (ADDRESS); //Branch on Overflow Set (Pv = 1)
void CLC (void); //Clear Carry Flag                       !!! implied, not sure if I need a address func
void CLD (void); //Clear Decimal Mode
void CLI (void); //Clear Interrupt Disable Bit
void CLV (void); //Clear Overflow Flag
void CMP (ADDRESS); //Compare Memory and Accumulator
void COP (); //Coprocessor                                !!!! takes a constant. return to
void CPX (ADDRESS); //Compare Memory and Index X
void CPY (ADDRESS); //Compare Memory and Index Y
void DEC (ADDRESS); //Decrement Memory or Accumulator by One  !!!! unclear if uses A address or accum.-- maybe seperate versions?
void DEX (void); //Decrement Index X by One               !!!! implied
void DEY (void); //Decrement Index Y by One
void EOR (ADDRESS); //Exclusive "OR" Memory with Accumulator 
void INC (ADDRESS); //Increment Memory or Accumulator by One     !!!! see DEC
void INX (void); //Increment Index X by One
void INY (void); //Increment Index Y by One
void JML (ADDRESS); //Jump Long
void JMP (ADDRESS); //Jump to New Location
void JSL (ADDRESS); //Jump Subroutine Long
void JSR (ADDRESS); //Jump to New Location Saving Return Address
void LDA (ADDRESS); //Load Accumulator with Memory
void LDX (ADDRESS); //Load Index X with Memory
void LDY (ADDRESS); //Load Index Y with Memory                  !!!! think about how to handle immediate addresses (constants)
void LSR (ADDRESS); //Shift One Bit Right (Memory or Accumulator) !!! more accumulator troubles
void MVN (ADDRESS[2]); //Block Move Negative              !!! see xyc, block address
void MVP (ADDRESS[2]); //Block Move Positive
void NOP (void); //No Operation                          
void ORA (ADDRESS); //"OR" Memory with Accumulator
void PEA (ADDRESS); //Push Effective Absolute Address on Stack (or Push Immediate Data on Stack)
void PEI (ADDRESS); //Push Effective Indirect Address on Stack (add one cycle if DL f 0)
void PER (ADDRESS); //Push Effective Program Counter Relative Address on Stack
void PHA (void); //Push Accumulator on Stack
void PHB (void); //Push Data Bank Register on Stack
void PHD (void); //Push Direct Register on Stack
void PHK (void); //Push Program Bank Register on Stack
void PHP (void); //Push Processor Status on Stack
void PHX (void); //Push Index X on Stack
void PHY (void); //Push index Y on Stack
void PLA (void); //Pull Accumulator from Stack
void PLB (void); //Pull Data Bank Register from Stack
void PLD (void); //Pull Direct Register from Stack
void PLP (void); //Pull Processor Status from Stack
void PLX (void); //Pull Index X from Stack
void PLY (void); //Pull Index Y form Stack
void REP (); //Reset Status Bits                         !!!!immediate (constant)
void ROL (ADDRESS); //Rotate One Bit Left (Memory or Accumulator)     !!!accumulator trouble
void ROR (ADDRESS); //Rotate One Bit Right (Memory or Accumulator)
void RTI (); //Return from Interrupt                     !!!! use stack address I guess. return to
void RTL (); //Return from Subroutine Long
void RTS (); //Return from Subroutine
void SBC (ADDRESS); //Subtract Memory from Accumulator with Borrow
void SEC (void); //Set Carry Flag                        
void SED (void); //Set Decimal Mode
void SEI (void); //Set Interrupt Disable Status
void SEP (); //Set Processor Status Bits              !!!!! immediate (constant)
void STA (ADDRESS); //Store Accumulator in Memory
void STP (void); //Stop the Clock
void STX (ADDRESS); //Store Index X in Memory
void STY (ADDRESS); //Store Index Y in Memory
void STZ (ADDRESS); //Store Zero in Memory
void TAX (void); //Transfer Accumulator to Index X
void TAY (void); //Transfer Accumulator to Index Y
void TCD (void); //Transfer Accumulator to Direct Register
void TCS (void); //Transfer Accumulator to Stack Pointer Register
void TDC (void); //Transfer Direct Register to Accumulator
void TRB (ADDRESS); //Test and Reset Bit
void TSB (ADDRESS); //Test and Set Bit
void TSC (void); //Transfer Stack Pointer Register to Accumulator
void TSX (void); //Transfer Stack Pointer Register to Index X
void TXA (void); //Transfer Index X to Accumulator
void TXS (void); //Transfer Index X to Stack Polnter Register
void TXY (void); //Transfer Index X to Index Y
void TYA (void); //Transfer Index Y to Accumulator
void TYX (void); //Transfer Index Y to Index X
void WAI (void); //Wait for Interrupt
void XBA (void); //Exchange AH and AL
void XCE (void); //Exchange Carry and Emulation Bits     !!!! B register and C register. looks like C = A | B << 8. find out more 

//TODO double check number of arguments to optcodes (can do when writing implimentation 
// # arguments decided by addressing mode

//immediate addressing opts (constants) should take 2 when regs. are 16b, and 1 when 8b. They should be declared with 2 here

/* begin functions for optcodes 
 * combination of address style (where applicable) and instruction 
 */
void opt00 (BYTE);
void opt01 (BYTE);
void opt02 (BYTE);
void opt03 (BYTE);
void opt04 (BYTE);
void opt05 (BYTE);
void opt06 (BYTE);
void opt07 (BYTE);
void opt08 (void);
void opt09 (BYTE, BYTE);
void opt0A (void);
void opt0B (void);
void opt0C (BYTE, BYTE);
void opt0D (BYTE, BYTE);
void opt0E (BYTE, BYTE);
void opt0F (BYTE, BYTE, BYTE);

void opt10 (BYTE);
void opt11 (BYTE);
void opt12 (BYTE);
void opt13 (BYTE);
void opt14 (BYTE);
void opt15 (BYTE);
void opt16 (BYTE);
void opt17 (BYTE);
void opt18 (void);
void opt19 (BYTE, BYTE);
void opt1A (void);
void opt1B (void);
void opt1C (BYTE, BYTE);
void opt1D (BYTE, BYTE);
void opt1E (BYTE, BYTE);
void opt1F (BYTE, BYTE, BYTE);

void opt20 (BYTE, BYTE);
void opt21 (BYTE);
void opt22 (BYTE, BYTE, BYTE);
void opt23 (BYTE);
void opt24 (BYTE);
void opt25 (BYTE);
void opt26 (BYTE);
void opt27 (BYTE);
void opt28 (void);
void opt29 (BYTE);
void opt2A (void);
void opt2B (void);
void opt2C (BYTE, BYTE);
void opt2D (BYTE, BYTE);
void opt2E (BYTE, BYTE);
void opt2F (BYTE, BYTE, BYTE);

void opt30 (BYTE);
void opt31 (BYTE);
void opt32 (BYTE);
void opt33 (BYTE);
void opt34 (BYTE);
void opt35 (BYTE);
void opt36 (BYTE);
void opt37 (BYTE);
void opt38 (void);
void opt39 (BYTE, BYTE);
void opt3A (void);
void opt3B (void);
void opt3C (BYTE, BYTE);
void opt3D (BYTE, BYTE);
void opt3E (BYTE, BYTE);
void opt3F (BYTE, BYTE, BYTE);

void opt40 (void);
void opt41 (BYTE);
void opt42 (BYTE);
void opt43 (BYTE);
void opt44 (BYTE, BYTE);
void opt45 (BYTE);
void opt46 (BYTE);
void opt47 (BYTE);
void opt48 (void);
void opt49 (BYTE, BYTE);
void opt4A (void);
void opt4B (void);
void opt4C (BYTE, BYTE);
void opt4D (BYTE, BYTE);
void opt4E (BYTE, BYTE);
void opt4F (BYTE, BYTE, BYTE);

void opt50 (BYTE);
void opt51 (BYTE);
void opt52 (BYTE);
void opt53 (BYTE);
void opt54 (BYTE, BYTE);
void opt55 (BYTE);
void opt56 (BYTE);
void opt57 (BYTE);
void opt58 (void);
void opt59 (BYTE, BYTE);
void opt5A (void);
void opt5B (void);
void opt5C (voidBYTE, BYTE, BYTEBYTE, BYTE);
void opt5D (BYTE, BYTE);
void opt5E (BYTE, BYTE);
void opt5F (BYTE, BYTE, BYTE);

void opt60 (void);
void opt61 (BYTE);
void opt62 (BYTE, BYTE);
void opt63 (BYTE);
void opt64 (BYTE);
void opt65 (BYTE);
void opt66 (void);
void opt67 (BYTE);
void opt68 (void);
void opt69 (BYTE);
void opt6A (void);
void opt6B (void);
void opt6C (BYTE, BYTE);
void opt6D (BYTE, BYTE);
void opt6E (BYTE, BYTE);
void opt6F (BYTE, BYTE, BYTE);

void opt70 (BYTE);
void opt71 (BYTE);
void opt72 (BYTE);
void opt73 (BYTE);
void opt74 (BYTE);
void opt75 (BYTE);
void opt76 (BYTE);
void opt77 (BYTE);
void opt78 (void);
void opt79 (BYTE, BYTE);
void opt7A (void);
void opt7B (void);
void opt7C (BYTE, BYTE);
void opt7D (BYTE, BYTE);
void opt7E (BYTE, BYTE);
void opt7F (BYTE, BYTE, BYTE);

void opt80 (BYTE);
void opt81 (BYTE);
void opt82 (BYTE, BYTE);
void opt83 (BYTE);
void opt84 (BYTE);
void opt85 (BYTE);
void opt86 (BYTE);
void opt87 (BYTE);
void opt88 (void);
void opt89 (BYTE, BYTE);
void opt8A (void);
void opt8B (void);
void opt8C (BYTE, BYTE);
void opt8D (BYTE, BYTE);
void opt8E (BYTE, BYTE);
void opt8F (BYTE, BYTE, BYTE);

void opt90 (BYTE);
void opt91 (BYTE);
void opt92 (BYTE);
void opt93 (BYTE);
void opt94 (BYTE);
void opt95 (BYTE);
void opt96 (BYTE);
void opt97 (BYTE);
void opt98 (void);
void opt99 (BYTE);
void opt9A (void);
void opt9B (void);
void opt9C (BYTE, BYTE);
void opt9D (BYTE, BYTE);
void opt9E (BYTE, BYTE);
void opt9F (BYTE, BYTE, BYTE);

void optA0 (BYTE, BYTE);
void optA1 (BYTE);
void optA2 (BYTE, BYTE);
void optA3 (BYTE);
void optA4 (BYTE);
void optA5 (BYTE);
void optA6 (BYTE);
void optA7 (BYTE);
void optA8 (void);
void optA9 (BYTE, BYTE);
void optAA (void);
void optAB (void);
void optAC (BYTE, BYTE);
void optAD (BYTE, BYTE);
void optAE (BYTE, BYTE);
void optAF (BYTE, BYTE, BYTE);

void optB0 (BYTE);
void optB1 (BYTE);
void optB2 (BYTE);
void optB3 (BYTE);
void optB4 (BYTE);
void optB5 (BYTE);
void optB6 (BYTE);
void optB7 (BYTE);
void optB8 (void);
void optB9 (BYTE, BYTE);
void optBA (void);
void optBB (void);
void optBC (BYTE, BYTE);
void optBD (BYTE, BYTE);
void optBE (BYTE, BYTE);
void optBF (BYTE, BYTE, BYTE);

void optC0 (BYTE, BYTE);
void optC1 (BYTE);
void optC2 (BYTE, BYTE);
void optC3 (BYTE);
void optC4 (BYTE);
void optC5 (BYTE);
void optC6 (BYTE);
void optC7 (BYTE);
void optC8 (void);
void optC9 (BYTE, BYTE);
void optCA (void);
void optCB (void);
void optCC (BYTE, BYTE);
void optCD (BYTE, BYTE);
void optCE (BYTE, BYTE);
void optCF (BYTE, BYTE, BYTE);

void optD0 (BYTE);
void optD1 (BYTE);
void optD2 (BYTE);
void optD3 (BYTE);
void optD4 (BYTE);
void optD5 (BYTE);
void optD6 (BYTE);
void optD7 (BYTE);
void optD8 (void);
void optD9 (BYTE, BYTE);
void optDA (void);
void optDB (void);
void optDC (BYTE, BYTE);
void optDD (BYTE, BYTE);
void optDE (BYTE, BYTE);
void optDF (BYTE, BYTE, BYTE);

void optE0 (BYTE, BYTE);
void optE1 (BYTE);
void optE2 (BYTE);
void optE3 (BYTE);
void optE4 (BYTE);
void optE5 (BYTE);
void optE6 (BYTE);
void optE7 (BYTE);
void optE8 (void);
void optE9 (BYTE, BYTE);
void optEA (void);
void optEB (void);
void optEC (BYTE, BYTE);
void optED (BYTE, BYTE);
void optEE (BYTE, BYTE);
void optEF (BYTE, BYTE, BYTE);

void optF0 (BYTE);
void optF1 (BYTE);
void optF2 (BYTE);
void optF3 (BYTE);
void optF4 (BYTE, BYTE);
void optF5 (BYTE);
void optF6 (BYTE);
void optF7 (BYTE);
void optF8 (void);
void optF9 (BYTE, BYTE);
void optFA (void);
void optFB (void);
void optFC (BYTE, BYTE);
void optFD (BYTE, BYTE);
void optFE (BYTE, BYTE);
void optFF (BYTE, BYTE, BYTE);
//end optcode functions 

