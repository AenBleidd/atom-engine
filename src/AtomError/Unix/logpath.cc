#include "../../preproc.h"
#include "../AtomError.h"

char* AtomLog::GetLogPath(void) {
  char *temppath = new char[MAX_PATH];
// Check log folder
#ifdef ATOM_DEBUG
  if (chdir("log") != 0) {
// TODO(Lawliet): Also we can create log file in the directory /var/log/
    if (mkdir("log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
        snprintf(temppath, t, "%s", "/var/log/");
    } else {
      chdir("..");
    }
  }
#else
  snprintf(temppath, t, "%s", "/var/log/");
#endif  // ATOM_DEBUG
  return temppath;
}