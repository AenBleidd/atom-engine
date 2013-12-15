#ifndef _ATOMFS_H_
#define _ATOMFS_H_

#include <preproc.h>
#include "OCryptEngine.h"
#include "OControlSumEngine.h"
<<<<<<< local
#include <AtomError/AtomError.h>
=======
#include <AtomLog/AtomLog.h>
>>>>>>> other
#include <SysVars/guid.h>

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

static const uint32_t magic = 0x41454653;  // Magic number ("AEFS")
static const uint8_t version = 0x01;  // current version (1)

static const uint8_t flag_file = 0x0F;  // file
static const uint8_t flag_folder = 0x0C;  // folder
static const uint8_t flag_folder_deleted = 0xDC;  // deleted folder
static const uint8_t flag_file_deleted = 0xDF;  // deleted file

static const uint8_t ff_ro = 0x00;  // read-only
static const uint8_t ff_rw = 0x01;  // read-write

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
};

class AtomFS {
public:
  explicit AtomFS(AtomLog *log);

private:
  AtomFSHeader *listAtomFSHeader;
  AtomFSFiletableRecord *listAtomFSFiletableRecord;
  AtomLog *atomlog;
};

#endif  // _ATOMFS_H_