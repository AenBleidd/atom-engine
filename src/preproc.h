#ifndef _PREPROC_H_
#define _PREPROC_H_
// SIC! Engine byteorder is Little-Endian! SIC!
#ifdef _FSMANAGER_
#define _FSMAN_
#define _CRC_CHECK_
#define _FSMAN_VERSION_ _ATOM_VERSION_
#endif  // _FSMANAGER_

// Microsoft Security File
// TODO(Lawliet): I don't want to use it now. 
// #include "banned.h"

// Check environment
#ifdef WINDOWS
// we works on windows
#ifdef _MSC_VER
// we compile engine using Microsoft Visual C/C++
#elif __BORLANDC__
// we compile engine using Borland Builder
#else
// unsupported compiler
#warning "Unsupported compiler. A.T.O.M Engine can be built using \
Microsoft Visual C++ or Borland C++ Builder. You can build it \
with another compiler on your own risk without any guaranties."
#endif  // windows compiler
#elif UNIX
// we works on linux
#else
// we can't work here
#warning "Unsupported system. \
A.T.O.M Engine can be built only on Windows or Unix. \
You can build it on other system at you own risk without any guaranties"
#endif  // system

// If it will be compiled on Windows
#ifdef WINDOWS
#define localtime_r(timep, result)  (localtime (timep) ? memcpy  ((result), \
localtime (timep), sizeof (*(result))) : 0)
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
// Oh God! I hate Microsoft!..
#define snprintf sprintf_s
#endif  // _MSC_VER
#ifdef __BORLANDC__
#include "sys/types.h"
#endif  // __BORLANDC__
#else
#include <inttypes.h>
#endif  // WINDOWS

#include "SysVars/sysvars.h"
// System parameters

// byteorder:
#define BO_BE  0  // 0123
#define BO_LE  1  // 3210
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
