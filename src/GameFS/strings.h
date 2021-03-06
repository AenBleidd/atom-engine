#ifndef _GAMEFS_STRINGS_H_
#define _GAMEFS_STRINGS_H_

#include <string.h>
#include "gamefs.h"

#include "../preproc.h"
#include "../AtomError/AtomError.h"

struct ARGUMENTS {
  char **output;
  int32_t count;
};

// Parse the *nix-style path
ARGUMENTS* ParsePath(AtomLog *atomlog, char *input);
// Parse the line of arguments
ARGUMENTS* ParseArgs(AtomLog *atomlog, char *input);
// Find the file in the file system
TREE_FILE* FindFileFromPath(AtomLog *atomlog, TREE_FOLDER *root,
                            TREE_FOLDER *current, ARGUMENTS *path);
// 16-byte password input function
uint32_t* PassPrint(void);
void PassInput(char *input);

#endif  // _GAMEFS_STRINGS_H_
