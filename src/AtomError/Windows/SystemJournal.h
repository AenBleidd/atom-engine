#ifndef _SYSTEMJOURNAL_H_
#define _SYSTEMJOURNAL_H_

#include "../../preproc.h"

#include <windows.h>
#include <stdio.h>

class AtomSystemJournal {
public:
  explicit AtomSystemJournal(char *appName = "atom", char *msgFileName = 0, int CatCount = 0);
  ~AtomSystemJournal();
  void ReportEvent(uint32_t cat, uint32_t mess, uint16_t type, const char* file, int32_t line);
private:
  HANDLE hEventSource;
};

#define ReportErr(cat, mess) ReportEvent(cat, mess, EVENTLOG_ERROR_TYPE, __FILE__,__LINE__);
#define ReportError(cat, mess, file, line) ReportEvent(cat, mess, EVENTLOG_ERROR_TYPE, file, line);
#define ReportWrn(cat, mess) ReportEvent(cat, mess, EVENTLOG_WARNING_TYPE, __FILE__,__LINE__);
#define ReportWarn(cat, mess, file, line) ReportEvent(cat, mess, EVENTLOG_WARNING_TYPE, file, line);

#endif  // _SYSTEMJOURNAL_H_