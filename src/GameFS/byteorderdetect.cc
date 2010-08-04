#include "byteorderdetect.h"
char ByteOrderDetect(void) {
  char *ptest;
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
    return 0;
  } else if (ptest[0] == 0x34) {
    return 2;
  } else if (ptest[0] == 0x56) {
    return 3;
  } else if (ptest[0] == 0x78) {
    return 1;
  } else {
// WTF&&& O_o
    return -1;
  }
  return 0;
}
