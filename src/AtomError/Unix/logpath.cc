#include "../../preproc.h"
#include "../AtomError.h"

char* AtomLog::GetLogPath(char* temppath, uint32_t size) {
// Check log folder
#ifdef ATOM_DEBUG
snprintf(temppath, size, "%s", "log/");
  if (chdir("log") != 0) {
// TODO(Lawliet): Also we can create log file in the directory /var/log/
    if (mkdir("log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
        snprintf(temppath, size, "%s", "/var/log/");
  } else {
    chdir("..");
  }
#else
  snprintf(temppath, size, "%s", "/var/log/");
#endif  // ATOM_DEBUG
  return temppath;
}