#ifndef _SYSTEMJOURNAL_H_
#define _SYSTEMJOURNAL_H_

#include "../../preproc.h"

#include <windows.h>
#include <stdio.h>

class AtomSystemJournal {
public:
  explicit AtomSystemJournal(char *appName = "atom", char *msgFileName = 0, int CatCount = 0);
  ~AtomSystemJournal();
};

#endif  // _SYSTEMJOURNAL_H_