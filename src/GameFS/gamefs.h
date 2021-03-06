#ifndef _CORE_GAMEFS_H_
#define _CORE_GAMEFS_H_

#include "../preproc.h"
#include "messages.h"

#ifdef _CRC_CHECK_
#include "crc32.h"
#endif  // _CRC_CHECK_

#include "../AtomError/AtomError.h"
#include <stdlib.h>
#include <stdio.h>
#include "../SysVars/guid.h"

#ifdef UNIX

#ifdef _FSMAN_
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#endif  // _FSMAN_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif  // _GNU_SOURCE

#endif  // UNIX

#define MAX_READ_LEN 16*1024*1024

#ifdef WINDOWS
FILE* fmemopen(void *s, size_t len, const char *modes);
#endif  // WINDOWS

#ifdef _FSMAN_
#ifdef UNIX
inline static int32_t dot_exclude(const struct dirent64 *dir);
#endif  // UNIX
#ifdef WINDOWS
inline static int32_t dot_exclude(const WIN32_FIND_DATA *dir);
#endif  // WINDOWS
#endif  // _FSMAN_

// main constants
static const uint32_t magic = 0x41454653;  // Magic number ("AEFS")
static const uint8_t version = 0x01;  // current version (1)
// folder and file flags
static const uint8_t ff_rw = 0x00;  // read-write
static const uint8_t ff_ro = 0x01;  // read-only

static const uint8_t flag_eoc = 0xCE;  // end of folder
static const uint8_t flag_file = 0x0F;  // file
static const uint8_t flag_folder = 0x0C;  // folder
static const uint8_t flag_folder_deleted = 0xDC;  // deleted folder
static const uint8_t flag_file_deleted = 0xDF;  // deleted file
// unicode
static const uint8_t flag_ascii = 0x00;
static const uint8_t flag_utf8 = 0xFF;
// archive types
static const uint8_t type_standard = 0x00;
//static const uint8_t type_addon = 0x01;
static const uint8_t type_critical = 0xFF;

// It is Windows ?
#ifdef WINDOWS
#pragma pack(push,1)
#endif  // WINDOWS
#ifdef UNIX
#pragma pack(1)
#endif  // UNIX

