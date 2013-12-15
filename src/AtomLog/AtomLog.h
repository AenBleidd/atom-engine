#ifndef _ATOMLOG_H_
#define _ATOMLOG_H_

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
  SHOW_ONLY_ATOM_ERRORS = 0xFE,
  // show last system error codes and their description
  SHOW_ALL_ERRORS = 0xFF
};

#define MSG_BUFFER_SIZE 1024

#define SetLastErr(module,error) SetLastError(module,error,__FILE__,__LINE__);
#define SetLastWrn(module,warning) SetLastWarning(module,warning,__FILE__,__LINE__);
#define LogMessage(string) LogMsg(string,SHOW_ALL_ERRORS,__FILE__,__LINE__);
#define DebugMessage(string) DebugMsg(string,SHOW_ALL_ERRORS,__FILE__,__LINE__);
#define LogMessageV(string,lvl) LogMsg(string,lvl,__FILE__,__LINE__);
#define DebugMessageV(string,lvl) DebugMsg(string,lvl,__FILE__,__LINE__);

class AtomLog {
// Declare friend class for testing AtomLog
#ifdef ATOM_TEST
private:
  FRIEND_TEST(AtomLogTest, Test_GetLogPath);
#endif  // ATOM_TEST
public:
  explicit AtomLog(char *name = "atom", bool alone = false,
    uint8_t lvl = 0xFF);
  ~AtomLog();
  char MsgBuf[MSG_BUFFER_SIZE];
// logfile
  FILE *logfile;
// Current date and time
  char* CurDateTime();
// Current time
  char* CurTime();
// set verbose level
  inline void SetLogVerbosity(uint8_t lvl) { verbose_level = lvl; }
// get verbose level
  inline uint8_t GetLogVerbosity() { return verbose_level; }
// write last error and warning into the log
  void ErrorMessage(const char* module, const char* error,
    const char* file, int32_t line);
  void WarningMessage(const char* module, const char* warning,
    const char* file, int32_t line);
// write log message
  void LogMsg(const char *string, uint8_t lvl, const char *file,
    int32_t line);
// write debug log message
  void DebugMsg(const char *string, uint8_t lvl, const char *file,
    int32_t line);
private:
// verbose level
  uint8_t verbose_level;
// for datetime functions
  char datetimebuf[20];
  char timebuf[10];
  tm *ptimeinfo;
// find some path where we can place our log file
  char* GetLogPath(char* path, uint32_t size);
// write log message
  void LogMsg(const char *string);
// write debug log message
  void DebugMsg(const char *string);
};

#endif // _ATOMLOG_H_
