#include <preproc.h>
#include <AtomError.h>

char* AtomLog::GetLogPath(void) {
  char *temppath = new char[MAX_PATH];
// Check log folder
#ifdef ATOM_DEBUG
    if (SetCurrentDirectory("log") == 0) {
      if (CreateDirectory("log", NULL) == 0) {
        if (GetTempPath(MAX_PATH, temppath) == 0) {
// WTF ???
          fprintf(stderr, "Can't create log directory\n");
          return 0;
        } else {
          snprintf(temppath, MAX_PATH, "%s", "log\\");
        }
      } else {
        SetCurrentDirectory("..");
      }
    }
#else
  if (GetTempPath(MAX_PATH, temppath) == 0) {
// WTF ???
    if (SetCurrentDirectory("log") == 0) {
      if (CreateDirectory("log", NULL) == 0) {
        fprintf(stderr, "Can't create log directory\n");
        return 0;
      } else {
        snprintf(temppath, MAX_PATH, "%s", "log\\");
      }
    } else {
      SetCurrentDirectory("..");
    }
  }
#endif  // ATOM_DEBUG
  return temppath;
}