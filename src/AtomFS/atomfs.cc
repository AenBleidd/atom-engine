#include "atomfs.h"

AtomFS::AtomFS(AtomLog *log) {
  if (log == 0)
    throw -1;
  atomlog = log;
  ERROR_CORE_FS = atomlog->LoadStrings(ATOMERROR, atomfserrorcodes,
    atomfs_error_description);
  WARNING_CORE_FS = atomlog->LoadStrings(ATOMWARNING, atomfswarncodes,
    atomfs_warn_description);
  if (ERROR_CORE_FS == -1 || WARNING_CORE_FS == -1) {
    throw - 1;
  }
  listAtomFSHeader = 0;
  listAtomFSFiletableRecord = 0;
}