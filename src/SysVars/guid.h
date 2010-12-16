#ifndef _GUID_H_
#define _GUID_H_

#include "../preproc.h"
#ifdef UNIX
#include <uuid/uuid.h>
#endif  // UNIX
#ifdef WINDOWS
#include <rpc.h>
#endif  // WINDOWS

struct AGUID {
  unsigned int data0;
  unsigned short int data1;
  unsigned short int data2;
  unsigned char data3[8];
};

// Generate new GUID
// or return a GUID of previously generated GUID string
AGUID* GetAGUID (char *guid = 0);
// Generate new GUID and return it like a string
// or return a string of the already generated GUID
char* GetCGUID (AGUID *guid = 0);

#endif  // _GUID_H_
