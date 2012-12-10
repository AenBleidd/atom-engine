#ifndef _SYSTEMJOURNAL_H_
#define _SYSTEMJOURNAL_H_

#include "../../preproc.h"
#include <AtomError.h>

#include <windows.h>
#include <stdio.h>

class AtomSystemJournal : public OAtomLog {
public:
  explicit AtomSystemJournal(char *appName = "atom", char *msgFileName = 0, int CatCount = 0);
  ~AtomSystemJournal();
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

private:
  HANDLE hEventSource;
  ERR global_error;
  ERR global_warning;
// verbose level
  uint8_t verbose_level;
  void AtomReportEvent(uint32_t cat, uint32_t mess, uint16_t type, uint8_t lvl, const char* file, int32_t line);
};

#endif  // _SYSTEMJOURNAL_H_
