#include "error.h"
AtomLog::AtomLog () {
// get current date and time
  time_t seconds = time(NULL);
  tm* timeinfo = localtime(&seconds);
  char buffer [80];
  char logfilename [100];
  char *pbuffer = buffer;
  char *plogfilename = logfilename;
  pbuffer = asctime(timeinfo);

#ifdef _FSMAN_
  strcpy(plogfilename, "log/fsman ");
#else
  strcpy(plogfilename, "log/atom ");
#endif // _FSMAN_
  strcat(plogfilename, pbuffer);
  strcat(plogfilename, ".log");
//open log file
 logfile = fopen(plogfilename, "wt");
}
AtomLog::~AtomLog () {
// close log file
  fclose(logfile);
}
void AtomLog::LogMessage (char *string) {
  fputs(string, logfile), fputs("\n", logfile), fflush(logfile);
}
void AtomLog::DebugMessage (char *string) {
#ifdef ATOM_DEBUG
  LogMessage (string);
#endif
return;
}
void AtomLog::SetLastErr (unsigned int code, unsigned int subcode) {
  char *s_code,*s_subcode;
  unsigned int len = 0;
  global_error.code = code;
  global_error.sub_code = subcode;
// looking for error code
  switch (global_error.code) {
    case ERROR_CORE_FS: {
      s_code = new char [33];
      s_code = "Core Error. File System Error.\t";
      len = 33;
// looking for error subcode
      switch (global_error.sub_code) {
        case ERROR_OPEN_FILE: {
          s_subcode = new char [31];
          s_subcode = "Error while opening the file.\n";
          len += 31;
          break;
        }
        case ERROR_READ_FILE: {
          s_subcode = new char [31];
          s_subcode = "Error while reading the file.\n";
          len += 31;
          break;
        }
        case ERROR_PARSE_MOUNT_FILE_QUOTES: {
          s_subcode = new char [56];
          s_subcode = "Error while parsing the mount file. No closing quotes.";
          len += 56;
          break;
        }
        case ERROR_PARSE_MOUNT_FILE_PRIORITY: {
          s_subcode = new char [78];
          s_subcode = "Error while parsing the mount file. To big priority or it's somesthing else.";
          len += 78;
          break;
        }
        case ERROR_MOUNT_FS: {
          s_subcode = new char [38];
          s_subcode = "Error while mounting the file system.";
          len += 78;
          break;
        }
      }
      break;
    }
  }
// add an error description
  global_error.description = new char [len+7];
  strcpy(global_error.description, "ERROR:\t");
  strcat(global_error.description, s_code);
  strcat(global_error.description, s_subcode);
// log the error
  LogMessage(global_error.description);
// cleaning
  delete [] s_code;
  delete [] s_subcode;
}
void AtomLog::SetLastWrn (unsigned int code, unsigned int subcode) {
  return;
}