/* 
 * author: Thomas Ulmer
 * 
 * This file should contain the main timing features and explanations on the general workflow
 */
#include "optcodes.h"
#include "memory.h"

#ifndef BYTE 
typedef unsigned char BYTE;
#endif

#ifndef WORD 
typedef unsigned short int WORD;
#endif

#ifndef ADDRESS 
typedef unsigned long int ADDRESS; //top byte must be 0x00 
#endif 

/*
 * ok so the idea is to lump operations together to avoid unessiary function calls
 * so the instruction should happen (in one function call) and then when it is complete
 * check for interupts
 * check for dma (order with the above?)
 *
 * then seperately, have a sigalarm call that triggers when each scanline should be ready. 
 * the handler function then does the screen update, checks and performs hdma, and checks and queues an interupt if H-blank is enabled
 * have a counter, and as necessary use it to keep track of when V-blank happens. 
 *
 * TODO think about how to handle midline interupts (maybe a second sigalarm?)
 * or pass some data through a global variable about what sort of alarm interupt this is: midline, or full-line
 * (note that by rendering whole lines at a time, I can avoid calling SDL renderer updates with small changes)
 * note that full emulation of crt-esque graphics is likely impossible for this kind of project, as it would necessitate a screen update every pixel
 * also many modern systems would likely lump those changes into less frequent larger updates anyway (locked 60hz screens and whatnot)
 */

//0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
const BYTE optLengths[0xFF] = {
  2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  3, 2, 4, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  1, 2, 2, 2, 3, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 3, 2, 2, 2, 1, 3, 1, 1, 4, 3, 3, 4, 
  1, 2, 3, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 3, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  3, 2, 3, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  3, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  3, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
  2, 2, 2, 2, 3, 2, 2, 2, 1, 3, 1, 1, 3, 3, 3, 4, 
 }
//see cols at top, and use line-numbers on left to find optcode (row major order)
//length includes optcode itself, so implied mode will be 1
//base length is the maximum length (assumes 16b registers), check later and decrease
//this is because in 8b cases, I need to pass garbage as 2nd argument, which is ignored later
//this avoids having 2 versions of those instructions with differing argument numbers 
//note that brk and cop (00 and 02) take a signature byte 

//allow single type array using an func. ptr with ambiguous arg count 
typedef union {
  (void) (*noArg)(void);
  (void) (*oneArg)(BYTE);
  (void) (*twoArg)(BYTE, BYTE);
  (void) (*threeArg)(BYTE, BYTE, BYTE);
} ambigFPtr; 

