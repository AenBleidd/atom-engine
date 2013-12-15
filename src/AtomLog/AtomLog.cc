#include <AtomLog.h>
AtomLog::AtomLog(char *name, bool alone, uint8_t lvl) {
  ptimeinfo = new tm;
  logfile = 0;
  verbose_level = lvl;
  if (name != 0) {
    char plogfilename[MSG_BUFFER_SIZE];
    char temppath[MSG_BUFFER_SIZE];
    if (GetLogPath(temppath, MSG_BUFFER_SIZE) != 0) {
#ifdef WINDOWS
      snprintf(plogfilename, MSG_BUFFER_SIZE, "%s", "nul");
#endif  // WINDOWS
#ifdef UNIX
      snprintf(plogfilename, MSG_BUFFER_SIZE, "%s", "/dev/null");
#endif  // UNIX
      // get current date and time
      if (alone == false) {
        snprintf(plogfilename, MSG_BUFFER_SIZE, "%s%s_%s%s",
          temppath, name, CurDateTime(), ".log");
      }
      else {
        snprintf(plogfilename, MSG_BUFFER_SIZE, "%s%s%s",
          temppath, name, ".log");
      }
    }

    // open log file
    if (alone == false)
      logfile = fopen(plogfilename, "wt");
    else
      logfile = fopen(plogfilename, "at");
    if (logfile == 0) {
      // Another last hope...
      // Try to use standard name
      snprintf(plogfilename, MSG_BUFFER_SIZE, "%s%s_%s%s",
        temppath, "atom", CurDateTime(), ".log");
      if (alone == false)
        logfile = fopen(plogfilename, "wt");
      else
        logfile = fopen(plogfilename, "at");
      if (logfile == 0) {
        // We can't do this... Then we will write to nowhere!
#ifdef WINDOWS
        logfile = fopen("nul", "wt");
#endif  // WINDOWS
#ifdef UNIX
        logfile = fopen("/dev/null", "wt");
#endif  // UNIX
        if (logfile == 0) {
          fprintf(stderr, "We can't write log\n");
        }
      }
      snprintf((char*)MsgBuf, MSG_BUFFER_SIZE, "Given filename '%s' is wrong",
        name);
      LogMessage(MsgBuf);
    }
    if (logfile == 0) {
      // We can't do this...
      // TODO (Lawliet): Maybe we can do this without writing the log?
      throw - 1;
    }
  }
}

AtomLog::~AtomLog() {
  delete ptimeinfo;
  // close log file
  if (logfile != 0)
    fclose(logfile);
}

// returns current system date & time in text format
char* AtomLog::CurDateTime() {
  time_t seconds = time(NULL);
  ptimeinfo = (tm*)localtime_r(&seconds, ptimeinfo);
  snprintf(datetimebuf, sizeof(datetimebuf), "%i.%02i.%02i_%02i.%02i.%02i",
    ptimeinfo->tm_year + 1900, ptimeinfo->tm_mon + 1,
    ptimeinfo->tm_mday, ptimeinfo->tm_hour,
    ptimeinfo->tm_min, ptimeinfo->tm_sec);
  return datetimebuf;
}

char* AtomLog::CurTime() {
  time_t seconds = time(NULL);
  ptimeinfo = (tm*)localtime_r(&seconds, ptimeinfo);
  snprintf(timebuf, sizeof(timebuf), "%02i:%02i:%02i", ptimeinfo->tm_hour,
    ptimeinfo->tm_min, ptimeinfo->tm_sec);
  return timebuf;
}

void AtomLog::ErrorMessage(const char* module, const char* error,
  const char* file, int32_t line) {
  uint32_t errlen = 300 + strlen(module) +
    strlen(error);
  char *description = new char[errlen];
  snprintf(description, errlen, "%s:%i\tERROR: %s:\t%s",
    file, line, module, error);
  if (verbose_level >= SHOW_ALL_ERRORS) {
// SIC! This is last system error. It may be unusefull in some situations.
#ifdef WINDOWS
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), MsgBuf,
      MSG_BUFFER_SIZE, NULL);
    CharToOem((LPCSTR)MsgBuf, (LPSTR)MsgBuf);
#endif  // WINDOWS
#ifdef UNIX
    strerror_r(errno, MsgBuf, MSG_BUFFER_SIZE);
#endif  // UNIX
    DebugMsg(MsgBuf);
  }
  // log the error
  LogMsg(description);
  delete [] description;
}

void AtomLog::WarningMessage(const char* module, const char* warning,
  const char* file, int32_t line) {
  uint32_t warnlen = 300 + strlen(module) + strlen(warning);
  char *description = new char[warnlen];
  snprintf(description, warnlen, "%s:%i\tWARNING: %s:\t%s",
    file, line, module,warning);
  // log the error
  LogMsg(description);
  delete [] description;
  return;
}

void AtomLog::LogMsg(const char *string, uint8_t lvl, const char *file,
  int32_t line) {
  if (lvl <= verbose_level) {
#ifdef ATOM_DEBUG
    fprintf(stderr, "%s %s:%i\t%s", CurTime(), file, line, string);
#endif  // ATOM_DEBUG
    if (logfile != 0) {
      fprintf(logfile, "%s %s:%i\t%s", CurTime(), file, line, string);
      fflush(logfile);
    }
  }
  return;
}
void AtomLog::LogMsg(const char *string) {
#ifdef ATOM_DEBUG
  if (string[strlen(string)] == '\n')
    fprintf(stderr, "%s %s", CurTime(), string);
  else
    fprintf(stderr, "%s %s\n", CurTime(), string);
#endif  // ATOM_DEBUG
  if (logfile != 0) {
    if (string[strlen(string)] == '\n')
      fprintf(logfile, "%s %s", CurTime(), string);
    else
      fprintf(logfile, "%s %s\n", CurTime(), string);
    fflush(logfile);
  }
  return;
}
void AtomLog::DebugMsg(const char *string, uint8_t lvl, const char *file,
  int32_t line) {
#ifdef ATOM_DEBUG
  LogMsg(string, lvl, file, line);
#endif  // ATOM_DEBUG
  return;
}
void AtomLog::DebugMsg(const char *string) {
#ifdef ATOM_DEBUG
  LogMsg(string);
#endif  // ATOM_DEBUG
  return;
}
