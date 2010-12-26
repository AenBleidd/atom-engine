#include "crc32.h"
#include "gamefs.h"
// Function hashing crc32
uint32_t GenCRC32(uint8_t *buf, uint64_t lenght, uint32_t crc) {
  if (crc == 0)
    crc = mask;
  while (lenght--)
    crc = crc32table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
  return crc ^ mask;
}
