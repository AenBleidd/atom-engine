#ifndef _ATOM_ERROR_H_
#define _ATOM_ERROR_H_

#include <preproc.h>
#include "OAtomError.h"

#ifdef WINDOWS
#include <windows.h>
#ifdef SYSJOURNAL
#include <SystemJournal.h>
#endif  // SYSJOURNAL
#ifdef __BORLANDC__
#include <time.h>
#endif  // __BORLANDC__
#endif  // WINDOWS
#include <string.h>
#include <stdio.h>
#ifdef UNIX
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#ifdef SYSJOURNAL
#include <Unix/SystemJournal.h>
#endif  // SYSJOURNAL
#endif  // UNIX

#ifdef ATOM_TEST
#include <gtest/gtest.h>
#endif  // ATOM_TEST

class AtomLog : public OAtomLog {
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
};

#endif  // _CORE_ERROR_H_
