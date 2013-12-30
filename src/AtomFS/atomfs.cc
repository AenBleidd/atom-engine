#include "atomfs.h"

AtomFSHeader::AtomFSHeader(AtomFSFile *_file, uint32_t _magic, uint32_t _fsVersion, uint64_t _filetableAddress, uint8_t _cryptEngineNameLength,
                           char *_cryptEngineName, OCryptEngine *_cryptEngine, uint8_t _controlSumEngineNameLength,
                           char *_controlSumEngineName, OControlSumEngine *_controlSumEngine, AtomFSHeader *_prev, AtomFSHeader *_next) {
  file = _file;
  magic = _magic;
  fsVersion = _fsVersion;
  filetableAddress = _filetableAddress;
  cryptEngineNameLength = _cryptEngineNameLength;
  cryptEngineName = _cryptEngineName;
  cryptEngine = _cryptEngine;
  controlSumEngineNameLength = _controlSumEngineNameLength;
  controlSumEngineName = _controlSumEngineName;
  controlSumEngine = _controlSumEngine;
  prev = _prev;
  next = _next;
}

AtomFSHeader::AtomFSHeader(void) {
  file = 0;
  magic = 0;
  fsVersion = 0;
  filetableAddress = 0;
  cryptEngineNameLength = 0;
  cryptEngineName = 0;
  cryptEngine = 0;
  controlSumEngineNameLength = 0;
  controlSumEngineName = 0;
  controlSumEngine = 0;
  prev = 0;
  next = 0;
}

AtomFSHeader::~AtomFSHeader(void)
{
  if (next != 0)
    delete next;
  next = 0;
  prev = 0;
}

AtomFSFiletableRecord::AtomFSFiletableRecord(uint8_t _recordType, uint16_t _recordNameLength, char *_recordName, AGUID _recordID,
                                             AGUID _recordParentID, AtomFSFiletableRecord *_parent, uint64_t _filesize,
                                             uint64_t _offset, uint8_t _recordMode, OCryptEngine *_cryptEngine,
                                             OControlSumEngine *_controlSumEngine, AtomFSHeader *_header,
                                             AtomFSFiletableRecord *_prev, AtomFSFiletableRecord *_next, AtomFSFiletableRecord *_child) {
  recordType = _recordType;
  recordNameLength = _recordNameLength;
  recordName = _recordName;
  recordID = _recordID;
  recordParentID = _recordParentID;
  parent = _parent;
  filesize = _filesize;
  offset = _offset;
  recordMode = _recordMode;
  cryptEngine = _cryptEngine;
  controlSumEngine = _controlSumEngine;
  header = _header;
  prev = _prev;
  next = _next;
  child = _child;
}

AtomFSFiletableRecord::AtomFSFiletableRecord(void) {
  recordType = 0;
  recordNameLength = 0;
  recordName = 0;
  recordID = null_guid;
  recordParentID = null_guid;
  parent = 0;
  filesize = 0;
  offset = 0;
  recordMode = 0;
  cryptEngine = 0;
  controlSumEngine = 0;
  header = 0;
  prev = 0;
  next = 0;
  child = 0;
}

AtomFSFiletableRecord::~AtomFSFiletableRecord(void)
{
  if (next != 0)
    delete next;
  next = 0;
  prev = 0;
  if (child != 0)
    delete child;
  parent = 0;
  child = 0;
}

AtomFS::AtomFS(AtomLog *log) {
  if (log == 0)
    throw -1;
  atomlog = log;
  listAtomFSHeader = 0;
  listAtomFSFiletableRecord = new AtomFSFiletableRecord(flag_folder, strlen(root_name), (char*)root_name, root_guid);
}

AtomFS::~AtomFS(void)
{
  if (listAtomFSHeader != 0)
    delete listAtomFSHeader;
  listAtomFSHeader = 0;
  if (listAtomFSFiletableRecord != 0)
    delete listAtomFSFiletableRecord;
  listAtomFSFiletableRecord = 0;
}
