// This file was automatically created by CMake script
#ifndef _WINDOWMANAGER_MESSAGES_H_
#define _WINDOWMANAGER_MESSAGES_H_
// Error codes
static int32_t ERROR_ENGINE_WM;
static char *windowmanager_error_description = "Engine Error. WindowManager Error.";
/* WindowManager Error Codes */
enum {
  NOERRORS = 0x00000000,
  ERROR_CLSREG_FAIL,
  ERROR_WND_CR_FAIL,
  ERROR_INVALID_INPUT_PARAMETER,
  ERROR_CLSUNREG_FAIL,
  ERROR_TITLE,
  ERROR_WINDOWINFO_QUERY_FAIL,
  ERROR_WINDOWINFO_SETTEXT,
  ERROR_MVWIN_FAIL,
  ERROR_ADJUSTRECT_FAIL,
  ERROR_BRING_TO_TOP,
  ERROR_X11_CONNECTION,
  ERROR_X11_WM_HINTS,
  ERROR_X11_STRING_CONVERSION
};
/* WindowManager Error Descriptions */
static char *windowmanagererrorcodes[] = {
  "No Error",
  "Fatal Error: Error registering the window class",
  "Fatal Error: Couldn't create a window",
  "Invalid input parameter",
  "Error unregistering the window class",
  "Error getting window title",
  "Error getting the window info block",
  "Error setting the window text while applying the window info block",
  "Error trying to change window size and dimentions",
  "Error adjusting the client area for a window",
  "Error trying to make window topmost",
  "No connection to X server established",
  "Error getting WM hints: window size and position",
  "No memory for XLib string conversion"
};
#endif  // _WINDOWMANAGER_MESSAGES_H_
