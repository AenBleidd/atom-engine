#include "guid.h"
AGUID* GetAGUID (char *guid) {
  AGUID *aguid = new AGUID;
#ifdef UNIX
  uuid_t uuid;
  if (guid == 0)
    uuid_generate(uuid);  // we will generate new GUID
  else if (uuid_parse(guid, uuid) == -1) {
    delete aguid;
    return 0;
  }
// set this guid to our struct
// Long but fast.... I guess... I hope!...
  char bo = BYTEORDER;
  if (bo == BO_BE) {
    aguid->data0 = uuid[0] + uuid[1] * 256 + uuid[2] * 256 * 256 +
      uuid[3] * 256 * 256 * 256;
    aguid->data1 = uuid[4] + uuid[5] * 256;
    aguid->data2 = uuid[6] + uuid[7] * 256;
  } else if (bo == BO_LE) {
    aguid->data0 = uuid[3] + uuid[2] * 256 + uuid[1] * 256 * 256 +
      uuid[0] * 256 * 256 * 256;
    aguid->data1 = uuid[5] + uuid[4] * 256;
    aguid->data2 = uuid[7] + uuid[6] * 256;
  } else if (bo == BO_MBE) {
    aguid->data0 = uuid[1] + uuid[0] * 256 + uuid[3] * 256 * 256 +
      uuid[2] * 256 * 256 * 256;
    aguid->data1 = uuid[4] + uuid[5] * 256;
    aguid->data2 = uuid[6] + uuid[7] * 256;
  } else if (bo == BO_MLE) {
    aguid->data0 = uuid[2] + uuid[3] * 256 + uuid[0] * 256 * 256 +
      uuid[1] * 256 * 256 * 256;
    aguid->data1 = uuid[5] + uuid[4] * 256;
    aguid->data2 = uuid[7] + uuid[6] * 256;
  } else {
    delete aguid;
    return 0;
  }
  for (int i = 0; i < 8; i++)
    aguid->data3[i] = uuid[i+8];
#endif // UNIX
#ifdef WINDOWS
  GUID *uuid = new GUID;
  if (guid == 0) {
    if (UuidCreate(uuid) != RPC_S_OK) {
      delete aguid;
      delete uuid;
      return 0;
    }
  } else if (UuidFromString((unsigned char*)guid, uuid) != RPC_S_OK) {
    delete aguid;
    delete uuid;
    return 0;
  }
  aguid->data0 = (unsigned int)uuid->Data1;
  aguid->data1 = uuid->Data2;
  aguid->data2 = uuid->Data3;
  memcpy(aguid->data3, uuid->Data4, 8);
  delete uuid;
#endif  // WINDOWS
  return aguid;
}
char* GetCGUID (AGUID *guid) {
  char *cguid = new char [37];
  if (guid == 0) {
    guid = new AGUID;
    guid = GetAGUID();
    if (guid == 0) {
      delete [] cguid;
      return 0;
    }
  }
#ifdef UNIX
  uuid_t uuid;
  char bo = BYTEORDER;
  if (bo == BO_BE) {
    uuid[0] = LOBYTE(LOWORD(guid->data0));
    uuid[1] = HIBYTE(LOWORD(guid->data0));
    uuid[2] = LOBYTE(HIWORD(guid->data0));
    uuid[3] = HIBYTE(HIWORD(guid->data0));
    uuid[4] = LOBYTE(guid->data1);
    uuid[5] = HIWORD(guid->data1);
    uuid[6] = LOBYTE(guid->data2);
    uuid[7] = HIWORD(guid->data2);
  } else if (bo == BO_LE) {
    uuid[0] = HIBYTE(HIWORD(guid->data0));
    uuid[1] = LOBYTE(HIWORD(guid->data0));
    uuid[2] = HIBYTE(LOWORD(guid->data0));
    uuid[3] = LOBYTE(LOWORD(guid->data0));
    uuid[4] = HIWORD(guid->data1);
    uuid[5] = LOBYTE(guid->data1);
    uuid[6] = HIWORD(guid->data2);
    uuid[7] = LOBYTE(guid->data2);
  } else if (bo == BO_MBE) {
    uuid[0] = HIBYTE(HIWORD(guid->data0));
    uuid[1] = LOBYTE(HIWORD(guid->data0));
    uuid[2] = LOBYTE(LOWORD(guid->data0));
    uuid[3] = HIBYTE(LOWORD(guid->data0));
    uuid[4] = LOBYTE(guid->data1);
    uuid[5] = HIWORD(guid->data1);
    uuid[6] = LOBYTE(guid->data2);
    uuid[7] = HIWORD(guid->data2);
  } else if (bo == BO_MLE) {
    uuid[0] = HIBYTE(LOWORD(guid->data0));
    uuid[1] = LOBYTE(LOWORD(guid->data0));
    uuid[2] = HIBYTE(HIWORD(guid->data0));
    uuid[3] = LOBYTE(HIWORD(guid->data0));
    uuid[4] = HIWORD(guid->data1);
    uuid[5] = LOBYTE(guid->data1);
    uuid[6] = HIWORD(guid->data2);
    uuid[7] = LOBYTE(guid->data2);
  } else {
    delete [] cguid;
    return 0;
  }
  for (int i = 0; i < 8; i++)
    uuid[i+8] = guid->data3[i];
  uuid_unparse(uuid, cguid);
#endif  // UNIX
#ifdef WINDOWS
  GUID *uuid = new GUID;
  uuid->Data1 = guid->data0;
  uuid->Data2 = guid->data1;
  uuid->Data3 = guid->data2;
  memcpy(uuid->Data4, guid->data3, 8); 
  char **ccguid = new char*;
  ccguid = &cguid;
  if (UuidToString(uuid, (unsigned char**)ccguid) != RPC_S_OK) {
    delete [] cguid;
    delete uuid;
    return 0;
  }
#endif  // WINDOWS
  return cguid;
}
