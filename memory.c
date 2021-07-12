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
