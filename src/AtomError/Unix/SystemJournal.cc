#include "SystemJournal.h"
AtomSystemJournal::AtomSystemJournal(char *appName) {
  if (appName == 0) {
    throw;
  }
  openlog(appName, LOG_NDELAY, LOG_USER);
}

AtomSystemJournal::~AtomSystemJournal() {
  closelog();
}

void AtomSystemJournal::ReportEvent(char* mess, uint32_t type, const char* file, int32_t line) {
  syslog(LOG_USER | type, "%s:%i\t%s", file, line, mess);
}