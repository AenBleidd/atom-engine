#ifndef _CORE_GAMEFS_H_
#define _CORE_GAMEFS_H_

#include "../preproc.h"

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
static const uint8_t flag_key = 0x0E;  // it's wake addon key
// unicode
static const uint8_t flag_ascii = 0x00;
static const uint8_t flag_utf8 = 0xFF;
// archive types
static const uint8_t type_standart = 0x00;
static const uint8_t type_addon = 0x01;
static const uint8_t type_critical = 0xFF;
// CRC32
#ifdef _CRC_CHECK_
// crc32 table generated using standart polynom
static const uint32_t crc32table[256] = {
  0x00000000UL, 0x77073096UL, 0xEE0E612CUL, 0x990951BAUL,
  0x076DC419UL, 0x706AF48FUL, 0xE963A535UL, 0x9E6495A3UL,
  0x0EDB8832UL, 0x79DCB8A4UL, 0xE0D5E91EUL, 0x97D2D988UL,
  0x09B64C2BUL, 0x7EB17CBDUL, 0xE7B82D07UL, 0x90BF1D91UL,
  0x1DB71064UL, 0x6AB020F2UL, 0xF3B97148UL, 0x84BE41DEUL,
  0x1ADAD47DUL, 0x6DDDE4EBUL, 0xF4D4B551UL, 0x83D385C7UL,
  0x136C9856UL, 0x646BA8C0UL, 0xFD62F97AUL, 0x8A65C9ECUL,
  0x14015C4FUL, 0x63066CD9UL, 0xFA0F3D63UL, 0x8D080DF5UL,
  0x3B6E20C8UL, 0x4C69105EUL, 0xD56041E4UL, 0xA2677172UL,
  0x3C03E4D1UL, 0x4B04D447UL, 0xD20D85FDUL, 0xA50AB56BUL,
  0x35B5A8FAUL, 0x42B2986CUL, 0xDBBBC9D6UL, 0xACBCF940UL,
  0x32D86CE3UL, 0x45DF5C75UL, 0xDCD60DCFUL, 0xABD13D59UL,
  0x26D930ACUL, 0x51DE003AUL, 0xC8D75180UL, 0xBFD06116UL,
  0x21B4F4B5UL, 0x56B3C423UL, 0xCFBA9599UL, 0xB8BDA50FUL,
  0x2802B89EUL, 0x5F058808UL, 0xC60CD9B2UL, 0xB10BE924UL,
  0x2F6F7C87UL, 0x58684C11UL, 0xC1611DABUL, 0xB6662D3DUL,
  0x76DC4190UL, 0x01DB7106UL, 0x98D220BCUL, 0xEFD5102AUL,
  0x71B18589UL, 0x06B6B51FUL, 0x9FBFE4A5UL, 0xE8B8D433UL,
  0x7807C9A2UL, 0x0F00F934UL, 0x9609A88EUL, 0xE10E9818UL,
  0x7F6A0DBBUL, 0x086D3D2DUL, 0x91646C97UL, 0xE6635C01UL,
  0x6B6B51F4UL, 0x1C6C6162UL, 0x856530D8UL, 0xF262004EUL,
  0x6C0695EDUL, 0x1B01A57BUL, 0x8208F4C1UL, 0xF50FC457UL,
  0x65B0D9C6UL, 0x12B7E950UL, 0x8BBEB8EAUL, 0xFCB9887CUL,
  0x62DD1DDFUL, 0x15DA2D49UL, 0x8CD37CF3UL, 0xFBD44C65UL,
  0x4DB26158UL, 0x3AB551CEUL, 0xA3BC0074UL, 0xD4BB30E2UL,
  0x4ADFA541UL, 0x3DD895D7UL, 0xA4D1C46DUL, 0xD3D6F4FBUL,
  0x4369E96AUL, 0x346ED9FCUL, 0xAD678846UL, 0xDA60B8D0UL,
  0x44042D73UL, 0x33031DE5UL, 0xAA0A4C5FUL, 0xDD0D7CC9UL,
  0x5005713CUL, 0x270241AAUL, 0xBE0B1010UL, 0xC90C2086UL,
  0x5768B525UL, 0x206F85B3UL, 0xB966D409UL, 0xCE61E49FUL,
  0x5EDEF90EUL, 0x29D9C998UL, 0xB0D09822UL, 0xC7D7A8B4UL,
  0x59B33D17UL, 0x2EB40D81UL, 0xB7BD5C3BUL, 0xC0BA6CADUL,
  0xEDB88320UL, 0x9ABFB3B6UL, 0x03B6E20CUL, 0x74B1D29AUL,
  0xEAD54739UL, 0x9DD277AFUL, 0x04DB2615UL, 0x73DC1683UL,
  0xE3630B12UL, 0x94643B84UL, 0x0D6D6A3EUL, 0x7A6A5AA8UL,
  0xE40ECF0BUL, 0x9309FF9DUL, 0x0A00AE27UL, 0x7D079EB1UL,
  0xF00F9344UL, 0x8708A3D2UL, 0x1E01F268UL, 0x6906C2FEUL,
  0xF762575DUL, 0x806567CBUL, 0x196C3671UL, 0x6E6B06E7UL,
  0xFED41B76UL, 0x89D32BE0UL, 0x10DA7A5AUL, 0x67DD4ACCUL,
  0xF9B9DF6FUL, 0x8EBEEFF9UL, 0x17B7BE43UL, 0x60B08ED5UL,
  0xD6D6A3E8UL, 0xA1D1937EUL, 0x38D8C2C4UL, 0x4FDFF252UL,
  0xD1BB67F1UL, 0xA6BC5767UL, 0x3FB506DDUL, 0x48B2364BUL,
  0xD80D2BDAUL, 0xAF0A1B4CUL, 0x36034AF6UL, 0x41047A60UL,
  0xDF60EFC3UL, 0xA867DF55UL, 0x316E8EEFUL, 0x4669BE79UL,
  0xCB61B38CUL, 0xBC66831AUL, 0x256FD2A0UL, 0x5268E236UL,
  0xCC0C7795UL, 0xBB0B4703UL, 0x220216B9UL, 0x5505262FUL,
  0xC5BA3BBEUL, 0xB2BD0B28UL, 0x2BB45A92UL, 0x5CB36A04UL,
  0xC2D7FFA7UL, 0xB5D0CF31UL, 0x2CD99E8BUL, 0x5BDEAE1DUL,
  0x9B64C2B0UL, 0xEC63F226UL, 0x756AA39CUL, 0x026D930AUL,
  0x9C0906A9UL, 0xEB0E363FUL, 0x72076785UL, 0x05005713UL,
  0x95BF4A82UL, 0xE2B87A14UL, 0x7BB12BAEUL, 0x0CB61B38UL,
  0x92D28E9BUL, 0xE5D5BE0DUL, 0x7CDCEFB7UL, 0x0BDBDF21UL,
  0x86D3D2D4UL, 0xF1D4E242UL, 0x68DDB3F8UL, 0x1FDA836EUL,
  0x81BE16CDUL, 0xF6B9265BUL, 0x6FB077E1UL, 0x18B74777UL,
  0x88085AE6UL, 0xFF0F6A70UL, 0x66063BCAUL, 0x11010B5CUL,
  0x8F659EFFUL, 0xF862AE69UL, 0x616BFFD3UL, 0x166CCF45UL,
  0xA00AE278UL, 0xD70DD2EEUL, 0x4E048354UL, 0x3903B3C2UL,
  0xA7672661UL, 0xD06016F7UL, 0x4969474DUL, 0x3E6E77DBUL,
  0xAED16A4AUL, 0xD9D65ADCUL, 0x40DF0B66UL, 0x37D83BF0UL,
  0xA9BCAE53UL, 0xDEBB9EC5UL, 0x47B2CF7FUL, 0x30B5FFE9UL,
  0xBDBDF21CUL, 0xCABAC28AUL, 0x53B39330UL, 0x24B4A3A6UL,
  0xBAD03605UL, 0xCDD70693UL, 0x54DE5729UL, 0x23D967BFUL,
  0xB3667A2EUL, 0xC4614AB8UL, 0x5D681B02UL, 0x2A6F2B94UL,
  0xB40BBE37UL, 0xC30C8EA1UL, 0x5A05DF1BUL, 0x2D02EF8DUL
};
// XOR mask for CRC32
static const uint32_t mask = 0xFFFFFFFFUL;
#endif  // _CRC_CHECK_
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
  ERROR_OLD_FSMAN
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
"FSMan version is too old or file is from the future."
};
// Warning codes
static int32_t WARNING_CORE_FS;
static char *gamefs_warn_descr = "Core Warning. File System Warning.";
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
// type (critical, standart or addon)
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
  int32_t FolderScan(char *ch, FILE *dat, FILE *bin, int32_t level);
// Write data from added files
  int32_t Write(char *in, FILE *dat, FILE *bin, char *shortname = 0);
#endif  // _FSMAN_
  OPENALLOC *openalloc;
};

#endif  // _CORE_GAMEFS_H_
