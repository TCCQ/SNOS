/*
 * author: Thomas Ulmer
 * Definitions for the addressing functions
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


//registers 
extern WORD A;
extern WORD X;
extern WORD Y;

extern WORD D;
extern WORD PC;
extern WORD S;

extern BYTE P;
extern BYTE E;
extern BYTE PB;
extern BYTE DB;

extern BYTE* AL;
extern BYTE* AH;  
extern BYTE* XL;
extern BYTE* XH;  
extern BYTE* YL;
extern BYTE* YH;  
extern BYTE* DL;
extern BYTE* DH;  
extern BYTE* PCL;
extern BYTE* PCH;  
extern BYTE* SL;
extern BYTE* SH;  


/*
 * 24 ways of adressing memory 
 * comment after declaration is the abbreviation for that address mode 
 * some of these are dummy declarations that will not be called, here for posterity 
 */
ADDRESS immediate (BYTE, BYTE); //#      
ADDRESS absolute (BYTE, BYTE); //a
ADDRESS absoluteLong (BYTE, BYTE, BYTE); //al
ADDRESS direct (BYTE); //d
ADDRESS accumulator (void); //A         
ADDRESS implied (void); //i            
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
ADDRESS stack (void); //s             
ADDRESS stackRelative (BYTE); //d,s
ADDRESS stackRelativeIndirectIndexed (BYTE); //(d,s),y
ADDRESS[2] blockSourceBankDestinationBank (BYTE, BYTE); //xyc
