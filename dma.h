/*
 * author:Thomas Ulmer
 *
 * this file has functions and declarations for dma and hdma and their registers
 */

#ifndef BYTE
typedef unsigned char BYTE
#endif

#ifndef WORD
typedef unsigned short int WORD
#endif

#ifndef ADDRESS
typedef unsigned long int ADDRESS
#endif


//not a mask, just a value, each channel is 1 bit
#define DMA_ENABLE_REG 0x420B

/*
 * since the registers are of the form 0xRRXR
 * where X is the channel number
 * I will provide masks, and the register can be formed as 
 * mask | (channelNum << 4)
 * channel numbers are 0-7
 */

#define DMA_CONTROL_MASK 0x4300
#define DMA_DESTINATION_MASK 0x4301
#define DMA_SOURCE_ADDRESS_LOW_MASK 0x4302
#define DMA_SOURCE_ADDRESS_HIGH_MASK 0x4303
#define DMA_SOURCE_ADDRESS_BANK_MASK 0x4304
#define DMA_SIZE_LOW_MASK 0x4305
#define DMA_SIZE_HIGH_MASK 0x4306

//similar to DMA, 1 enable reg. and several masks
#define HDMA_ENABLE_REG 0x420C

#define HDMA_CONTROL_MASK 0x4300
#define DHMA_DESTINATION_MASK 0x4301
#define HDMA_TABLE_ADDRESS_LOW_MASK 0x4302
#define HDMA_TABLE_ADDRESS_HIGH_MASK 0x4303
#define HDMA_TABLE_ADDRESS_BANK_MASK 0x4304
#define HDMA_INDIRECT_ADDRESS_LOW_MASK 0x4305
#define HDMA_INDIRECT_ADDRESS_HIGH_MASK 0x4306
#define HDMA_INDIRECT_ADDRESS_BANK_MASK 0x4307
#define HDMA_MIDFRAME_TABLE_ADDRESS_LOW_MASK 0x4308
#define HDMA_MIDFRAME_TABLE_ADDRESS_HIGH_MASK 0x4309
#define HDMA_LINE_COUNTER_MASK 0x430A

//functions that actually do the data transfers
//doHdma will do 1 scanline's worth of transfer, and should be called once per line at the appropriate time 
void doDMA(void);
void doHDMA(void);
