#include "AtomError.h"
// returns current system date & time in text format
char* AtomLog::CurDateTime() {
  time_t seconds = time(NULL);
  ptimeinfo = (tm*) localtime_r(&seconds, ptimeinfo);
  snprintf(datetimebuf, sizeof(datetimebuf), "%i.%02i.%02i_%02i.%02i.%02i",
          ptimeinfo->tm_year+1900, ptimeinfo->tm_mon+1,
          ptimeinfo->tm_mday, ptimeinfo->tm_hour,
          ptimeinfo->tm_min, ptimeinfo->tm_sec);
//  delete timeinfo;
  return datetimebuf;
}
char* AtomLog::CurTime() {
  time_t seconds = time(NULL);
  ptimeinfo = (tm*) localtime_r(&seconds, ptimeinfo);
  snprintf(timebuf, sizeof(timebuf), "%02i:%02i:%02i", ptimeinfo->tm_hour,
          ptimeinfo->tm_min, ptimeinfo->tm_sec);
//  delete timeinfo;
  return timebuf;
}
AtomLog::AtomLog(char *name, bool alone, uint8_t lvl) {
  ptimeinfo = new tm;
  logfile = 0;
  verbose_level = lvl;
  if (name != 0) {
    const uint16_t s = 355;
    char *plogfilename  = new char[s];
// get current date and time
    const uint8_t t = 255;
    char *temppath = new char[t];
#ifdef UNIX
#ifndef ATOM_DEBUG
    snprintf(temppath, t, "%s", "/tmp/");
#else
    snprintf(temppath, t, "%s", "log/");
#endif  // ATOM_DEBUG
#endif  // UNIX
#ifdef WINDOWS
#ifndef ATOM_DEBUG
    GetTempPath(t, temppath);
#else
    snprintf(temppath, t, "%s", "log\\");
#endif  // ATOM_DEBUG
#endif  // WINDOWS

// Check log folder
#ifdef ATOM_DEBUG
#ifdef UNIX
    if (chdir("log") != 0) {
#endif  // UNIX
#ifdef WINDOWS
    if (SetCurrentDirectory("log") == 0) {
#endif  // WINDOWS
// One by one, We will fall, down down...
// Wait a minute ! We have last hope!
// Lets save logfile into the temp directory.
// At least this directory MUST be exist...
#ifdef UNIX
      if (mkdir("log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
        snprintf(temppath, t, "%s", "/tmp/");
#endif  // UNIX
#ifdef WINDOWS
      if (CreateDirectory("log", NULL) == 0) {
        GetTempPath(t, temppath);
#endif  // WINDOWS
        fprintf(stderr, "Can't create log directory\n");
      }
    } else {
#ifdef UNIX
      chdir("..");
#endif  // UNIX
#ifdef WINDOWS
      SetCurrentDirectory("..");
#endif  // WINDOWS
    }
#endif  // ATOM_DEBUG

    if (alone == false) {
      if (strcmp(name, "atom") != 0) {
        snprintf(plogfilename, s, "%s%s_%s%s",
                 temppath, name, CurDateTime(), ".log");
      } else {
        snprintf(plogfilename, s, "%s%s_%s%s",
                 temppath, "atom", CurDateTime(), ".log");
      }
    } else {
      if (strcmp(name, "atom") != 0) {
        snprintf(plogfilename, s, "%s%s%s",
                 temppath, name, ".log");
      } else {
        snprintf(plogfilename, s, "%s%s%s",
                 temppath, "atom", ".log");
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
      snprintf(plogfilename, s, "%s%s_%s%s",
               temppath, "atom", CurDateTime(), ".log");
      if (alone == false)
        logfile = fopen(plogfilename, "wt");
      else
        logfile = fopen(plogfilename, "at");
      if (logfile == 0) {
        delete [] temppath;
        delete [] plogfilename;
// We can't do this...
        throw -1;
      }
      snprintf((char*)MsgBuf, MSG_BUFFER_SIZE, "Given filename '%s' is wrong",
               name);
      LogMessage(MsgBuf);
    }
    delete [] temppath;
    delete [] plogfilename;
    if (logfile == 0) {
// We can't do this...
      throw -1;
    }
  }
  sysjrn = 0;
#ifdef SYSJOURNAL
    try { sysjrn = new AtomSystemJournal; }
    catch (...) {
      throw -1;
    }
#endif  // SYSJOURNAL
  errorcode = 0;
  errorsubcode = 0;
  warningcode = 0;
  warningsubcode = 0;
  global_error.code = 0;
  global_error.sub_code = 0;
  global_error.description = 0;
  global_warning.code = 0;
  global_warning.sub_code = 0;
  global_warning.description = 0;
  module_error_count = 0;
  module_warning_count = 0;
}
AtomLog::~AtomLog() {
  delete ptimeinfo;
// close log file
  if (logfile != 0)
    fclose(logfile);
// do some clean
  if (global_error.description != 0) {
    delete [] global_error.description;
    global_error.description = 0;
  }
  if (global_warning.description != 0) {
    delete [] global_warning.description;
    global_warning.description = 0;
  }
#ifdef SYSJOURNAL
  if(sysjrn)
    delete sysjrn;
#endif  // SYSJOURNAL
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
void AtomLog::SetLastError(uint32_t code, uint32_t subcode,
                           const char* file, int32_t line) {
// do some clean
  if (global_error.description != 0) {
    delete [] global_error.description;
    global_error.description = 0;
  }
  global_error.code = code;
  global_error.sub_code = subcode;
// SIC! This is last system error. It may be unusefull in some situations.
#ifdef WINDOWS
  global_error.system_code = GetLastError();
#endif  // WINDOWS
#ifdef UNIX
  global_error.system_code = errno;
#endif  // UNIX
  uint32_t errlen = strlen(errorcode[global_error.code]) +
             strlen(errorsubcode[global_error.code][global_error.sub_code]);
  global_error.description = new char[errlen];
  errlen += 300;
  snprintf(global_error.description, errlen, "%s:%i\tERROR: %s\t%s",
           file, line, errorcode[global_error.code],
           errorsubcode[global_error.code][global_error.sub_code]);
  snprintf(MsgBuf, MSG_BUFFER_SIZE, "%s\n", global_error.code);
  if (verbose_level >= SHOWSYSTEMERRORS) {
    snprintf(MsgBuf, MSG_BUFFER_SIZE, "%s: %x", "Last system error is",
             global_error.system_code);
    DebugMsg(MsgBuf);
  }
  if (verbose_level >= SHOWSYSTEMERRORSDESCR) {
#ifdef WINDOWS
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, global_error.system_code,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), MsgBuf,
                  MSG_BUFFER_SIZE, NULL);
#endif  // WINDOWS
#ifdef UNIX
    strerror_r(global_error.system_code, MsgBuf, MSG_BUFFER_SIZE);
#endif  // UNIX
    DebugMsg(MsgBuf);
  }
// log the error
  LogMsg(global_error.description);
}
void AtomLog::SetLastWarning(uint32_t code, uint32_t subcode,
                             const char* file, int32_t line) {
// do some clean
  if (global_warning.description != 0) {
    delete [] global_warning.description;
    global_warning.description = 0;
  }
  global_warning.code = code;
  global_warning.sub_code = subcode;
  uint32_t warnlen = 300 + strlen(warningcode[global_warning.code]) +
         strlen(warningsubcode[global_warning.code][global_warning.sub_code]);
  global_warning.description = new char[warnlen];
  snprintf(global_warning.description, warnlen, "%s:%i\tWARNING: %s\t%s",
           file, line, warningcode[global_warning.code],
           warningsubcode[global_warning.code][global_warning.sub_code]);
// log the error
  LogMsg(global_warning.description);

  return;
}

int32_t AtomLog::LoadStrings(bool type, char **subcodes, 
  char *module_description) {
  if (type == ATOMERROR) {
// load errors
    if (errorcode == 0 && errorsubcode == 0) {
      errorcode = new char*;
      errorcode[0] = module_description;
      errorsubcode = new char**;
      errorsubcode[0] = subcodes;
    } else {
      char ***temp = new char**[module_error_count + 1];
      for (uint32_t i = 0; i < module_error_count; i++)
        temp[i] = errorsubcode[i];
      temp[module_error_count] = subcodes;
      delete [] errorsubcode;
      errorsubcode = temp;
      char **t = new char*[module_error_count + 1];
      for (uint32_t i = 0; i < module_error_count; i++)
        t[i] = errorcode[i];
      t[module_error_count] = module_description;
      delete [] errorcode;
      errorcode = t;
    }
    return module_error_count++;
  } else {
// load warnings
    if (warningcode == 0 && warningsubcode == 0) {
      warningcode = new char*;
      warningcode[0] = module_description;
      warningsubcode = new char**;
      warningsubcode[0] = subcodes;
    } else {
      char ***temp = new char**[module_warning_count + 1];
      for (uint32_t i = 0; i < module_warning_count; i++)
        temp[i] = warningsubcode[i];
      temp[module_warning_count] = subcodes;
      delete [] warningsubcode;
      warningsubcode = temp;
      char **t = new char*[module_warning_count + 1];
      for (uint32_t i = 0; i < module_warning_count; i++)
        t[i] = warningcode[i];
      t[module_warning_count] = module_description;
      delete [] warningcode;
      warningcode = t;
    }
    return module_warning_count++;
  }
}
