#ifndef _CORE_ERROR_H_
#define _CORE_ERROR_H_

#include <string.h>
#include <stdio.h>
#include <ctime>

#include "../preproc.h"

struct ERR {
  int code;
  int sub_code;
  char *description;
};
// returns current system time in text format
// can be useful in another modules
char* CurDateTime(void);

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
  ERR GetLastErr() { return global_error; }
  ERR GetLastWrn() { return global_warning; }
// write last error and warning into the log
  void SetLastError(unsigned int code, unsigned int subcode,
                    char* file, int line);
  void SetLastWarning(unsigned int code, unsigned int subcode,
                      char* file, int line);
// write log message
  void LogMsg(char *string, char *file, int line);
// write debug log message
  void DebugMsg(char *string, char *file, int line);
 private:
// write log message
  void LogMsg(char *string);
// write debug log message
  void DebugMsg(char *string);
};

#define SetLastErr(code,subcode) SetLastError(code,subcode,__FILE__,__LINE__);  /*NOLINT*/
#define SetLastWnd(code,subcode) SetLastWarning(code,subcode,__FILE__,__LINE__);  /*NOLINT*/
#define LogMessage(string) LogMsg(string,__FILE__,__LINE__);  /*NOLINT*/
#define DebugMessage(string) DebugMsg(string,__FILE__,__LINE__);  /*NOLINT*/

#define ERROR_CORE_FS 0x00000001
#define ERROR_OPEN_FILE 0x00000001
#define ERROR_READ_FILE 0x00000002
#define ERROR_PARSE_MOUNT_FILE_QUOTES 0x00000003
#define ERROR_PARSE_MOUNT_FILE_PRIORITY 0x00000004
#define ERROR_MOUNT_FS 0x00000005
#define ERROR_OPEN_FOLDER 0x00000006
#define ERROR_WRITE_FILE 0x00000007

#endif  // _CORE_ERROR_H_
