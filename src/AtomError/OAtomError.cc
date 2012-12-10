#include "OAtomError.h"
OAtomLog::OAtomLog() {
  ptimeinfo = new tm;
}

OAtomLog::~OAtomLog() {
  delete ptimeinfo;
}

// returns current system date & time in text format
char* OAtomLog::CurDateTime() {
  time_t seconds = time(NULL);
  ptimeinfo = (tm*) localtime_r(&seconds, ptimeinfo);
  snprintf(datetimebuf, sizeof(datetimebuf), "%i.%02i.%02i_%02i.%02i.%02i",
          ptimeinfo->tm_year+1900, ptimeinfo->tm_mon+1,
          ptimeinfo->tm_mday, ptimeinfo->tm_hour,
          ptimeinfo->tm_min, ptimeinfo->tm_sec);
  return datetimebuf;
}

char* OAtomLog::CurTime() {
  time_t seconds = time(NULL);
  ptimeinfo = (tm*) localtime_r(&seconds, ptimeinfo);
  snprintf(timebuf, sizeof(timebuf), "%02i:%02i:%02i", ptimeinfo->tm_hour,
          ptimeinfo->tm_min, ptimeinfo->tm_sec);
  return timebuf;
}
