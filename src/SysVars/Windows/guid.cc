#include "../guid.h"
AGUID* GetAGUID(char *guid) {
  AGUID *aguid = new AGUID;
  aguid->data0 = 0;
  aguid->data1 = 0;
  aguid->data2 = 0;
  aguid->data3 = 0;
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
// Convert to system byteorder
  aguid->data0 = (uint32_t)uuid->Data1;
  uuid->Data2 = BoSys2LE(uuid->Data2);
  uuid->Data3 = BoSys2LE(uuid->Data3);
  aguid->data1 = uuid->Data3 + uuid->Data2 * 256 * 256;
  aguid->data2 = uuid->Data4[3] + uuid->Data4[2] * 256 + uuid->Data4[1] *
    256 * 256 + uuid->Data4[0] * 256 * 256 * 256;
  aguid->data3 = uuid->Data4[7] + uuid->Data4[6] * 256 + uuid->Data4[5] *
    256 * 256 + uuid->Data4[4] * 256 * 256 * 256;
  aguid->data1 = BoLE2Sys(aguid->data1);
  aguid->data2 = BoLE2Sys(aguid->data2);
  aguid->data3 = BoLE2Sys(aguid->data3);

  delete uuid;
  return aguid;
}
char* GetCGUID(AGUID *guid) {
  char *cguid = new char[37];
  if (guid == 0) {
    guid = new AGUID;
    guid = GetAGUID();
    if (guid == 0) {
      delete [] cguid;
      return 0;
    }
  }
  GUID *uuid = new GUID;
  uuid->Data1 = guid->data0;
// convert from system byteorder
  guid->data1 = BoSys2LE(guid->data1);
  guid->data2 = BoSys2LE(guid->data2);
  guid->data3 = BoSys2LE(guid->data3);

  uuid->Data2 = HIWORD(guid->data1);
  uuid->Data3 = LOWORD(guid->data1);
  uuid->Data4[0] = HIBYTE(HIWORD(guid->data2));
  uuid->Data4[1] = LOBYTE(HIWORD(guid->data2));
  uuid->Data4[2] = HIBYTE(LOWORD(guid->data2));
  uuid->Data4[3] = LOBYTE(LOWORD(guid->data2));
  uuid->Data4[4] = HIBYTE(HIWORD(guid->data3));
  uuid->Data4[5] = LOBYTE(HIWORD(guid->data3));
  uuid->Data4[6] = HIBYTE(LOWORD(guid->data3));
  uuid->Data4[7] = LOBYTE(LOWORD(guid->data3));
// restore system byteorder
  guid->data1 = BoLE2Sys(guid->data1);
  guid->data2 = BoLE2Sys(guid->data2);
  guid->data3 = BoLE2Sys(guid->data3);

  char **ccguid = new char*;
  ccguid = &cguid;
  if (UuidToString(uuid, (unsigned char**)ccguid) != RPC_S_OK) {
    delete [] cguid;
    delete uuid;
    return 0;
  }
  return cguid;
}
