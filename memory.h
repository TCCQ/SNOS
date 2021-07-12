/*
 * author; Thomas Ulmer
 *
 * This file contains the means by which the emulator reads and writes to RAM, ROM, and hardware registers
 * all of which are accessed with a 24b address
 * mirroring, and distinctions between RAM, ROM, and registers will be handled *internally*
 * to the read and write functions, compliant with the dev. manuals and other resources I have found
 * (check the documentation directory)
 */

#ifndef BYTE 
typedef unsigned char BYTE;
#endif

#ifndef WORD 
typedef unsigned short int WORD;
#endif

#ifndef ADDRESS 
typedef unsigned long int ADDRESS; //top byte must be 0x00 
#endif 


//setters and getters, pretty standard
//allows for layer of abstraction, so I can change memory model without redoing everything (again)
//versions with and without wrapping
//will probably mostly use the non-wrapping versions, but its nice to have them 
BYTE getByte(ADDRESS);
void setByte(ADDRESS, BYTE);

WORD getWordWrap(ADDRESS);
WORD getWordNoWrap(ADDRESS);
void setWordWrap(ADDRESS, WORD);
void setWordNoWrap(ADDRESS, WORD);

ADDRESS getAddressWrap(ADDRESS);
ADDRESS getAddressNoWrap(ADDRESS);
void setAddressWrap(ADDRESS, ADDRESS);
void setAddressNoWrap(ADDRESS, ADDRESS);