//auto cast from f ptr of appropriate type to union ambigFPtr 
ambigFPtr optPtrs[0xFF] = {
  &opt00, &opt01, &opt02, &opt03, &opt04, &opt05, &opt06, &opt07, &opt08, &opt09, &opt0A, &opt0B, &opt0C, &opt0D, &opt0E, &opt0F, 
  &opt10, &opt11, &opt12, &opt13, &opt14, &opt15, &opt16, &opt17, &opt18, &opt19, &opt1A, &opt1B, &opt1C, &opt1D, &opt1E, &opt1F, 
  &opt20, &opt21, &opt22, &opt23, &opt24, &opt25, &opt26, &opt27, &opt28, &opt29, &opt2A, &opt2B, &opt2C, &opt2D, &opt2E, &opt2F, 
  &opt30, &opt31, &opt32, &opt33, &opt34, &opt35, &opt36, &opt37, &opt38, &opt39, &opt3A, &opt3B, &opt3C, &opt3D, &opt3E, &opt3F, 
  &opt40, &opt41, &opt42, &opt43, &opt44, &opt45, &opt46, &opt47, &opt48, &opt49, &opt4A, &opt4B, &opt4C, &opt4D, &opt4E, &opt4F, 
  &opt50, &opt51, &opt52, &opt53, &opt54, &opt55, &opt56, &opt57, &opt58, &opt59, &opt5A, &opt5B, &opt5C, &opt5D, &opt5E, &opt5F, 
  &opt60, &opt61, &opt62, &opt63, &opt64, &opt65, &opt66, &opt67, &opt68, &opt69, &opt6A, &opt6B, &opt6C, &opt6D, &opt6E, &opt6F, 
  &opt70, &opt71, &opt72, &opt73, &opt74, &opt75, &opt76, &opt77, &opt78, &opt79, &opt7A, &opt7B, &opt7C, &opt7D, &opt7E, &opt7F, 
  &opt80, &opt81, &opt82, &opt83, &opt84, &opt85, &opt86, &opt87, &opt88, &opt89, &opt8A, &opt8B, &opt8C, &opt8D, &opt8E, &opt8F, 
  &opt90, &opt91, &opt92, &opt93, &opt94, &opt95, &opt96, &opt97, &opt98, &opt99, &opt9A, &opt9B, &opt9C, &opt9D, &opt9E, &opt9F, 
  &optA0, &optA1, &optA2, &optA3, &optA4, &optA5, &optA6, &optA7, &optA8, &optA9, &optAA, &optAB, &optAC, &optAD, &optAE, &optAF, 
  &optB0, &optB1, &optB2, &optB3, &optB4, &optB5, &optB6, &optB7, &optB8, &optB9, &optBA, &optBB, &optBC, &optBD, &optBE, &optBF, 
  &optC0, &optC1, &optC2, &optC3, &optC4, &optC5, &optC6, &optC7, &optC8, &optC9, &optCA, &optCB, &optCC, &optCD, &optCE, &optCF, 
  &optD0, &optD1, &optD2, &optD3, &optD4, &optD5, &optD6, &optD7, &optD8, &optD9, &optDA, &optDB, &optDC, &optDD, &optDE, &optDF, 
  &optE0, &optE1, &optE2, &optE3, &optE4, &optE5, &optE6, &optE7, &optE8, &optE9, &optEA, &optEB, &optEC, &optED, &optEE, &optEF, 
  &optF0, &optF1, &optF2, &optF3, &optF4, &optF5, &optF6, &optF7, &optF8, &optF9, &optFA, &optFB, &optFC, &optFD, &optFE, &optFF, 
}




void performOptcode(void) {
  BYTE opt = getByte((ADDRESS)PB << 16 | PC);
  BYTE optLen = optLengths[opt];
  BYTE shortenLen = 0;
  if ( opt == 0x09 || opt == 0x29 || opt == 0x49 || opt == 0x69 || 
       opt == 0x89 || opt == 0xA9 || opt == 0xC9 || opt == 0xE9 ) {
    if (getM()) {
      //8b accumulator, need to pull one fewer value
      shortenLen = 1;
    }
  } else if ( opt == 0xA0 || opt == 0xA2 || opt == 0xC0 || opt == 0xE0 ) {
    if (getX()) {
      //8b X/Y, pull 1 fewer val
      shortenLen = 1;
    }
  } 
  BYTE args[3];
  for (int i = 1; i < optLen - shortenLen; i++) {
    args[i-1] = getByte((ADDRESS)PB << 16 | (PC + i));
  }
  //optcode and arguments are saved, safe to inc PC
  PC += optLength - shortenLen;

  /*
   * so the idea is an array indexed by optcode 
   * where each element is a union struct of 
   * 0-3 arg function pointers
   * so to call the optcode, 
   * you go the union in the array, 
   * and depending on the val of optLength
   * deref and execute the function at the proper place in the union with the right arguments
   */
  //NOTE THAT I DO NOT ADJUST FOR SHORTENED LENGTH HERE
  //even in 8b modes, it will still pass 2 arguments, the second of which is uninitialized
  //they will be ignored in the instruction call, as the 8b switch causes the second argument to be ignored
   switch (optLength) {
     case (1):
       *(optPtrs[opt].noArg)(void);
       break;
     case (2):
       *(optPtrs[opt].oneArg)(args[0]);
       break;
     case (3):
       *(optPtrs[opt].twoArg)(args[0], args[1]);
       break;
     case (4):
       *(optPtrs[opt]threeArg)(args[0], args[1], args[2]);
       break;
   }
}

