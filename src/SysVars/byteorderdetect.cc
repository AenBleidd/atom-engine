#include "sysvars.h"
char ByteOrderDetect(void) {
  char *ptest;
  char byteorder = -1;
  if (sizeof(long int) == 4) {
    long int test = 0x12345678;
    ptest = (char*)&test;
  } else if(sizeof(int) == 4) {
    int test = 0x12345678;
    ptest = (char*)&test;
  } else {
// WTF??? O_o
    return -1;
  }
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
