#include "sysvars.h"
int8_t ByteOrderDetect(void) {
  int8_t *ptest;
  int32_t test = 0x12345678;
  ptest = (int8_t*)&test;
  if (ptest[0] == 0x12)
    return 0;
  else if (ptest[0] == 0x34)
    return 2;
  else if (ptest[0] == 0x56)
    return 3;
  else if (ptest[0] == 0x78)
    return 1;
// WTF??? O_o
  else
    return -1;
}
// convert from system byteorder to Little-Endian
uint16_t BoSys2LE(uint16_t n) {
  int8_t bo = BYTEORDER;
// nothing to convert ^__^
  if (bo == BO_LE || bo == BO_MLE)
    return n;
  else if (bo == BO_BE || bo == BO_MBE)
    return (HIBYTE(n) + LOBYTE(n) * 256);
// we can only return it
  else
    return n;
}
uint32_t BoSys2LE(uint32_t n) {
  int8_t bo = BYTEORDER;
// nothing to convert ^__^
  if (bo == BO_LE)
    return n;
  else if (bo == BO_BE)
    return (HIBYTE(HIWORD(n)) + LOBYTE(HIWORD(n)) * 256 +
      HIBYTE(LOWORD(n)) * 256 * 256 + LOBYTE(LOWORD(n)) * 256 * 256 * 256);
// I hope we will never use this
  else if (bo == BO_MBE)
    return (HIBYTE(LOWORD(n)) + LOBYTE(LOWORD(n)) * 256 +
      HIBYTE(HIWORD(n)) * 256 * 256 + LOBYTE(HIWORD(n)) * 256 * 256 * 256);
  else if (bo == BO_MLE)
    return (LOBYTE(HIWORD(n)) + HIBYTE(HIWORD(n)) * 256 +
      LOBYTE(LOWORD(n)) * 256 * 256 + HIBYTE(LOWORD(n)) * 256 * 256 * 256);
// we can only return it
  else
    return n;
}
uint64_t BoSys2LE(uint64_t n) {
  int8_t bo = BYTEORDER;
// nothing to convert ^__^
  if (bo == BO_LE)
    return n;
// I hope we will never use this
  else if (bo == BO_BE || bo == BO_MBE)
    return (BoSys2LE(HIDWORD(n)) + BoSys2LE(LODWORD(n)) *
      256 * 256 * 256 * 256);
  else if (bo == BO_MLE)
    return (BoSys2LE(LODWORD(n)) + BoSys2LE(HIDWORD(n)) *
      256 * 256 * 256 * 256);
  else
    return n;
}
// convert from Little-Endian byteorder to system
uint16_t BoLE2Sys(uint16_t n) {
  return BoSys2LE(n);
}
uint32_t BoLE2Sys(uint32_t n) {
  int8_t bo = BYTEORDER;
// nothing to convert ^__^
  if (bo == BO_LE)
    return n;
  else if (bo == BO_BE)
    return BoSys2LE(n);
// I hope we will never use this
  else if (bo == BO_MBE)
    return (LOBYTE(HIWORD(n)) + HIBYTE(HIWORD(n)) * 256 +
      LOBYTE(LOWORD(n)) * 256 * 256 + HIBYTE(LOWORD(n)) * 256 * 256 * 256);
  else if (bo == BO_MLE)
    return (HIBYTE(LOWORD(n)) + LOBYTE(LOWORD(n)) * 256 +
      HIBYTE(HIWORD(n)) * 256 * 256 + LOBYTE(HIWORD(n)) * 256 * 256 * 256);
  else
    return n;
}
uint64_t BoLE2Sys(uint64_t n) {
  int8_t bo = BYTEORDER;
// nothing to convert ^__^
  if (bo == BO_LE)
    return n;
// I hope we will never use this
  else if (bo == BO_BE || bo == BO_MBE)
    return (BoLE2Sys(HIDWORD(n)) + BoLE2Sys(LODWORD(n)) *
      256 * 256 * 256 * 256);
  else if (bo == BO_MLE)
    return (BoLE2Sys(LODWORD(n)) + BoLE2Sys(HIDWORD(n)) *
      256 * 256 * 256 * 256);
  else
    return n;
}
