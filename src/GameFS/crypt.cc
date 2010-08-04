#include "gamefs.h"
// Table generation
void AtomFS::GenKey(unsigned int k0, unsigned int k1,
                    unsigned int k2, unsigned int k3) {
  long x, z;
  int p;
  static long tt[10]= {
                       0x726a8f3bL,  // table
                       0xe69a3b5cL,
                       0xd3c71fe5L,
                       0xab3c73d2L,
                       0x4d3a8eb3L,
                       0x0396d6e8L,
                       0x3d4c2f7aL,
                       0x9ee27cf3L, };
  wake_table[0] = k0;
  wake_table[1] = k1;
  wake_table[2] = k2;
  wake_table[3] = k3;
  for (p = 4; p < 256; p++) {
    x = wake_table[p-4] + wake_table[p-1];  // fill t
    wake_table[p] = (x>>3) ^ tt[(unsigned char)(x&7)];
  }
  for (p = 0; p < 23; p++)
    wake_table[p] += wake_table[p+89];  // mix first entries
  x = wake_table[33];
  z = wake_table[59] | 0x01000001L;
  z = z&0xff7fffffL;
  for (p = 0; p < 256; p++) {  // change top byte to
    x = (x&0xff7fffffL) + z;  // a permutation etc
    wake_table[p] = (wake_table[p] & 0x00ffffffL) ^ x;
  }
  wake_table[256] = wake_table[0];
  unsigned char y = (unsigned char)(x);
  for (p = 0; p < 256; p++) {  // further change perm.
// and other digits
    wake_table[p] = wake_table[y = (unsigned char)(wake_table[p^y]^y)];
    wake_table[y] = wake_table[p+1];
  }
}
#ifdef _FSMAN_
void AtomFS::Crypt(unsigned int *data, int lenght,
                   unsigned int k[4], unsigned int r[4], unsigned int *t) {
  int d;
  unsigned int r1, r2, r3, r4, r5, r6, *e, m = 0x00ffffff;
  r3 = k[0], r4 = k[1], r5 = k[2], r6 = k[3];
  if (lenght < 0)
    d = -1;
  else
    d = 1;
  e = data + lenght;
  while (data-e) {
    r1 = *data;
    r2 = r1^r6;
    *data = r2;  // Change into r1 for decoding.
    data += d;
    r3 = r3 + r2;
    r3 = (r3>>8&m)^wake_table[r3&255];
    r4 = r4+r3;
    r4 = (r4>>8&m)^wake_table[r4&255];
    r5 = r5+r4;
    r5 = (r5>>8&m)^wake_table[r5&255];
    r6 = r6+r5;
    r6 = (r6>>8&m)^wake_table[r6&255];
  }
  r[0] = r3, r[1] = r4, r[2] = r5, r[3] = r6;
}
#endif  // _FSMAN_
void AtomFS::Decrypt(unsigned int *data, int lenght, unsigned int k[4],
                     unsigned int r[4], unsigned int *t) {
  int d;
  unsigned int r1, r2, r3, r4, r5, r6, *e, m = 0x00ffffff;
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
    r3 = (r3>>8&m)^wake_table[r3&255];
    r4 = r4+r3;
    r4 = (r4>>8&m)^wake_table[r4&255];
    r5 = r5+r4;
    r5 = (r5>>8&m)^wake_table[r5&255];
    r6 = r6+r5;
    r6 = (r6>>8&m)^wake_table[r6&255];
  }
    r[0] = r3, r[1] = r4, r[2] = r5, r[3] = r6;
}

