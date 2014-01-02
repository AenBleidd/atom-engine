#ifndef _ATOMFS_H_
#define _ATOMFS_H_

#include <preproc.h>
#include "OCryptEngine.h"
#include "OControlSumEngine.h"
#include <AtomLog/AtomLog.h>
#include <SysVars/guid.h>

#ifdef WINDOWS
#elif UNIX
#endif

#ifdef WINDOWS
#define AtomFSFile HANDLE
#elif
#define AtomFSFile FILE*
#endif


///////////////////////////////////////////////////////////////////////////////
/*
 AtomFS bin file structure (version 1)

 This version of FS supports only ASCII encoding of the strings

 ---------- Begin                       ----------
 ---------- Header                      ----------
 Magic number                           - 4 bytes         (0x41454653)
 AtomFS version                         - 1 byte          (0x01)
 Filetable address                      - 8 bytes
 ---------- Crypt engine header         ----------
 Crypt engine name length               - 1 byte          (0x0 if the atomfs is non-crypted)
 Crypt engine name                      - 1-255 bytes     (presence only if "Crypt engine name length" is not 0x0)
 Other specific parameters              - variable        (presence only if "Crypt engine name length" is not 0x0)
 ---------- Control Sum engine header   ----------
 Control Sum engine name length         - 1 byte          (0x0 if there is no control sum engine)
 Control Sum engine name                - 1-255 bytes     (presence only if "Control Sum engine name length" is not 0x0)
 Other specific parameters              - variable        (presence only if "Control Sum engine name length" is not 0x0)
 ---------- Data                        ----------
 ---------- Filetable                   ----------
 Type of record                         - 1 byte          (0x0C - it's folder; 0x0F - it's file; 0xDC - this folder was deleted; 0xDF - this file was deleted;)
 Unique identifier                      - 16 bytes        (GUID)
 Name length                            - 2 bytes
 Name                                   - 1-65535 bytes
 Identifier of the parent folder        - 16 bytes        (GUID)
 File size                              - 8 bytes         (presence only if "Type of record" is 0x0F or 0xDF)
 File offset                            - 8 bytes         (offset from the begining of the "Data" section) (presence only if "Type of record" is 0x0F or 0xDF)
 Crypt engine specific parameters       - variable        (presence only if "Crypt engine name length" is not 0x0 and "Type of record" is 0x0F or 0xDF)
 Control Sum engine specific parameters - variable        (presence only if "Crypt engine name length" is not 0x0 and "Type of record" is 0x0F or 0xDF)
 ---------- End                         ----------


*/
///////////////////////////////////////////////////////////////////////////////

// main constants

static const char AtomFSClassName[] = "AtomFS";

static const uint32_t magic = 0x41454653;  // Magic number ("AEFS")
static const uint8_t version = 0x01;  // current version (1)

static const uint8_t flag_file = 0x0F;  // file
static const uint8_t flag_folder = 0x0C;  // folder
static const uint8_t flag_folder_deleted = 0xDC;  // deleted folder
static const uint8_t flag_file_deleted = 0xDF;  // deleted file

static const uint8_t ff_ro = 0x00;  // read-only
static const uint8_t ff_rw = 0x01;  // read-write

static const AGUID root_guid = { 0x6CAFA79F, 0x2ECD4734, 0x8E7AE2F6, 0x62E59116 };
static const AGUID null_guid = { 0x00000000, 0x00000000, 0x00000000, 0x00000000 };
static const char root_name[] = "/";

struct AtomFSHeader {
// main
  uint32_t magic;
  uint32_t fsVersion;
  uint64_t filetableAddress;
  uint8_t cryptEngineNameLength;
  char *cryptEngineName;
  OCryptEngine *cryptEngine;
  uint8_t controlSumEngineNameLength;
  char *controlSumEngineName;
  OControlSumEngine *controlSumEngine;
// service
  AtomFSHeader *prev;
  AtomFSHeader *next;
  AtomFSFile file;

  AtomFSHeader(AtomFSFile *_file, uint32_t _magic, uint32_t _fsVersion, uint64_t _filetableAddress, uint8_t _cryptEngineNameLength = 0,
                        char *_cryptEngineName = 0, OCryptEngine *_cryptEngine = 0, uint8_t _controlSumEngineNameLength = 0,
                        char *_controlSumEngineName = 0, OControlSumEngine *_controlSumEngine = 0, AtomFSHeader *_prev = 0, AtomFSHeader *_next = 0);
  AtomFSHeader(void);
  ~AtomFSHeader(void);
};

struct AtomFSFiletableRecord {
// main
  uint8_t recordType;
  AGUID recordID;
  uint16_t recordNameLength;
  char *recordName;
  AGUID recordParentID;
  uint64_t filesize;
  uint64_t offset;
  OCryptEngine *cryptEngine;
  OControlSumEngine *controlSumEngine;
// additional
  uint8_t recordMode; // (0x00 - ReadOnly, 0x01 - ReadWrite)
// service
  AtomFSHeader *header;
  AtomFSFiletableRecord *prev;
  AtomFSFiletableRecord *next;
  AtomFSFiletableRecord *parent;
  AtomFSFiletableRecord *child;

  AtomFSFiletableRecord(uint8_t _recordType, uint16_t _recordNameLength, char *_recordName, AGUID _recordID, 
                                 AGUID _recordParentID = null_guid, AtomFSFiletableRecord *_parent = 0, uint64_t _filesize = 0, 
                                 uint64_t _offset = 0, uint8_t _recordMode = ff_ro, OCryptEngine *_cryptEngine = 0, 
                                 OControlSumEngine *_controlSumEngine = 0, AtomFSHeader *_header = 0, 
                                 AtomFSFiletableRecord *_prev = 0, AtomFSFiletableRecord *_next = 0, AtomFSFiletableRecord *_child = 0);
  AtomFSFiletableRecord(void);
  ~AtomFSFiletableRecord(void);
};

class AtomFS {
public:
  explicit AtomFS(AtomLog *log);
  ~AtomFS(void);

  bool Create(char **input, uint64_t count, char *filename);

private:
  AtomFSHeader *listAtomFSHeader;
  AtomFSFiletableRecord *listAtomFSFiletableRecord;
  AtomLog *atomlog;
};

#endif  // _ATOMFS_H_