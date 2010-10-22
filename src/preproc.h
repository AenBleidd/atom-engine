#ifndef _PREPROC_H_
#define _PREPROC_H_

#ifdef _FSMANAGER_
#define _FSMAN_
#define _CRC_CHECK_
#define _FSMAN_VERSION_ _ATOM_VERSION_
#endif  // _FSMANAGER_

#include "SysVars/sysvars.h"
// System parameters

// byteorder:
// 0 - BE: 0123
// 1 - LE: 3210
// 2 - MBE: 1032
// 3 - MLE: 2301
#define BYTEORDER ByteOrderDetect()        // default is: Little-Endian

// size of main types
#define SIZEOFCHAR      sizeof(char)       // default is: 1 byte
#define SIZEOFSHORT     sizeof(short int)  // default is: 2 bytes
#define SIZEOFINT       sizeof(int)        // default is: 4 bytes
#define SIZEOFLONG      sizeof(long)       // default is: 8 bytes
#define SIZEOFLONGLONG  sizeof(long long)  // default is: 8 bytes
#define SIZEOFWCHAR_T   sizeof(wchar_t)    // default is: 4 bytes

#endif  // _PREPROC_H_
