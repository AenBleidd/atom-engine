#ifndef _BYTEORDERDETECT_H_
#define _BYTEORDERDETECT_H_

// byteorder:
// 0 - BE: 0123
// 1 - LE: 3210
// 2 - MBE: 1032
// 3 - MLE: 2301
char ByteOrderDetect(void);

#endif  // _BYTEORDERDETECT_H_
