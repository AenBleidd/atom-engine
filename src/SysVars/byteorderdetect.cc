#include "sysvars.h"
int8_t ByteOrderDetect(void) {
  int8_t *ptest;
  int8_t byteorder = -1;
  int32_t test = 0x12345678;
  ptest = (int8_t*)&test;
  if (ptest[0] == 0x12) {
    byteorder = 0;
  } else if (ptest[0] == 0x34) {
    byteorder = 2;
  } else if (ptest[0] == 0x56) {
    byteorder = 3;
  } else if (ptest[0] == 0x78) {
    byteorder = 1;
  } else {
// WTF??? O_o
    return -1;
  }
  return byteorder;
}
// convert from system byteorder to Little-Endian
uint16_t BoSys2LE(uint16_t n) {
  return n;
}
uint32_t BoSys2LE(uint32_t n) {
  return n;
}
uint64_t BoSys2LE(uint64_t n) {
  return n;
}
// convert from Little-Endian byteorder to system
uint16_t BoLE2Sys(uint16_t n) {
  return n;
}
uint32_t BoLE2Sys(uint32_t n) {
  return n;
}
uint64_t BoLE2Sys(uint64_t n) {
  return n;
}