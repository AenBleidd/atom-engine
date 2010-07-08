#include "error.h"
// returns current system date & time in text format
#ifndef UNIX
#define localtime_r(timep, result)  (localtime (timep) ? memcpy  ((result), localtime (timep), sizeof (*(result))) : 0) /*NOLINT*/
#endif  // UNIX
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
  const unsigned short s = 355;
  char *plogfilename  = new char[s];
// get current date and time
  char *pbuffer = CurDateTime();
  const unsigned char t = 255;
  char *temppath = new char[t];
#ifdef UNIX
#ifndef ATOM_DEBUG
  snprintf(temppath, t, "%s", "/tmp/");
#else
  snprintf(temppath, t, "%s", "log/");
#endif  // DEBUG
#else
#ifndef ATOM_DEBUG
  GetTempPath(t, temppath);
#else
  snprintf(temppath, t, "%s", "log\\");
#endif  // DEBUG
#endif  // UNIX

#ifdef _FSMAN_
  snprintf(plogfilename, s, "%s%s %s%s",
           temppath, "fsman", pbuffer, ".log");
#else
  snprintf(plogfilename, s, "%s%s %s%s",
           temppath, "atom", pbuffer, ".log");
#endif  // _FSMAN_
  delete [] temppath;
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
void AtomLog::LogMsg(const char *string, const char *file, int line) {
#ifdef ATOM_DEBUG
  fprintf(stderr, "%s %s:%i\t%s\n", __TIME__, file, line, string);
#endif
  fprintf(logfile, "%s %s:%i\t%s\n", __TIME__, file, line, string);
  fflush(logfile);
  return;
}
void AtomLog::LogMsg(const char *string) {
#ifdef ATOM_DEBUG
  fprintf(stderr, "%s %s\n", __TIME__, string);
#endif
  fprintf(logfile, "%s %s\n", __TIME__, string);
  fflush(logfile);
  return;
}
void AtomLog::DebugMsg(const char *string, const char *file, int line) {
#ifdef ATOM_DEBUG
  LogMsg(string, file, line);
#endif
return;
}
void AtomLog::DebugMsg(const char *string) {
#ifdef ATOM_DEBUG
  LogMsg(string);
#endif
return;
}
// Errors description
const char *errorcode[] = {
"No Error.",
"Core Error. File System Error."
};
const char *errorsubcode[] = {
"No Error.",
"Error while opening the file.",
"Error while reading the file.",
"Error while parsing the mount file. No closing quotes.",
"Error while parsing the mount file. To big priority or it's something else.",
"Error while mounting the file system.",
"Couldn't open the directory.",
"Error while writing the file."
};
// Warning description
const char *warningcode[] = {
"No Warning."
};
const char *warningsubcode[] = {
"No Warning."
};
void AtomLog::SetLastError(unsigned int code, unsigned int subcode,
                           const char* file, int line) {
// do some clean
  if (global_error.description != 0) {
    delete [] global_error.description;
    global_error.description = 0;
  }
  unsigned int errlen = 300 + strlen(errorcode[global_error.code]) +
                        strlen(errorsubcode[global_error.sub_code]);
  global_error.description = new char[errlen];
  snprintf(global_error.description, errlen, "%s:%i\tERROR: %s\t%s",
           file, line, errorcode[global_error.code],
           errorsubcode[global_error.sub_code]);
// log the error
  LogMsg(global_error.description);
}
void AtomLog::SetLastWarning(unsigned int code, unsigned int subcode,
                             const char* file, int line) {
// do some clean
  if (global_warning.description != 0) {
    delete [] global_warning.description;
    global_warning.description = 0;
  }
  unsigned int warnlen = 300 + strlen(warningcode[global_warning.code]) +
                        strlen(warningsubcode[global_warning.sub_code]);
  global_warning.description = new char[warnlen];
  snprintf(global_warning.description, warnlen, "%s:%i\tWARNING: %s\t%s",
           file, line, warningcode[global_warning.code],
           warningsubcode[global_warning.sub_code]);
// log the error
  LogMsg(global_warning.description);

  return;
}
