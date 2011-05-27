#ifndef _PREPROC_H_
#define _PREPROC_H_
// SIC! Engine byteorder is Little-Endian! SIC!
#ifdef _FSMANAGER_
#define _FSMAN_
#define _CRC_CHECK_
#define _FSMAN_VERSION_ _ATOM_VERSION_
#endif  // _FSMANAGER_

// If it will be compiled on MSVC
#ifdef _MSC_VER
// Oh God! I hate Microsoft!..
#define snprintf sprintf_s
#else
#include <inttypes.h>
#endif // _MSC_VER

#include "SysVars/sysvars.h"
// System parameters

// byteorder:
#define BO_BE 0  // 0123
#define BO_LE 1  // 3210
#define BO_MBE 2  // 1032
#define BO_MLE 3  // 2301

#define BYTEORDER ByteOrderDetect()        // default is: Little-Endian

// size of main types
#define SIZEOFWCHAR_T   sizeof(wchar_t)    // default is: 4 bytes

// if they are not defined...
#ifndef LODWORD
#define LODWORD(ll) ((uint32_t)(ll))
#endif
#ifndef HIDWORD
#define HIDWORD(ll) ((uint32_t)(((uint64_t)(ll) >> 32) & 0xFFFFFFFF))
#endif
#ifndef LOWORD
#define LOWORD(l) ((uint16_t)(l))
#endif
#ifndef HIWORD
#define HIWORD(l) ((uint16_t)(((uint32_t)(l) >> 16) & 0xFFFF))
#endif
#ifndef LOBYTE
#define LOBYTE(w) ((uint8_t)(w))
#endif
#ifndef HIBYTE
#define HIBYTE(w) ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))
#endif

#endif  // _PREPROC_H_
