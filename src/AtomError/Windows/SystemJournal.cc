#include "SystemJournal.h"
AtomSystemJournal::AtomSystemJournal(char *appName, char *msgFileName, int CatCount) {
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
  const char cStPath[] = "\\SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application\\";
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
  if (RegCreateKeyExA(hRootKey, appRegPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, (LPDWORD)&disp) == ERROR_SUCCESS) {
    try {
      if (RegSetValueExA(hKey, cCatMessFile, NULL, REG_EXPAND_SZ, (LPBYTE)msgFileName, len + 1) != ERROR_SUCCESS)
        throw;
      if (RegSetValueExA(hKey, cEvMessFile, NULL, REG_EXPAND_SZ, (LPBYTE)msgFileName, len + 1) != ERROR_SUCCESS)
        throw;
      if (RegSetValueExA(hKey, cParMessFile, NULL, REG_EXPAND_SZ, (LPBYTE)msgFileName, len + 1) != ERROR_SUCCESS)
        throw;
      if (RegSetValueExA(hKey, cCatCount, NULL, REG_DWORD, (LPBYTE)&CatCount, sizeof(CatCount)) != ERROR_SUCCESS)
        throw;
      if (RegSetValueExA(hKey, cTypSupp, NULL, REG_DWORD, (LPBYTE)&TypeSupported, sizeof(TypeSupported)) != ERROR_SUCCESS)
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
  hEventSource = RegisterEventSourceA(NULL, appName);
  if (hEventSource == NULL)
    throw;
}

AtomSystemJournal::~AtomSystemJournal() {
// deregister event source
  DeregisterEventSource(hEventSource);
}

void AtomSystemJournal::ReportEvent(uint32_t cat, uint32_t mess, uint16_t type, const char* file, int32_t line) {
  char **msg = new char*[2];  
  uint32_t file_len = strlen(file) + 7;
  uint32_t line_len = 17;
  msg[0] = new char[file_len];
  msg[1] = new char[line_len];
  snprintf(msg[0], file_len, "File: %s", file);
  snprintf(msg[1], line_len, "Line: %i", line);
  if(!::ReportEvent(hEventSource, type, cat, mess, NULL, 2, 0, (LPCSTR*)msg, NULL)) {
    throw;
  }
}