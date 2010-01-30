#include "../preproc.h"

#include "../core/gamefs.h"
#include "../core/error.h"

AtomLog atomlog;
AtomFS *atomfs;

int main (void) {
  try { atomfs = new AtomFS; }
  catch (int i) { }
// cleaning
  delete atomfs;
  return 0;
}
