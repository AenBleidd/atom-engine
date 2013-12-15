#include "atomfs.h"

AtomFS::AtomFS(AtomLog *log) {
  if (log == 0)
    throw -1;
  atomlog = log;
  listAtomFSHeader = 0;
  listAtomFSFiletableRecord = 0;
}