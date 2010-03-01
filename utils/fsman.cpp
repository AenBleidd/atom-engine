#include "../preproc.h"

#include "../core/gamefs.h"
#include "../core/error.h"

#include <stdio.h>

AtomLog atomlog;
AtomFS *atomfs;

int main (int arg, char *argc[]) {
// input parameters processing
  char help[] = "File System  Manager - utility to work with \
Atom File System\nOptions:\n\t-t, --test\t\tTest default file system \
with standart mount file\n\t-t, --test [mountfile]\tTest file system\n\
\t-n,--new [output] [input]\tCreate new file with name [output] using \
folders or files [input], for current folder use './'\n";
  if (arg == 1 || strcmp(argc[1], "-h") == 0 || \
      strcmp(argc[1], "--help") == 0 || strcmp(argc[1], "-help") == 0 || \
      strcmp(argc[1], "/?") == 0) {
    atomlog.DebugMessage(help);
    printf(help);
  }
// Testing filesystem
  else if (strcmp(argc[1], "-t") == 0 || strcmp(argc[1], "--test") == 0) {
    atomlog.DebugMessage("Test file system");
    try { atomfs = new AtomFS; }
    catch (int i) { }
    if (arg == 2) {
      atomfs->Mount("mount");
    }
    else if (arg == 3) {
      atomfs->Mount(argc[2]);
    }
    else {
      atomlog.DebugMessage(help);
      printf(help);
    }
  }
// Create new file
  else if (strcmp(argc[1], "-n") == 0 || strcmp(argc[1], "--new") == 0) {
    if (arg > 3) {
      char **input = new char*[arg-3];
      for (int i = 0; i < arg-3; i++)
        input[i] = argc[i+3];
      atomfs->Create(input, arg-3, argc[2]);
    }
    else {
      atomlog.DebugMessage(help);
      printf(help);
    }
  }
  else {
    atomlog.DebugMessage(help);
    printf(help);
  }
// cleaning
  delete atomfs;
  return 0;
}
