// This file was automatically created by CMake script
#ifndef _GAMEFS_MESSAGES_H_
#define _GAMEFS_MESSAGES_H_
// Error codes
static int32_t ERROR_CORE_FS;
static char *gamefs_error_description = "Core Error. File System Error.";
/* GameFS Error Codes */
enum {
  NOERRORS = 0x00000000,
  ERROR_OPEN_FILE,
  ERROR_READ_FILE,
  ERROR_PARSE_MOUNT_FILE_QUOTES,
  ERROR_PARSE_MOUNT_FILE,
  ERROR_MOUNT_FS,
  ERROR_OPEN_FOLDER,
  ERROR_WRITE_FILE,
  ERROR_INCORRECT_FILE,
  ERROR_INCORRECT_MOUNTPOINT,
  ERROR_OVERWRITE_DENIED,
  ERROR_LOST_QUOTES,
  ERROR_PARSE_STRING,
  ERROR_WRONG_BYTEORDER,
  ERROR_INCORRECT_PATH,
  ERROR_INCORRECT_CRC32,
  ERROR_FUNCTION_ARGUMENTS,
  ERROR_OLD_FSMAN,
  ERROR_ADD_FILE
};
/* GameFS Error Descriptions */
static char *gamefserrorcodes[] = {
  "No Error.",
  "Error opening the file.",
  "Error reading the file.",
  "Error parsing the mount file. No closing quotes.",
  "Error parsing the mount file.",
  "Error mounting the file system.",
  "Couldn't open the directory.",
  "Error while writing the file.",
  "Incorrect file.",
  "Incorrect mountpoint",
  "Can't overwrite the file.",
  "Lost quotes.",
  "Error parsing the string.",
  "Wrong byteorder. Program can't read files with unknown byteorder.",
  "Path incorrect or directory or file don't exist.",
  "Crc check is failed. File is broken.",
  "Function was called with wrong arguments.",
  "FSMan version is too old or file is from the future.",
  "Couldn't add the file."
};
// Warning codes
static int32_t WARNING_CORE_FS;
static char *gamefs_warn_description = "Core Warning. File System Warning.";
/* GameFS Warning Codes */
enum {
  NOWARNINGS = 0x00000000,
  WARNING_OVERWRITE,
  WARNING_EMPTY_STRING,
  WARNING_WRONG_COMMAND,
  WARNING_INCORRECT_PATH
};
/* GameFS Warning Descriptions */
static char *gamefswarncodes[] = {
  "No Warning.",
  "File was overwritten.",
  "Empty string. Nothing to parse.",
  "Wrong syntax or unknown command.",
  "Path incorrect or directory doesn't exist."
};
#endif  // _GAMEFS_MESSAGES_H_
