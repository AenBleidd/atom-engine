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
  uint32_t data0;
  uint32_t data1;
  uint32_t data2;
  uint32_t data3;
};

// Generate new GUID
// or return a GUID of previously generated GUID string
AGUID* GetAGUID(char *guid = 0);
// Generate new GUID and return it like a string
// or return a string of the already generated GUID
char* GetCGUID(AGUID *guid = 0);

#endif  // _GUID_H_
