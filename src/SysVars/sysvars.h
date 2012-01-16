#ifndef _SYSVARS_H_
#define _SYSVARS_H_
#include <stdint.h>
#include "../preproc.h"
// byteorder
int8_t ByteOrderDetect(void);
// convert from system byteorder to Little-Endian
uint16_t BoSys2LE(uint16_t n);
uint32_t BoSys2LE(uint32_t n);
uint64_t BoSys2LE(uint64_t n);
// convert from Little-Endian byteorder to system
uint16_t BoLE2Sys(uint16_t n);
uint32_t BoLE2Sys(uint32_t n);
uint64_t BoLE2Sys(uint64_t n);
#endif  // _SYSVARS_H_
