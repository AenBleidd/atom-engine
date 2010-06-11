#include "../preproc.h"

#include "../core/gamefs.h"
#include "../core/error.h"

#include <stdio.h>

AtomLog *atomlog;
AtomFS *atomfs;

int main(int arg, char *argc[]) {
// input parameters processing
  atomlog = new AtomLog;
  atomlog->LogMessage("Begin fsman...");
/*NOLINT*/  char help[] = "File System  Manager - utility to work with Atom File System\nOptions:\n\t-t, --test\t\tTest default file system with standart mount file\n\t-t, --test [mountfile]\tTest file system\n\t-n,--new [output] [input]\tCreate new file with name [output] using folders or files [input], for current folder use './'\n";
  try { atomfs = new AtomFS(atomlog); }
  catch(int i) { }
  if (arg == 1 || strcmp(argc[1], "-h") == 0 || \
      strcmp(argc[1], "--help") == 0 || strcmp(argc[1], "-help") == 0 || \
      strcmp(argc[1], "/?") == 0) {
    atomlog->DebugMessage(help);
    printf("%s", help);
  }
// Testing filesystem
  else if (strcmp(argc[1], "-t") == 0 || strcmp(argc[1], "--test") == 0) {
    atomlog->DebugMessage("Test file system");
    if (arg == 2) {
      atomfs->Mount("mount");
    }
    else if (arg == 3) {
      atomfs->Mount(argc[2]);
    } else {
      atomlog->DebugMessage(help);
      printf("%s", help);
    }
  }
// Create new file
  else if (strcmp(argc[1], "-n") == 0 || strcmp(argc[1], "--new") == 0) {
    if (arg > 3) {
      char **input = new char*[arg-3];
      for (int i = 0; i < arg-3; i++)
        input[i] = argc[i+3];
      atomlog->DebugMessage("Create new file");
      atomfs->Create(input, arg-3, argc[2]);
    } else {
      atomlog->DebugMessage(help);
      printf("%s", help);
    }
  } else {
    atomlog->DebugMessage(help);
    printf("%s", help);
  }
// cleaning
  delete atomfs;
  atomlog->LogMessage("End fsman...");
  return 0;
}
