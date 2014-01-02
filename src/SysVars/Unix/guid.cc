#include "../guid.h"
AGUID* GetAGUID(char *guid) {
  AGUID *aguid = new AGUID;
  aguid->data0 = 0;
  aguid->data1 = 0;
  aguid->data2 = 0;
  aguid->data3 = 0;
  uuid_t uuid;
  if (guid == 0)
    uuid_generate(uuid);  // we will generate new GUID
  else if (uuid_parse(guid, uuid) == -1) {
    delete aguid;
    return 0;
  }
// set this guid to our struct
// Long but fast.... I guess... I hope!...
  aguid->data0 = uuid[3] + uuid[2] * 256 + uuid[1] * 256 * 256 +
    uuid[0] * 256 * 256 * 256;
  aguid->data1 = uuid[7] + uuid[6] * 256 + uuid[5] * 256 * 256 +
    uuid[4] * 256 * 256 * 256;
  aguid->data2 = uuid[11] + uuid[10] * 256 + uuid[9] * 256 * 256 +
    uuid[8] * 256 * 256 * 256;
  aguid->data3 = uuid[15] + uuid[14] * 256 + uuid[13] * 256 * 256 +
    uuid[12] * 256 * 256 * 256;
// Convert to system byteorder
  aguid->data0 = BoLE2Sys(aguid->data0);
  aguid->data1 = BoLE2Sys(aguid->data1);
  aguid->data2 = BoLE2Sys(aguid->data2);
  aguid->data3 = BoLE2Sys(aguid->data3);
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
  uuid_t uuid;
// convert from system byteorder
  guid->data0 = BoSys2LE(guid->data0);
  guid->data1 = BoSys2LE(guid->data1);
  guid->data2 = BoSys2LE(guid->data2);
  guid->data3 = BoSys2LE(guid->data3);

  uuid[0] = HIBYTE(HIWORD(guid->data0));
  uuid[1] = LOBYTE(HIWORD(guid->data0));
  uuid[2] = HIBYTE(LOWORD(guid->data0));
  uuid[3] = LOBYTE(LOWORD(guid->data0));
  uuid[4] = HIBYTE(HIWORD(guid->data1));
  uuid[5] = LOBYTE(HIWORD(guid->data1));
  uuid[6] = HIBYTE(LOWORD(guid->data1));
  uuid[7] = LOBYTE(LOWORD(guid->data1));
  uuid[8] = HIBYTE(HIWORD(guid->data2));
  uuid[9] = LOBYTE(HIWORD(guid->data2));
  uuid[10] = HIBYTE(LOWORD(guid->data2));
  uuid[11] = LOBYTE(LOWORD(guid->data2));
  uuid[12] = HIBYTE(HIWORD(guid->data3));
  uuid[13] = LOBYTE(HIWORD(guid->data3));
  uuid[14] = HIBYTE(LOWORD(guid->data3));
  uuid[15] = LOBYTE(LOWORD(guid->data3));
// restore system byteorder
  guid->data0 = BoLE2Sys(guid->data0);
  guid->data1 = BoLE2Sys(guid->data1);
  guid->data2 = BoLE2Sys(guid->data2);
  guid->data3 = BoLE2Sys(guid->data3);

  uuid_unparse(uuid, (char*)cguid);
  return cguid;
}
