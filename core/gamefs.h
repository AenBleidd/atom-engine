#ifndef _CORE_GAMEFS_H_
#define _CORE_GAMEFS_H_

#include "../preproc.h"

#ifdef _CRC_CHECK_
#include "../utils/crc32.h"
#endif //_CRC_CHECK_

#include "error.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _FSMAN_
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#endif // _FSMAN_

// main constants
static const unsigned int magic = 0x41454653; // Magic number ("AEFS")
static const unsigned char version = 0x01; // current version (1)
static const unsigned short int eoc = 0x4345; // end of catalogue ("CE")
// folder and file flags
static const unsigned char ff_ro = 0x80; // read-only
static const unsigned char ff_rw = 0x0; // read-write

struct HEADER {
// magic number
  unsigned int magic;
// version of the paked file
  unsigned char version;
// size of the packed file
  unsigned long int filesize;
// size of the filetable
  unsigned long int tablesize;
};
// identify each record in filetable of the packed file
struct RECORD {
// flag of the file (0x0 - it's folder; 0xff - it's file)
  unsigned char flag;
// lenght of the file or folder name
  unsigned short int namelen;
// name of the folder or file
  char *name;
// size of the file (0 if it is a folder)
  unsigned long int size;
// SIC! offset of the file in data from the beginning of the data!
// (0 if it is a folder)
  unsigned long int offset;
// control check sum (0 if it is a folder)
  unsigned long int crc;
// marked the end of the folder (there is no this mark if it is a file)
  unsigned short int eoc;
};
// struct of the file in the file system
struct TREE_FILE {
// name of the file
  char *name;
// size of the file
  unsigned long int size;
// offset of the file in the packed file
  unsigned long int offset;
// file flags (mode of the opened file)
  unsigned char flag;
// SIC! only for file manager! not for engine!
#ifdef _CRC_CHECK_
// control check sum
  unsigned long int crc;
#endif // _CRC_CHECK_
// id of the packed file which contain this file
  unsigned short int id;
// rewrite priority of the file
  unsigned char priority;
// pointer to the opened file
  FILE *file;
// counter of the opened instances of the file
  unsigned short int descriptor;
// pointer to the next file in folder
TREE_FILE *tree_file;
};
// struct of the folder in the file system
struct TREE_FOLDER {
// name of the folder
  char* name;
// flags of the folder
  unsigned char flag;
// pointer to the next folder
  TREE_FOLDER *tree_folder;
// pointer to the next file
  TREE_FILE *tree_file;
};
// FS Class
class AtomFS {
 public:
// constructor
  AtomFS();
/* Mounting single file
   filename - name of the mounted file
   mountfolder - folder in the FS to mount the file system
                 located in the mounted file
   priority - priority to overwrite existing files int the FS:
              0 - no one can overwrite mounted files
              another value - files can be overwritten with higher priority
                              with warning
   return value:
                0 - there is no error
                another value - some kind of error, look error code
*/
  int Mount (char* filename, char* mountfolder, unsigned char priority);
/* Mounting from the mount table
   filename - name of the file with configuration to mount files
   return value:
                0 - there is no error
                another value - some kind of error, look error code
*/
  int Mount (char* filename);
// destructor
  ~AtomFS();
 private:
// root directory
  TREE_FOLDER *root;
// WAKE crypt algorithm
  unsigned int wake_table[257];
  void GenKey (unsigned int k0, unsigned int k1, unsigned int k2, unsigned int k3);
  void Decrypt (unsigned int *data, int lenght, unsigned int k[4], unsigned int r[4], unsigned int *t);
#ifdef _FSMAN_
  void Crypt (unsigned int *data, int lenght, unsigned int k[4], unsigned int r[4], unsigned int *t);
#endif // _FSMAN_
#ifdef _FSMAN_
// Scan for all files and directories to add to the packed file
  long int FolderScan (char *ch, int level);
#endif // _FSMAN_
};
extern AtomLog atomlog;
#endif //_CORE_GAMEFS_H_
