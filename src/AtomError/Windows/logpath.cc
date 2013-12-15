#include <preproc.h>
#include <AtomError.h>

char* AtomLog::GetLogPath(char *temppath, uint32_t size) {
  if (temppath == 0 || size == 0) {
    return 0;
  }
  snprintf(temppath, size, "%s", "log\\");
#ifdef ATOM_DEBUG
// Check log folder
  if (SetCurrentDirectory("log") == 0) {
    if (CreateDirectory("log", NULL) == 0) {
      if (GetTempPath(size, temppath) == 0) {
// WTF ???
        fprintf(stderr, "Can't create log directory\n");
        return 0;
      }
    }
  }  else {
      SetCurrentDirectory("..");
    }
#else
  if (GetTempPath(size, temppath) == 0) {
// WTF ???
    if (SetCurrentDirectory("log") == 0) {
      if (CreateDirectory("log", NULL) == 0) {
        fprintf(stderr, "Can't create log directory\n");
        return 0;
      } else {
        snprintf(temppath, size, "%s", "log\\");
      }
    } else {
      SetCurrentDirectory("..");
    }
  }
#endif  // ATOM_DEBUG
  return temppath;
}