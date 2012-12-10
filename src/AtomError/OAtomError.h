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

// some verbosity levels
enum {
// show only last system error codes
  SHOW_ONLY_SYSTEM_ERRORS = 0xFE,
// show last system error codes and their description
  SHOW_ALL_ERRORS = 0xFF
};

#define MSG_BUFFER_SIZE 1024

#define SetLastErr(code,subcode) SetLastError(code,subcode,__FILE__,__LINE__);
#define SetLastWrn(code,subcode) SetLastWarning(code,subcode,__FILE__,__LINE__);
#define LogMessage(string) LogMsg(string,SHOW_ALL_ERRORS,__FILE__,__LINE__);
#define DebugMessage(string) DebugMsg(string,SHOW_ALL_ERRORS,__FILE__,__LINE__);
#define LogMessageV(string,lvl) LogMsg(string,lvl,__FILE__,__LINE__);
#define DebugMessageV(string,lvl) DebugMsg(string,lvl,__FILE__,__LINE__);

struct ERR {
  uint32_t code;
  uint32_t sub_code;
  uint32_t system_code;
  char *description;
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