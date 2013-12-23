#include "atomfs.h"

AtomFS::AtomFS(AtomLog *log) {
  if (log == 0)
    throw -1;
  atomlog = log;
  listAtomFSHeader = 0;
  listAtomFSFiletableRecord = new AtomFSFiletableRecord(flag_folder, strlen(root_name), (char*)root_name, root_guid);
}