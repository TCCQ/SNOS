/*
 * author: Thomas Ulmer
 *
 * This file contains the implementation for the functions in memory.h
 * and more importantly, contains the declarations of mbar and mmap
 * this way, no accesses can be made to them from other files
 * effectively limiting their scope
 *
 * Perhaps I will want to change this to make block moves (MVP/MVN/dma) more efficient? 
 * remains to be seen
 */

#include "memory.h"

//MAIN MEMORY DECLARATION

BYTE mbar[0xFFFFFF]; //24b address space 
ADDRESS mmap[0xFFFFFF]; //same size, for mirroring
//internal access with mbar[mmap[a]] for address a
//allows multiple pointers to same data

//contains wrapping and non-wrapping variants of all calls.
//almost always, a single read/write does *NOT* wrap, but incremeted things in address.h *will* wrap
//some of these may not be used, but I might as well write them just in case

BYTE getByte(ADDRESS a) {
  //out of bounds check
  a &= 0xFFFFFF; //bottom 24b
  return mbar[mmap[a]];
}

void setByte(ADDRESS a, BYTE b) {
  a &= 0xFFFFFF;
  mbar[mmap[a]] = b;
}

WORD getWordWrap(ADDRESS a) {
  a & 0xFFFFFF;
  return (WORD)mbar[mmap[((a+1) & 0xFFFF) | (a & 0xFF0000)]] << 8 | mbar[mmap[a]];
}

WORD getWordNoWrap(ADDRESS a) {
  a &= 0xFFFFFF;
  return (WORD)mbar[mmap[a+1]] << 8 | mbar[mmap[a]];
}

void setWordWrap(ADDRESS a, WORD w) {
  a &= 0xFFFFFF;
  mbar[mmap[a]] = (BYTE)w;
  mbar[mmap[((a+1) & 0xFFFF) | (a & 0xFF0000)]] = (BYTE)(w >> 8);
}

void setWordNoWrap(ADDRESS a, WORD w) {
  a &= 0xFFFFFF;
  mbar[mmap[a]] = (BYTE)w;
  mbar[mmap[a+1]] = (BYTE)(w >> 8);
}

ADDRESS getAddressWrap(ADDRESS a) {
  a &= 0xFFFFFF;
  ADDRESS bank = a & 0xFF0000
  return (ADDRESS)mbar[mmap[((a+2) & 0xFFFF) | bank]] << 16 | 
    (ADDRESS)mbar[mmap[((a+1) & 0xFFFF) | bank]] << 8 |
    mbar[mmap[a]];
}

ADDRESS getAddressNoWrap(ADDRESS a) {
  a &= 0xFFFFFF;
  return (ADDRESS)mbar[mmap[a+2]] << 16 |
    (ADDRESS)mbar[mmap[a+1]] << 8 |
    mbar[mmap[a]];
}

//location, then data
void setAddressWrap(ADDRESS a, ADDRESS, d) {
  a &= 0xFFFFFF;
  ADDRESS bank = 0xFF0000;
  mbar[mmap[a]] = (BYTE)d;
  mbar[mmap[((a+1) & 0xFFFF) | bank]] = (BYTE)(d >> 8);
  mbar[mmap[((a+2) & 0xFFFF) | bank]] = (BYTE)(d >> 16);
}

void setAddressNoWrap(ADDRESS a, ADDRESS d) {
   a &= 0xFFFFFF;
   mbar[mmap[a]] = (BYTE)d;
   mbar[mmap[a+1]] = (BYTE)(d >> 8);
   mbar[mmap[a+2]] = (BYTE)(d >> 16);
}


//util funcs 
BYTE getN(void) {
 return (P & 0x80) >> 7;
}

void setN(BYTE i) {
 P &= ~0x80;
 P |= (i << 7);
}

BYTE getV(void) {
 return (P & 0x40) >> 6; 
}

void setV (BYTE i) {
 P &= ~0x40;
 P |= (i << 6);
}

BYTE getM(void) {
 return (P & 0x20) >> 5;
}

BYTE setM (BYTE i) {
 P &= ~0x20;
 P |= (i << 5);
}

BYTE getX(void) {
 return (P & 0x10) >> 4;
}

void setX (BYTE i) {
 P &= ~0x10;
 P |= (i << 4);
}

BYTE getD(void) {
 return (P & 0x80) >> 3;
}

void setD (BYTE i) {
 P &= ~0x08;
 P |= (i << 3);
}

BYTE getI(void) {
 return (P & 0x04) >> 2;
}

void getI (BYTE i) {
 P &= ~0x04;
 P |= (i << 2);
}

BYTE getZ(void) {
 return (P & 0x02) >> 1;
}

void setZ (BYTE i) {
 P &= ~0x02;
 P |= (i << 1);
}

BYTE getC(void) {
 return P & 0x01;
}

void setC(void) {
 P &= ~0x01;
 P |= (i);
}


//ignoring emulation mode for now 
void pushByte (BYTE i) {
 setByteNoWrap((ADDRESS)(S--),i);
}

BYTE popByte (void) {
 return getByte((ADDRESS)(++S);
}

void pushWord (WORD i) {
 setWordNoWrap((ADDRESS)S - 1,i);
 S -= 2;
}

WORD popWord(void) {
 WORD out = getWordNoWrap((ADDRESS)S);
 S += 2;
 return out;
}
