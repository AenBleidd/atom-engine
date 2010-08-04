#ifndef _GAMEFS_NAVIGATE_H_
#define _GAMEFS_NAVIGATE_H_

#include "../AtomError/AtomError.h"
#include "gamefs.h"
#include "strings.h"

void ShowHelp(AtomLog *atomlog);
// mode is:
// 0 - simple
// 1 - extended
int List(AtomLog *atomlog, TREE_FOLDER *curfolder, unsigned char mode);
TREE_FOLDER* ChDir(AtomLog *atomlog, TREE_FOLDER *root,
                   TREE_FOLDER *curfolder, ARGUMENTS *path);
#endif  // _GAMEFS_NAVIGATE_H_
