#ifndef _GAMEFS_STRINGS_H_
#define _GAMEFS_STRINGS_H_

#include <string.h>

#include "../preproc.h"
#include "../AtomError/AtomError.h"

struct ARGUMENTS {
  char **output;
  int count;
};

// Parse the *nix-style path
ARGUMENTS* ParsePath(AtomLog *atomlog, char *input);
// Parse the line of arguments
ARGUMENTS* ParseArgs(AtomLog *atomlog, char *input);
#endif  // _GAMEFS_STRINGS_H_
