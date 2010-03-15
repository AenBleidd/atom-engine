#ifndef _CORE_ERROR_H_
#define _CORE_ERROR_H_

#include "../preproc.h"

#include <string.h>
#include <stdio.h>
#include <ctime>

struct ERR
{
      int code;
      int sub_code;
      char *description;
};

class AtomLog {
 public:
  AtomLog();
  ~AtomLog();
 private:
// global last error and warning
  ERR global_error;
  ERR global_warning;
// logfile
  FILE *logfile;
 public:
// get last error and warning
  ERR GetLastErr () { return global_error; }
  ERR GetLastWrn () { return global_warning; }
// write last error and warning into the log
  void SetLastErr (unsigned int code, unsigned int subcode);
  void SetLastWrn (unsigned int code, unsigned int subcode);
// write log message
  void LogMessage(char *string);
// write debug log message
  void DebugMessage(char *string);
};

#define ERROR_CORE_FS 0x00000001
#define ERROR_OPEN_FILE 0x00000001
#define ERROR_READ_FILE 0x00000002
#define ERROR_PARSE_MOUNT_FILE_QUOTES 0x00000003
#define ERROR_PARSE_MOUNT_FILE_PRIORITY 0x00000004
#define ERROR_MOUNT_FS 0x00000005
#define ERROR_OPEN_FOLDER 0x00000006
#define ERROR_WRITE_FILE 0x00000007

#endif //_CORE_ERROR_H_