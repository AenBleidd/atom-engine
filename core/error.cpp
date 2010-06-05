#include "error.h"
// returns current system time in text format
char* CurDateTime() {
  time_t seconds = time(NULL);
  tm *timeinfo = new tm;
  timeinfo = localtime_r(&seconds, timeinfo);
  const unsigned char s = 20;
  char *date = new char[s];
  snprintf(date, s, "%i.%02i.%02i %02i:%02i:%02i",
          timeinfo->tm_year+1900, timeinfo->tm_mon,
          timeinfo->tm_mday, timeinfo->tm_hour,
          timeinfo->tm_min, timeinfo->tm_sec);
  delete timeinfo;
  return date;
}
char* CurTime() {
  time_t seconds = time(NULL);
  tm *timeinfo = new tm;
  timeinfo = localtime_r(&seconds, timeinfo);
  const unsigned char s = 10;
  char *time = new char[s];
  snprintf(time, s, "%02i:%02i:%02i", timeinfo->tm_hour,
          timeinfo->tm_min, timeinfo->tm_sec);
  return time;
}
AtomLog::AtomLog() {
  const unsigned char s = 100;
  char *plogfilename = new char[s];
// get current date and time
  char *pbuffer = CurDateTime();

#ifdef _FSMAN_
  snprintf(plogfilename, s, "%s %s %s",
           "log/fsman", pbuffer, ".log");
#else
  snprintf(plogfilename, s, "%s %s %s",
           "log/atom", pbuffer, ".log");
#endif  // _FSMAN_
// open log file
  logfile = fopen(plogfilename, "wt");
}
AtomLog::~AtomLog() {
// close log file
  fclose(logfile);
}
void AtomLog::LogMessage(char *string) {
  fputs(CurTime(), logfile), fputs("\t", logfile);
  fputs(string, logfile), fputs("\n", logfile), fflush(logfile);
}
void AtomLog::DebugMessage(char *string) {
#ifdef ATOM_DEBUG
  LogMessage(string);
#endif
return;
}
void AtomLog::SetLastErr(unsigned int code, unsigned int subcode) {
  char *s_code, *s_subcode;
  unsigned int len = 0;
  global_error.code = code;
  global_error.sub_code = subcode;
// looking for error code
  switch (global_error.code) {
    case ERROR_CORE_FS: {
      s_code = new char[33];
      s_code = "Core Error. File System Error.\t";
      len = 33;
// looking for error subcode
      switch (global_error.sub_code) {
        case ERROR_OPEN_FILE: {
          s_subcode = new char[31];
          s_subcode = "Error while opening the file.";
          len += 31;
          break;
        }
        case ERROR_READ_FILE: {
          s_subcode = new char[31];
          s_subcode = "Error while reading the file.";
          len += 31;
          break;
        }
        case ERROR_PARSE_MOUNT_FILE_QUOTES: {
          s_subcode = new char[56];
          s_subcode = "Error while parsing the mount file. No closing quotes.";
          len += 56;
          break;
        }
        case ERROR_PARSE_MOUNT_FILE_PRIORITY: {
          s_subcode = new char[78];
/*NOLINT*/s_subcode = "Error while parsing the mount file. To big priority or it's somesthing else.";
          len += 78;
          break;
        }
        case ERROR_MOUNT_FS: {
          s_subcode = new char[38];
          s_subcode = "Error while mounting the file system.";
          len += 78;
          break;
        }
        case ERROR_OPEN_FOLDER: {
          s_subcode = new char[30];
          s_subcode = "Couldn't open the directory.";
          len += 78;
          break;
        }
        case ERROR_WRITE_FILE: {
          s_subcode = new char[30];
          s_subcode = "Error while writing the file.";
          len += 78;
          break;
        }
      }
      break;
    }
  }
// add an error description
  global_error.description = new char[len+17];
  snprintf(global_error.description, len+17, "%s\tERROR:\t%s%s",
           CurTime(), s_code, s_subcode);
// log the error
  LogMessage(global_error.description);
// cleaning
  delete [] s_code;
  delete [] s_subcode;
}
void AtomLog::SetLastWrn(unsigned int code, unsigned int subcode) {
  return;
}
