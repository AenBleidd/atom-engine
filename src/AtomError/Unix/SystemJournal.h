#ifndef _SYSTEMJOURNAL_H_
#define _SYSTEMJOURNAL_H_

#include "../../preproc.h"
#include <syslog.h>

class AtomSystemJournal {
public:
  explicit AtomSystemJournal(char *appName = "atom");
  void ReportEvent(char* mess, uint32_t type, const char* file, int32_t line);
  ~AtomSystemJournal();
};

#define ReportErr(mess) ReportEvent(mess, LOG_ERR, __FILE__,__LINE__);
#define ReportError(mess, file, line) ReportEvent(mess, LOG_ERR, file, line);
#define ReportWrn(mess) ReportEvent(mess, LOG_WARNING, __FILE__,__LINE__);
#define ReportWarn(mess, file, line) ReportEvent(mess, LOG_WARNING, file, line);

#endif  // _SYSTEMJOURNAL_H_