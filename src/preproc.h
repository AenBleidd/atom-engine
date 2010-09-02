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
#define BYTEORDER ByteOrderDetect()

#endif  // _PREPROC_H_
