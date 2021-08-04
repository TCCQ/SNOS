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
         * TODO think about how to truely prevent collisions, this will not fix getting past this point and then having hdma happen
         * as above, possible solution is to have each byte of dma be itself a signal call,
         * it seems that posix does not allow nested signal handler interupts, so hdma would wait until after the data trasfer for 1 byte is done
         * might just be a combination of getpid() and kill(pid, usrInt)
         * set single byte dma to handler for usrInt
         * this is not the intended use of signals, and horribly inefficient, as it nessesitates multiple systemcalls for every byte to be transfered
         * think of something better
         * 
         * other thing:
         * the actual handler function for hblank could not call hdma but instead set some flag for it to be called after
         * so the handler checks/performs dma/hdma termination, sets the flag for hdma to happen, and then returns
         * execution on this dma finishes the byte, gets terminated 
         * have caller check hdma flag and call as nessisary
         * this would complicate the simplicity of the sigalarm hblank solution, as it would require hdma flag checks whenever hdma could have happened
         * 
         * AH!
         * what about doing it the other way, instead
         * have a 'running conflicting dma' flag
         * hblank is triggered, check for hdma/dma collitions, if not, run hdma
         * it they do, then terminate dma and set running conflicting dma flag
         * return to dma func, it terminates, IT CHECKS CONFLICT FLAG, if set, run hdma
         * conflict flag should be channel specific, so that extra hdma ticks don't happen on nonconflicting channels 
         * this requires the doHdma to be channel specific, but that is an easy change
         *
         */
        if (!(getByte(DMA_ENABLE_REG) & (0x01 << ch))) {
          return; //got terminated by hdma
        }
        //do whatever


      }
    }
  }
}
