#ifndef _ATOM_ERROR_H_
#define _ATOM_ERROR_H_

#ifndef UNIX
#include <windows.h>
#endif  // UNIX
#include <string.h>
#include <stdio.h>
#include <ctime>
#ifdef UNIX
#include <sys/stat.h>
#endif  // UNIX

#ifndef UNIX
#define localtime_r(timep, result)  (localtime (timep) ? memcpy  ((result), \
localtime (timep), sizeof (*(result))) : 0)  /*NOLINT*/
#endif  // UNIX

#include "../preproc.h"
#define MSG_BUFFER_SIZE 1024

struct ERR {
  int code;
  int sub_code;
  char *description;
};
// returns current system time in text format
// can be useful in another modules
char* CurDateTime(void);

class AtomLog {
 public:
  explicit AtomLog(char *name = "atom", bool alone = false,
                    unsigned char lvl = 0xFF);
  ~AtomLog();
  char const* MsgBuf;
 private:
// global last error and warning
  ERR global_error;
  ERR global_warning;
// logfile
  FILE *logfile;
 public:
// get last error and warning
  inline ERR GetLastErr() { return global_error; }
  inline ERR GetLastWrn() { return global_warning; }
// set verbose level
  inline void SetLogVerbosity(unsigned char lvl) { verbose_level = lvl; }
// get verbose level
  inline unsigned char GetLogVerbosity() { return verbose_level; }
// write last error and warning into the log
  void SetLastError(unsigned int code, unsigned int subcode,
                    const char* file, int line);
  void SetLastWarning(unsigned int code, unsigned int subcode,
                      const char* file, int line);
// write log message
  void LogMsg(const char *string, unsigned char lvl, const char *file,
              int line);
// write debug log message
  void DebugMsg(const char *string, unsigned char lvl, const char *file,
                int line);
 private:
// verbose level
  unsigned char verbose_level;
// write log message
  void LogMsg(const char *string);
// write debug log message
  void DebugMsg(const char *string);
};

#define SetLastErr(code,subcode) SetLastError(code,subcode,__FILE__,__LINE__);  /*NOLINT*/
#define SetLastWrn(code,subcode) SetLastWarning(code,subcode,__FILE__,__LINE__);  /*NOLINT*/
#define LogMessage(string) LogMsg(string,0,__FILE__,__LINE__);  /*NOLINT*/
#define DebugMessage(string) DebugMsg(string,0,__FILE__,__LINE__);  /*NOLINT*/
#define LogMessageV(string,lvl) LogMsg(string,lvl,__FILE__,__LINE__);  /*NOLINT*/
#define DebugMessageV(string,lvl) DebugMsg(string,lvl,__FILE__,__LINE__);  /*NOLINT*/

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
#define ERROR_X11_OPENDISPLAY                                        0x0000000B

/* Module General Warning Codes */
#define WARNING_CORE_FS                                              0x00000001

/* GameFS Warning Codes */
#define WARNING_OVERWRITE                                            0x00000001
#define WARNING_EMPTY_STRING                                         0x00000002
#define WARNING_WRONG_COMMAND                                        0x00000003
#define WARNING_INCORRECT_PATH                                       0x00000004

#endif  // _CORE_ERROR_H_
