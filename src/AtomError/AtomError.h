#ifndef _ATOM_ERROR_H_
#define _ATOM_ERROR_H_

#include <preproc.h>
#include "OAtomError.h"

#ifdef WINDOWS
#include <windows.h>
#ifdef SYSJOURNAL
#include <SystemJournal.h>
#endif  // SYSJOURNAL
#ifdef __BORLANDC__
#include <time.h>
#endif  // __BORLANDC__
#endif  // WINDOWS
#include <string.h>
#include <stdio.h>
#ifdef UNIX
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#ifdef SYSJOURNAL
#include <Unix/SystemJournal.h>
#endif  // SYSJOURNAL
#endif  // UNIX

// some verbosity levels
enum {
// errors will be written only into system journal (good for release project)
  LOGONLYINTOSYSTEMJOURNAL = 0xFD,
// show only last system error codes
  SHOWSYSTEMERRORS = 0xFE,
// show last system error codes and their description
  SHOWSYSTEMERRORSDESCR = 0xFF
};

class AtomLog : public OAtomLog{
 public:
  explicit AtomLog(char *name = "atom", bool alone = false,
                    uint8_t lvl = 0xFF);
  ~AtomLog();
  char MsgBuf[MSG_BUFFER_SIZE];
 private:
// global last error and warning
#ifdef SYSJOURNAL
  AtomSystemJournal *sysjrn;
#endif // SYSJOURNAL
  ERR global_error;
  ERR global_warning;
// errors and warnings
  char **errorcode;
  char ***errorsubcode;
  char **warningcode;
  char ***warningsubcode;
// logfile
  FILE *logfile;
 public:
// get last error and warning
  inline ERR GetLastErr() { return global_error; }
  inline ERR GetLastWrn() { return global_warning; }
// set verbose level
  inline void SetLogVerbosity(uint8_t lvl) { verbose_level = lvl; }
// get verbose level
  inline uint8_t GetLogVerbosity() { return verbose_level; }
// write last error and warning into the log
  void SetLastError(uint32_t code, uint32_t subcode,
                    const char* file, int32_t line);
  void SetLastWarning(uint32_t code, uint32_t subcode,
                      const char* file, int32_t line);
// write log message
  void LogMsg(const char *string, uint8_t lvl, const char *file,
              int32_t line);
// write debug log message
  void DebugMsg(const char *string, uint8_t lvl, const char *file,
                int32_t line);
// load error descriptions and return module code
// type - ERROR or WARNING
#define ATOMERROR   true
#define ATOMWARNING false
  int32_t LoadStrings(bool type, char **subcodes, char *module_description);
 private:
// verbose level
  uint8_t verbose_level;
// modules count
  uint32_t module_error_count;
  uint32_t module_warning_count;
// write log message
  void LogMsg(const char *string);
// write debug log message
  void DebugMsg(const char *string);
// find some path where we can place our log file
  char* GetLogPath(void);
};

#define SetLastErr(code,subcode) SetLastError(code,subcode,__FILE__,__LINE__);
#define SetLastWrn(code,subcode) SetLastWarning(code,subcode,__FILE__,__LINE__);
#define LogMessage(string) LogMsg(string,0,__FILE__,__LINE__);
#define DebugMessage(string) DebugMsg(string,0,__FILE__,__LINE__);
#define LogMessageV(string,lvl) LogMsg(string,lvl,__FILE__,__LINE__);
#define DebugMessageV(string,lvl) DebugMsg(string,lvl,__FILE__,__LINE__);

#endif  // _CORE_ERROR_H_
