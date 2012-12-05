#ifndef _OATOMERROR_H_
#define _OATOMERROR_H_

#include <preproc.h>

#ifdef WINDOWS
#include <windows.h>
#ifdef __BORLANDC__
#include <time.h>
#endif  // __BORLANDC__
#endif  // WINDOWS
#include <ctime>
#include <stdio.h>

#define MSG_BUFFER_SIZE 1024

struct ERR {
  uint32_t code;
  uint32_t sub_code;
  uint32_t system_code;
  char *description;
};

enum AtomLogMessages{
  SET_LAST_ERROR,
  SET_LAST_WARNING,
  DEBUG_MESSAGE,
  LOG_MESSAGE,
};

class OAtomLog {
 public:
   explicit OAtomLog();
   ~OAtomLog();
// Current date and time
  virtual char* CurDateTime();
// Current time
  virtual char* CurTime();
// get last error and warning
  virtual ERR GetLastErr() = 0;
  virtual ERR GetLastWrn() = 0;
// set verbose level
  virtual void SetLogVerbosity(uint8_t lvl) = 0;
// get verbose level
  virtual uint8_t GetLogVerbosity() = 0;
// write last error and warning into the log
  virtual void SetLastError(uint32_t code, uint32_t subcode,
                            const char* file, int32_t line) = 0;
  virtual void SetLastWarning(uint32_t code, uint32_t subcode,
                              const char* file, int32_t line) = 0;
// write log message
  virtual void LogMsg(const char *string, uint8_t lvl, const char *file,
                      int32_t line) = 0;
// write debug log message
  virtual void DebugMsg(const char *string, uint8_t lvl, const char *file,
                        int32_t line) = 0;
 private:
// for datetime functions
  char datetimebuf[20];
  char timebuf[10];
  tm *ptimeinfo;
};

#endif  // _OATOMERROR_H_