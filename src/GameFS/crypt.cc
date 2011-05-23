#include "gamefs.h"
// Table generation
uint32_t* AtomFS::GenKey(uint32_t k0, uint32_t k1,
                    uint32_t k2, uint32_t k3) {
  int64_t x, z;
  int32_t p;
  uint32_t *table = new uint32_t[257];
  static int64_t tt[10] = {
                       0x726a8f3bL,  // table
                       0xe69a3b5cL,
                       0xd3c71fe5L,
                       0xab3c73d2L,
                       0x4d3a8eb3L,
                       0x0396d6e8L,
                       0x3d4c2f7aL,
                       0x9ee27cf3L, };
  table[0] = k0;
  table[1] = k1;
  table[2] = k2;
  table[3] = k3;
  for (p = 4; p < 256; p++) {
    x = table[p-4] + table[p-1];  // fill t
    table[p] = (x>>3) ^ tt[(uint8_t)(x&7)];
  }
  for (p = 0; p < 23; p++)
    table[p] += table[p+89];  // mix first entries
  x = table[33];
  z = table[59] | 0x01000001L;
  z = z&0xff7fffffL;
  for (p = 0; p < 256; p++) {  // change top byte to
    x = (x&0xff7fffffL) + z;  // a permutation etc
    table[p] = (table[p] & 0x00ffffffL) ^ x;
  }
  table[256] = table[0];
  uint8_t y = (uint8_t)(x);
  for (p = 0; p < 256; p++) {  // further change perm.
// and other digits
    table[p] = table[y = (uint8_t)(table[p^y]^y)];
    table[y] = table[p+1];
  }
  return table;
}
#ifdef _FSMAN_
void AtomFS::Crypt(uint32_t *data, int32_t lenght,
                   uint32_t k[4], uint32_t r[4], uint32_t *t) {
  int32_t d;
  uint32_t r1, r2, r3, r4, r5, r6, *e, m = 0x00ffffff;
  r3 = k[0], r4 = k[1], r5 = k[2], r6 = k[3];
  if (lenght < 0)
    d = -1;
  else
    d = 1;
  e = data + lenght;
  while (data-e) {
    r1 = *data;
    r2 = r1^r6;
    *data = r2;
    data += d;
    r3 = r3 + r2;
    r3 = (r3>>8&m)^t[r3&255];
    r4 = r4+r3;
    r4 = (r4>>8&m)^t[r4&255];
    r5 = r5+r4;
    r5 = (r5>>8&m)^t[r5&255];
    r6 = r6+r5;
    r6 = (r6>>8&m)^t[r6&255];
  }
  r[0] = r3, r[1] = r4, r[2] = r5, r[3] = r6;
}
#endif  // _FSMAN_
void AtomFS::Decrypt(uint32_t *data, int32_t lenght, uint32_t k[4],
                     uint32_t r[4], uint32_t *t) {
  int32_t d;
  uint32_t r1, r2, r3, r4, r5, r6, *e, m = 0x00ffffff;
  r3 = k[0], r4 = k[1], r5 = k[2], r6 = k[3];
  if (lenght < 0)
    d = -1;
  else
    d = 1;
  e = data + lenght;
  while (data-e) {
    r1 = *data;
    r2 = r1^r6;
    *data = r2;
    data += d;
    r3 = r3+r1;
    r3 = (r3>>8&m)^t[r3&255];
    r4 = r4+r3;
    r4 = (r4>>8&m)^t[r4&255];
    r5 = r5+r4;
    r5 = (r5>>8&m)^t[r5&255];
    r6 = r6+r5;
    r6 = (r6>>8&m)^t[r6&255];
  }
    r[0] = r3, r[1] = r4, r[2] = r5, r[3] = r6;
}
// add new addon key
uint32_t AtomFS::AddAddonKey(uint32_t *key) {
  uint32_t **temp, **table;
  if (addon_key.count != 0) {
// search fo an existing key
    for (uint32_t i = 0; i < addon_key.count; i++) {
      if (memcmp(key, addon_key.addon_key[i], 16) == 0) {
// we find this key
        delete [] key;
        return i;
      }
    }
// we didn't find this key, let's add it
  temp = new uint32_t * [addon_key.count + 1];
  table = new uint32_t * [addon_key.count + 1];
// store pointers in new array
  for (uint32_t i = 0; i < addon_key.count; i++) {
    temp[i] = addon_key.addon_key[i];
    table[i] = addon_key.addon_table[i];
  }
  temp[addon_key.count] = key;
  table[addon_key.count] = GenKey(key[0], key[1], key[2], key[3]);
// delete old array
  delete [] addon_key.addon_key;
  delete [] addon_key.addon_table;
// save new array
  addon_key.addon_key = temp;
  addon_key.addon_table = table;
// return position of the new key
  return addon_key.count++;
  } else {
// create new key
    addon_key.addon_key = new uint32_t*;
    addon_key.addon_table = new uint32_t*;
// set the key
    addon_key.addon_key[0] = key;
    addon_key.addon_table[0] = GenKey(key[0], key[1], key[2], key[3]);
    addon_key.count = 1;
// return key number
    return 0;
  }
}
