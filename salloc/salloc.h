#ifndef _SALLOC_H
#define _SALLOC_H
void *  SRAMalloc( unsigned char nBytes);
void SRAMfree(void *  pSRAM);
void SRAMInitHeap(void);
#endif