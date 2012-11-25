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
  void ReportEvent(uint32_t cat, uint32_t mess, uint16_t type, const char* file, int32_t line);
// get last error and warning
  inline ERR GetLastErr() { return global_error; }
  inline ERR GetLastWrn() { return global_warning; }
// set verbose level
  inline void SetLogVerbosity(uint8_t lvl) { verbose_level = lvl; }
// get verbose level
  inline uint8_t GetLogVerbosity() { return verbose_level; }
private:
  HANDLE hEventSource;
  ERR global_error;
  ERR global_warning;
// verbose level
  uint8_t verbose_level;
};

#define ReportErr(cat, mess) ReportEvent(cat, mess, EVENTLOG_ERROR_TYPE, __FILE__,__LINE__);
#define ReportError(cat, mess, file, line) ReportEvent(cat, mess, EVENTLOG_ERROR_TYPE, file, line);
#define ReportWrn(cat, mess) ReportEvent(cat, mess, EVENTLOG_WARNING_TYPE, __FILE__,__LINE__);
#define ReportWarn(cat, mess, file, line) ReportEvent(cat, mess, EVENTLOG_WARNING_TYPE, file, line);

#endif  // _SYSTEMJOURNAL_H_