struct HEADER {
// magic number
  uint32_t magic;
// GUID
  AGUID guid;
// version of the paked file
  uint8_t version;
// type (critical, standard)
  uint8_t type;
// encoding of the symlols
  uint8_t encoding;
// crypt bytes
  uint16_t crypt;
// bin file count
  uint64_t bincount;
// filetable address
  uint64_t filetable;
};
// identify each record in filetable of the packed file
struct RECORD {
// flag of the file
// 0x0C - it's folder;
// 0x0F - it's file;
// 0xCE - it's the end of folder
// 0xDC - this folder was deleted
// 0xDF - this file was deleted
  uint8_t flag;
// lenght of the file or folder name
  uint16_t namelen;
// name of the folder or file. NULL for unicode strings
  char *name;
// utf-8 encoded name. NULL for ascii encoding
  wchar_t *wname;
// size of the file (0 if it is a folder)
  uint64_t size;
// SIC! offset of the file in data from the beginning of the data!
// (0 if it is a folder)
  uint64_t offset;
// control check sum (0 if it is a folder)
  uint64_t crc;
// link to the next record;
  RECORD *next;
};
// It is Windows ?
#ifdef WINDOWS
//TODO(Lawliet): Check this!
#pragma pack(pop)
#endif  // WINDOWS
#ifdef UNIX
#pragma unpack
#endif  // UNIX
struct TREE_FILE;
struct TREE_FOLDER;
// struct of the file in the file system
struct TREE_FILE {
// name of the file. NULL for unicode strings
  char *name;
// utf-8 encoded name. NULL for ascii encoding
  wchar_t *wname;
// size of the file
  uint64_t size;
// offset of the file in the packed file
  uint64_t offset;
// file flags (mode of the opened file)
  uint8_t flag;
// crypted bytes
  uint16_t bytescrypt;
// SIC! only for file manager! not for engine!
#ifdef _CRC_CHECK_
// control check sum
  uint64_t crc;
#endif  // _CRC_CHECK_
// id of the packed file which contain this file
// (first packed file if the packed file is separated)
  FILE *id;
// rewrite priority of the file
  uint8_t priority;
// pointer to the opened file
  FILE *file;
// counter of the opened instances of the file
  uint16_t descriptor;
// addon key (if this is not addon file key MUST be equal zero)
  uint32_t *key;
// wake table
  uint32_t *table;
// pointer to the next file in folder
  TREE_FILE *tree_file;
// pointer to the parent folder
  TREE_FOLDER *parent_folder;
};
// struct of the folder in the file system
struct TREE_FOLDER {
// name of the folder. NULL for unicode strings
  char* name;
// utf-8 encoded name. NULL for ascii encoding
  wchar_t *wname;
// flags of the folder
  uint8_t flag;
// pointer to the next folder inside this folder
  TREE_FOLDER *tree_folder;
// pointer to the first file inside this folder
  TREE_FILE *tree_file;
// pointer to the next folder near this folder
  TREE_FOLDER *next_folder;
// pointer to the parent folder
  TREE_FOLDER *parent_folder;
};
// structure to store opened files and allocated global blocks of memory
struct  OPENALLOC {
  FILE *file;
  OPENALLOC *next;
};
// structure to store used addon keys
struct ADDON_KEY {
  uint32_t count;
  uint32_t **addon_key;
  uint32_t **addon_table;
};
// FS Class
class AtomFS {
 public:
// constructors
  explicit AtomFS(AtomLog *log, uint32_t *key = 0);
/* Mounting single file
   filename - name of the mounted file
   mountfolder - folder in the FS to mount the file system
                 located in the mounted file
   key - wake key
   return value:
                0 - there is no error
                another value - some kind of error, look error code
*/
  int32_t Mount(char* filename, char* mountfolder, uint32_t *key = 0);
/* Mounting from the mount table
   filename - name of the file with configuration to mount files
   return value:
                0 - there is no error
                another value - some kind of error, look error code
*/
  int32_t Mount(char* filename);
// open file from FS
  FILE* Open(char* name, TREE_FOLDER *current = 0);
// close opened file from FS
  void Close(FILE *file);
#ifdef _FSMAN_
/* Create new file
   input - name of the folders or files or './' for current folder
   count - count of the input files and/or folders
   file - output filename
   encrypt - count of bytes to encrypt
   key - 16-byte key
   return value:
                0 - there is no error
                another value - some kind of error, look error code
*/
  int32_t Create(char** input, uint32_t count, char* file,
             uint16_t encrypt, uint32_t *key,
             uint8_t type);
// save file from FS to disc
  int32_t Save(FILE *input, char *output);
// Navigate VirtualFS
  int32_t Navigate(void);
#endif  // _FSMAN_
// destructor
  ~AtomFS();
 private:
// root directory
  TREE_FOLDER *root;
  AtomLog *atomlog;
  uint16_t bytescrypt;  // count of first bytes to encrypt
// WAKE crypt algorithm
  uint32_t *wake_table;
  uint32_t *wake_key;
// additional keys
  ADDON_KEY addon_key;
#ifdef _FSMAN_
  uint64_t datsize;  // size of data in file
  uint64_t binsize;  // size of bin in file
#endif  // _FSMAN_
// Add new addon key and return it's number
  uint32_t AddAddonKey(uint32_t *key);
  uint32_t* GenKey(uint32_t k0, uint32_t k1,
                   uint32_t k2, uint32_t k3);
  void Decrypt(uint32_t *data, int32_t lenght, uint32_t k[4],
               uint32_t r[4], uint32_t *t);
#ifdef _FSMAN_
  void Crypt(uint32_t *data, int32_t lenght, uint32_t k[4],
             uint32_t r[4], uint32_t *t);
// Scan for all files and directories to add to the packed file
  int32_t FolderScan(char *ch, RECORD *list, FILE *bin, int32_t level);
// Write data from added files
  int32_t Write(char *in, RECORD *list, FILE *bin, char *shortname = 0);
#endif  // _FSMAN_
  OPENALLOC *openalloc;
};

#endif  // _CORE_GAMEFS_H_
