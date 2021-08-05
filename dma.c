/*
 * author:Thomas Ulmer
 *
 * implementations of things declared in dma.h
 */

#include "dma.h"
#include "memory.h"


void doDMA(void) {
  for (int ch = 0; ch < 8; ch++) {
    //channels, decreasing in priority
    if (getByte(DMA_ENABLE_REG) & (0x01 << ch)) {
      //do ch dma 
      BYTE chNum = ch << 4;
      //get reg with MASK | (ch << 4)
      
      ADDRESS port = 0x2100 | getByte(DMA_DESTINATION_MASK | chNum);
      //hardware reg to write/read 
      
      ADDRESS aAddr = getByte(DMA_SOURCE_ADDRESS_BANK_MASK | chNum) << 16 |
                      getByte(DMA_SOURCE_ADDRESS_HIGH_MASK | chNum) << 8  |
                      getByte(DMA_SOURCE_ADDRESS_LOW_MASK  | chNum);
      //absolute long addr in memory/cartriage 

      WORD* count = (WORD*)getPtr(DMA_SIZE_LOW_MASK | chNum);
      if (*count == 0) *count = 65536;
      while (*count != 0) { 
        /*
         * thinking about how to prevent dma/hdma collitions
         *
         * simple and smart would be to simply block hblank operations for each tick of dma
         * then unblock
         * see: https://www.gnu.org/software/libc/manual/html_node/Remembering-a-Signal.html
         * this will cause slight variance in timing of scan lines, but it will be extremely slight, so I think it is fine
         * they come significantly faster than most modern monitors will display anyway, so I am not worried
         */
        if (!(getByte(DMA_ENABLE_REG) & (0x01 << ch))) {
          return; //got terminated by hdma
        }
        //do whatever


      }
    }
  }
}
