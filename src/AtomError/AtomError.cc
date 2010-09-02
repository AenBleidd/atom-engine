#include "AtomError.h"
// returns current system date & time in text format
char datetimebuf[20];
char timebuf[10];
char* AtomLog::CurDateTime() {
  time_t seconds = time(NULL);
  tm *timeinfo = new tm;
  timeinfo = (tm*) localtime_r(&seconds, timeinfo);
  snprintf(datetimebuf, 20, "%i.%02i.%02i_%02i.%02i.%02i",
          timeinfo->tm_year+1900, timeinfo->tm_mon+1,
          timeinfo->tm_mday, timeinfo->tm_hour,
          timeinfo->tm_min, timeinfo->tm_sec);
  delete timeinfo;
  return datetimebuf;
}
char* AtomLog::CurTime() {
  time_t seconds = time(NULL);
  tm *timeinfo = new tm;
  timeinfo = (tm*) localtime_r(&seconds, timeinfo);
  snprintf(timebuf, 10, "%02i:%02i:%02i", timeinfo->tm_hour,
          timeinfo->tm_min, timeinfo->tm_sec);
  delete timeinfo;
  return timebuf;
}
AtomLog::AtomLog(char *name, bool alone, unsigned char lvl) {
  logfile = 0;
  verbose_level = lvl;
  if (name != 0) {
    const unsigned short s = 355;
    char *plogfilename  = new char[s];
// get current date and time
    const unsigned char t = 255;
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
// Lets save logfile in the temp directory.
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
    logfile = fopen(plogfilename, "wt");
    if (logfile == 0) {
// Another last hope...
// Try to use standart name
      snprintf(plogfilename, s, "%s%s_%s%s",
               temppath, "atom", CurDateTime(), ".log");
      logfile = fopen(plogfilename, "wt");
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
  global_error.code = 0;
  global_error.sub_code = 0;
  global_error.description = 0;
  global_warning.code = 0;
  global_warning.sub_code = 0;
  global_warning.description = 0;
}
AtomLog::~AtomLog() {
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
}
void AtomLog::LogMsg(const char *string, unsigned char lvl, const char *file,
                     int line) {
  if (lvl <= verbose_level) {
#ifdef ATOM_DEBUG
    fprintf(stderr, "%s %s:%i\t%s\n", CurTime(), file, line, string);
#endif  // ATOM_DEBUG
    if (logfile != 0) {
      fprintf(logfile, "%s %s:%i\t%s\n", CurTime(), file, line, string);
      fflush(logfile);
    }
  }
  return;
}
void AtomLog::LogMsg(const char *string) {
#ifdef ATOM_DEBUG
  fprintf(stderr, "%s %s\n", CurTime(), string);
#endif  // ATOM_DEBUG
  if (logfile != 0) {
    fprintf(logfile, "%s %s\n", CurTime(), string);
    fflush(logfile);
  }
  return;
}
void AtomLog::DebugMsg(const char *string, unsigned char lvl, const char *file,
                       int line) {
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
// Errors description
const char *errorcode[] = {
"No Error.",
"Core Error. File System Error.",
"Engine Error. WindowManager Error."
};
const char *errorsubcode[3][17] = {
{
"No Error."
}, {
"No Error.",
"Error opening the file.",
"Error reading the file.",
"Error parsing the mount file. No closing quotes.",
"Error parsing the mount file.",
"Error mounting the file system.",
"Couldn't open the directory.",
"Error while writing the file.",
"Incorrect file.",
"Incorrect mountpoint",
"Can't overwrite the file.",
"Lost quotes.",
"Error parsing the string.",
"Wrong byteorder. Program can't read files with unknown byteorder.",
"Path incorrect or directory or file don't exist.",
"Crc check is failed. File is broken.",
"Function was called with wrong arguments."
}, {
"No Error",
"Fatal Error: Error registering the window class",
"Fatal Errpr: Couldn't create a window",
"Invalid input parameter",
"Error unregistering the window class",
"Error getting window title",
"Error getting the window info block",
"Error setting the window text while applying the window info block",
"Error trying to change window size and dimentions",
"Error adjusting the client area for a window",
"Error trying to make window topmost",
"No connection to X server established",
"Error getting WM hints: window size and position",
"No memory for XLib string conversion"
}
};
// Warning description
const char *warningcode[] = {
"No Warning.",
"Core Warning. File System Warning."
};
const char *warningsubcode[2][5] = {
{
"No Warning."
}, {
"No Warning.",
"File was overwritten.",
"Empty string. Nothing to parse.",
"Wrong syntax or unknown command.",
"Path incorrect or directory doesn't exist."
}
};
void AtomLog::SetLastError(unsigned int code, unsigned int subcode,
                           const char* file, int line) {
// do some clean
  if (global_error.description != 0) {
    delete [] global_error.description;
    global_error.description = 0;
  }
  global_error.code = code;
  global_error.sub_code = subcode;
  unsigned int errlen = 300 + strlen(errorcode[global_error.code]) +
               strlen(errorsubcode[global_error.code][global_error.sub_code]);
  global_error.description = new char[errlen];
  snprintf(global_error.description, errlen, "%s:%i\tERROR: %s\t%s",
           file, line, errorcode[global_error.code],
           errorsubcode[global_error.code][global_error.sub_code]);
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
  global_warning.code = code;
  global_warning.sub_code = subcode;
  unsigned int warnlen = 300 + strlen(warningcode[global_warning.code]) +
         strlen(warningsubcode[global_warning.code][global_warning.sub_code]);
  global_warning.description = new char[warnlen];
  snprintf(global_warning.description, warnlen, "%s:%i\tWARNING: %s\t%s",
           file, line, warningcode[global_warning.code],
           warningsubcode[global_warning.code][global_warning.sub_code]);
// log the error
  LogMsg(global_warning.description);

  return;
}
