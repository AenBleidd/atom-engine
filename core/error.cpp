#include "error.h"
// returns current system date & time in text format
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
AtomLog::AtomLog() {
  const unsigned char s = 100;
  char *plogfilename  = new char[s];
// get current date and time
  char *pbuffer = CurDateTime();

#ifdef _FSMAN_
  snprintf(plogfilename, s, "%s %s%s",
           "log/fsman", pbuffer, ".log");
#else
  snprintf(plogfilename, s, "%s %s%s",
           "log/atom", pbuffer, ".log");
#endif  // _FSMAN_
  delete [] pbuffer;
// open log file
  logfile = fopen(plogfilename, "wt");
  delete [] plogfilename;
  global_error.code = 0;
  global_error.sub_code = 0;
  global_error.description = 0;
  global_warning.code = 0;
  global_warning.sub_code = 0;
  global_warning.description = 0;
}
AtomLog::~AtomLog() {
// close log file
  fclose(logfile);
}
void AtomLog::LogMsg(char *string, char *file, int line) {
  fprintf(logfile, "%s %s:%i\t%s\n", __TIME__, file, line, string);
  fflush(logfile);
  return;
}
void AtomLog::LogMsg(char *string) {
  fprintf(logfile, "%s %s\n", __TIME__, string);
  fflush(logfile);
  return;
}
void AtomLog::DebugMsg(char *string, char *file, int line) {
#ifdef ATOM_DEBUG
  LogMsg(string, file, line);
#endif
return;
}
void AtomLog::DebugMsg(char *string) {
#ifdef ATOM_DEBUG
  LogMsg(string);
#endif
return;
}
void AtomLog::SetLastError(unsigned int code, unsigned int subcode,
                           char* file, int line) {
  char *s_code, *s_subcode;
  unsigned int len = 0;
  unsigned int glen = 0;
  if ((global_error.code == 0) && (global_error.sub_code == 0))
    delete [] global_error.description, global_error.description = 0;
  global_error.code = code;
  global_error.sub_code = subcode;
// looking for error code
  switch (global_error.code) {
    case ERROR_CORE_FS: {
      glen = 30;
      s_code = new char[glen];
      snprintf(s_code, glen, "%s", "Core Error. File System Error.");
// looking for error subcode
      switch (global_error.sub_code) {
        case ERROR_OPEN_FILE: {
          len = 29;
          s_subcode = new char[len];
          snprintf(s_subcode, len, "%s", "Error while opening the file.");
          break;
        }
        case ERROR_READ_FILE: {
          len = 29;
          s_subcode = new char[len];
          snprintf(s_subcode, len, "%s", "Error while reading the file.");
          break;
        }
        case ERROR_PARSE_MOUNT_FILE_QUOTES: {
          len = 54;
          s_subcode = new char[len];
          snprintf(s_subcode, len, "%s",
"Error while parsing the mount file. No closing quotes.");
          break;
        }
        case ERROR_PARSE_MOUNT_FILE_PRIORITY: {
          len = 75;
          s_subcode = new char[len];
          snprintf(s_subcode, len, "%s",
"Error while parsing the mount file. To big priority or it's something else.");
          break;
        }
        case ERROR_MOUNT_FS: {
          len = 37;
          s_subcode = new char[len];
          snprintf(s_subcode, len, "%s",
"Error while mounting the file system.");
          break;
        }
        case ERROR_OPEN_FOLDER: {
          len = 28;
          s_subcode = new char[len];
          snprintf(s_subcode, len, "%s", "Couldn't open the directory.");
          break;
        }
        case ERROR_WRITE_FILE: {
          len = 29;
          s_subcode = new char[len];
          snprintf(s_subcode, len, "%s", "Error while writing the file.");
          break;
        }
      }
      break;
    }
  }
// add an error description
  global_error.description = new char[glen+len+300];
  snprintf(global_error.description, glen+len+300, "%s:%i\tERROR: %s\t%s",
           file, line, s_code, s_subcode);
// log the error
  LogMsg(global_error.description);
// cleaning
  delete [] s_code;
  delete [] s_subcode;
}
void AtomLog::SetLastWarning(unsigned int code, unsigned int subcode,
                             char* file, int line) {
  return;
}
