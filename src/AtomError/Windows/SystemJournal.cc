#include "SystemJournal.h"
AtomSystemJournal::AtomSystemJournal(char *appName, char *msgFileName, 
                                     int CatCount) {
  if (appName == 0) {
    throw;
  }
  bool bCreatemsgFileName = false;
  uint32_t len = 0;
  if (msgFileName == 0) {
// create predefined name
    uint32_t len = strlen(appName) + 9;
    bCreatemsgFileName = true;
    msgFileName = new char[len];
    snprintf(msgFileName, len, "%s_msg.dll", appName);
  } else {
    len = strlen(msgFileName);
  }
  HKEY hKey, hRootKey = HKEY_LOCAL_MACHINE;
  const char cStPath[] = 
    "SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application\\";
  const char cCatMessFile[] = "CategoryMessageFile";
  const char cEvMessFile[] = "EventMessageFile";
  const char cParMessFile[] = "ParameterMessageFile";
  const char cCatCount[] = "CategoryCount";
  const char cTypSupp[] = "TypeSupported";
  const uint32_t TypeSupported = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE;
  uint32_t AppRegPathLen = strlen(cStPath) + strlen(appName) + 1;
  char *appRegPath = new char[AppRegPathLen];
  snprintf(appRegPath, AppRegPathLen, "%s%s", cStPath, appName);
  uint64_t disp;
  if (RegCreateKeyEx(hRootKey, appRegPath, 0, NULL, REG_OPTION_NON_VOLATILE,
                     KEY_WRITE, NULL, &hKey, 
                     (LPDWORD)&disp) == ERROR_SUCCESS) {
    try {
      if (RegSetValueEx(hKey, cCatMessFile, NULL, REG_EXPAND_SZ, 
          (LPBYTE)msgFileName, len + 1) != ERROR_SUCCESS)
        throw;
      if (RegSetValueEx(hKey, cEvMessFile, NULL, REG_EXPAND_SZ, 
          (LPBYTE)msgFileName, len + 1) != ERROR_SUCCESS)
        throw;
      if (RegSetValueEx(hKey, cParMessFile, NULL, REG_EXPAND_SZ, 
          (LPBYTE)msgFileName, len + 1) != ERROR_SUCCESS)
        throw;
      if (RegSetValueEx(hKey, cCatCount, NULL, REG_DWORD, 
          (LPBYTE)&CatCount, sizeof(CatCount)) != ERROR_SUCCESS)
        throw;
      if (RegSetValueEx(hKey, cTypSupp, NULL, REG_DWORD, 
          (LPBYTE)&TypeSupported, sizeof(TypeSupported)) != ERROR_SUCCESS)
        throw;
    } catch (...) {
      delete [] appRegPath;
      if (bCreatemsgFileName == true)
        delete [] msgFileName;
      throw;
    }    
  } else {
    delete [] appRegPath;
    if (bCreatemsgFileName == true)
      delete [] msgFileName;
    throw;
  }
// cleaning
  delete [] appRegPath;
  if (bCreatemsgFileName == true)
    delete [] msgFileName;
// register event source
  hEventSource = RegisterEventSource(NULL, appName);
  if (hEventSource == NULL)
    throw;
}

AtomSystemJournal::~AtomSystemJournal() {
// deregister event source
  DeregisterEventSource(hEventSource);
}

void AtomSystemJournal::AtomReportEvent(uint32_t cat, uint32_t mess, uint16_t type,
                                    const char *message, const char *file, 
                                    int32_t line) {
  if (type == EVENTLOG_ERROR_TYPE) {
    global_error.code = cat;
    global_error.sub_code = mess;
  } else if (type == EVENTLOG_WARNING_TYPE) {
    global_warning.code = cat;
    global_warning.sub_code = mess;
  }
  char msg[5][MSG_BUFFER_SIZE];
  snprintf(msg[0], MSG_BUFFER_SIZE, "File: %s", file);
  snprintf(msg[1], MSG_BUFFER_SIZE, "Line: %i", line);
  uint8_t msg_str_count = 2;
  if (message != 0)
    snprintf(msg[msg_str_count++], MSG_BUFFER_SIZE, "Message: %s", message);
  if (type == EVENTLOG_ERROR_TYPE && verbose_level >= SHOW_ONLY_SYSTEM_ERRORS) {
    global_error.system_code = GetLastError();
    snprintf(msg[msg_str_count++], MSG_BUFFER_SIZE, "Last system error: 0x%X", global_error.system_code);
    if (type >= SHOW_ALL_ERRORS) {
      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, global_error.system_code,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), msg[msg_str_count++],
                  MSG_BUFFER_SIZE, NULL);
    }
  }
  if(!::ReportEvent(hEventSource, type, cat, mess, NULL, msg_str_count, 0, 
     (LPCSTR*)msg, NULL)) {
    throw;
  }
}

void AtomSystemJournal::SetLastError(uint32_t code, uint32_t subcode,
                                     const char* file, int32_t line) {
  AtomReportEvent(code, subcode, EVENTLOG_ERROR_TYPE, 0, file, line);
}
void AtomSystemJournal::SetLastWarning(uint32_t code, uint32_t subcode,
                                       const char* file, int32_t line) {
  AtomReportEvent(code, subcode, EVENTLOG_WARNING_TYPE, 0, file, line);
}

void AtomSystemJournal::LogMsg(const char *string, uint8_t lvl, const char *file,
                               int32_t line) {
  if (lvl <= verbose_level) {
    AtomReportEvent(0, 0, EVENTLOG_INFORMATION_TYPE, string, file, line);
  }
}

void AtomSystemJournal::DebugMsg(const char *string, uint8_t lvl, const char *file,
                                 int32_t line) {
#ifdef ATOM_DEBUG
  if (lvl <= verbose_level) {
    AtomReportEvent(0, 0, EVENTLOG_INFORMATION_TYPE, string, file, line);
  }
#endif  // ATOM_DEBUG
  return;
}
