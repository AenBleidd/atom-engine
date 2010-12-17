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
#define BO_BE 0  // 0123
#define BO_LE 1  // 3210
#define BO_MBE 2  // 1032
#define BO_MLE 3  // 2301

#define BYTEORDER ByteOrderDetect()        // default is: Little-Endian

// size of main types
#define SIZEOFCHAR      sizeof(char)       // default is: 1 byte
#define SIZEOFSHORT     sizeof(short int)  // default is: 2 bytes
#define SIZEOFINT       sizeof(int)        // default is: 4 bytes
#define SIZEOFWCHAR_T   sizeof(wchar_t)    // default is: 4 bytes
#define SIZEOFLONG      sizeof(long)       // default is: 8 bytes
#define SIZEOFLONGLONG  sizeof(long long)  // default is: 8 bytes

#ifndef LOWORD
#define LOWORD(l) ((unsigned short int)(l))
#endif
#ifndef HIWORD
#define HIWORD(l) ((unsigned short int)(((unsigned int)(l) >> 16) & 0xFFFF))
#endif
#ifndef LOBYTE
#define LOBYTE(w) ((unsigned char)(w))
#endif
#ifndef HIBYTE
#define HIBYTE(w) ((unsigned char)(((unsigned short int)(w) >> 8) & 0xFF))
#endif

#endif  // _PREPROC_H_
