#ifndef _GAMEFS_STRINGS_H_
#define _GAMEFS_STRINGS_H_

#include <string.h>

#include "../preproc.h"
#include "../AtomError/AtomError.h"

// Parse the *nix-style path
int ParsePath(AtomLog *atomlog, char *input, char **output);
// Parse the line of arguments
int ParseArgs(AtomLog *atomlog, char *input, char **output);
#endif  // _GAMEFS_STRINGS_H_
