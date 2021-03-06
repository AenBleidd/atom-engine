#ifndef _ATOM_ERROR_H_
#define _ATOM_ERROR_H_

#include <preproc.h>

#ifdef WINDOWS
#include <windows.h>
#ifdef __BORLANDC__
#include <time.h>
#endif  // __BORLANDC__
#endif  // WINDOWS
#include <string.h>
#include <ctime>
#include <stdio.h>
#ifdef UNIX
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#endif  // UNIX

#ifdef ATOM_TEST
#include <gtest/gtest.h>
#endif  // ATOM_TEST// some verbosity levels
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

class AtomLog {
// Declare friend class for testing AtomLog
#ifdef ATOM_TEST
 private:
  FRIEND_TEST(AtomErrorTest, Test_GetLogPath);
#endif  // ATOM_TEST
 public:
  explicit AtomLog(char *name = "atom", bool alone = false,
                    uint8_t lvl = 0xFF);
  ~AtomLog();
  char MsgBuf[MSG_BUFFER_SIZE];
 private:
// global last error and warning
  ERR global_error;
  ERR global_warning;
// errors and warnings
  char **errorcode;
  char ***errorsubcode;
  char **warningcode;
  char ***warningsubcode;

// logfile
  FILE *logfile;
 public:
// Current date and time
   virtual char* CurDateTime();
// Current time
   virtual char* CurTime();
// get last error and warning
  inline ERR GetLastErr() { return global_error; }
  inline ERR GetLastWrn() { return global_warning; }
// set verbose level
  inline void SetLogVerbosity(uint8_t lvl) { verbose_level = lvl; }
// get verbose level
  inline uint8_t GetLogVerbosity() { return verbose_level; }
// write last error and warning into the log
  void SetLastError(uint32_t code, uint32_t subcode,
                    const char* file, int32_t line);
  void SetLastWarning(uint32_t code, uint32_t subcode,
                      const char* file, int32_t line);
// write log message
  void LogMsg(const char *string, uint8_t lvl, const char *file,
              int32_t line);
// write debug log message
  void DebugMsg(const char *string, uint8_t lvl, const char *file,
                int32_t line);
// load error descriptions and return module code
// type - ERROR or WARNING
#define ATOMERROR   true
#define ATOMWARNING false
  int32_t LoadStrings(bool type, char **subcodes, char *module_description);
 private:
// verbose level
  uint8_t verbose_level;
// modules count
  uint32_t module_error_count;
  uint32_t module_warning_count;
// write log message
  void LogMsg(const char *string);
// write debug log message
  void DebugMsg(const char *string);
// find some path where we can place our log file
  char* GetLogPath(char* path, uint32_t size);
// for datetime functions
  char datetimebuf[20];
  char timebuf[10];
  tm *ptimeinfo;
};

#endif  // _CORE_ERROR_H_
