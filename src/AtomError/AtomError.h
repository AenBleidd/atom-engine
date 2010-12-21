#ifndef _ATOM_ERROR_H_
#define _ATOM_ERROR_H_

#ifdef WINDOWS
#include <windows.h>
#endif  // WINDOWS
#include <string.h>
#include <stdio.h>
#include <ctime>
#ifdef UNIX
#include <unistd.h>
#include <sys/stat.h>
#endif  // UNIX

#ifdef WINDOWS
#define localtime_r(timep, result)  (localtime (timep) ? memcpy  ((result), \
localtime (timep), sizeof (*(result))) : 0)
#endif  // WINDOWS

#include "../preproc.h"
#define MSG_BUFFER_SIZE 1024

struct ERR {
  uint32_t code;
  uint32_t sub_code;
  char *description;
};

class AtomLog {
 public:
  explicit AtomLog(char *name = "atom", bool alone = false,
                    uint8_t lvl = 0xFF);
  ~AtomLog();
  char MsgBuf[MSG_BUFFER_SIZE];
 private:
// global last error and warning
  ERR global_error;
  ERR global_warning;
// logfile
  FILE *logfile;
 public:
// Current dtae and time
  char* CurDateTime();
// Current time
  char* CurTime();
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
 private:
// verbose level
  uint8_t verbose_level;
// write log message
  void LogMsg(const char *string);
// write debug log message
  void DebugMsg(const char *string);
};

#define SetLastErr(code,subcode) SetLastError(code,subcode,__FILE__,__LINE__);
#define SetLastWrn(code,subcode) SetLastWarning(code,subcode,__FILE__,__LINE__);
#define LogMessage(string) LogMsg(string,0,__FILE__,__LINE__);
#define DebugMessage(string) DebugMsg(string,0,__FILE__,__LINE__);
#define LogMessageV(string,lvl) LogMsg(string,lvl,__FILE__,__LINE__);
#define DebugMessageV(string,lvl) DebugMsg(string,lvl,__FILE__,__LINE__);

/* Module General Error Codes */
#define ERROR_CORE_FS                                                0x00000001
#define ERROR_ENGINE_WM                                              0x00000002

/* GameFS Error Codes */
#define ERROR_OPEN_FILE                                              0x00000001
#define ERROR_READ_FILE                                              0x00000002
#define ERROR_PARSE_MOUNT_FILE_QUOTES                                0x00000003
#define ERROR_PARSE_MOUNT_FILE                                       0x00000004
#define ERROR_MOUNT_FS                                               0x00000005
#define ERROR_OPEN_FOLDER                                            0x00000006
#define ERROR_WRITE_FILE                                             0x00000007
#define ERROR_INCORRECT_FILE                                         0x00000008
#define ERROR_INCORRECT_MOUNTPOINT                                   0x00000009
#define ERROR_OVERWRITE_DENIED                                       0x0000000A
#define ERROR_LOST_QUOTES                                            0x0000000B
#define ERROR_PARSE_STRING                                           0x0000000C
#define ERROR_WRONG_BYTEORDER                                        0x0000000D
#define ERROR_INCORRECT_PATH                                         0x0000000E
#define ERROR_INCORRECT_CRC32                                        0x0000000F
#define ERROR_FUNCTION_ARGUMENTS                                     0x00000010
#define ERROR_OLD_FSMAN                                              0x00000011

/* WindowManager Error Codes */
#define ERROR_CLSREG_FAIL                                            0x00000001
#define ERROR_WND_CR_FAIL                                            0x00000002
#define ERROR_INVALID_INPUT_PARAMETER                                0x00000003
#define ERROR_CLSUNREG_FAIL                                          0x00000004
#define ERROR_TITLE                                                  0x00000005
#define ERROR_WINDOWINFO_QUERY_FAIL                                  0x00000006
#define ERROR_WINDOWINFO_SETTEXT                                     0x00000007
#define ERROR_MVWIN_FAIL                                             0x00000008
#define ERROR_ADJUSTRECT_FAIL                                        0x00000009
#define ERROR_BRING_TO_TOP                                           0x0000000A
#define ERROR_X11_CONNECTION                                         0x0000000B
#define ERROR_X11_WM_HINTS                                           0x0000000C
#define ERROR_X11_STRING_CONVERSION                                  0x0000000D

/* Module General Warning Codes */
#define WARNING_CORE_FS                                              0x00000001

/* GameFS Warning Codes */
#define WARNING_OVERWRITE                                            0x00000001
#define WARNING_EMPTY_STRING                                         0x00000002
#define WARNING_WRONG_COMMAND                                        0x00000003
#define WARNING_INCORRECT_PATH                                       0x00000004

#endif  // _CORE_ERROR_H_
