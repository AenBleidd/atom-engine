#ifndef _CRC32_H_
#define _CRC32_H_
#include <inttypes.h>
// Function hashing crc32
// doxor:
// 0 - do not do xor
// 1 - do xor
// 2 - do only xor
uint32_t GenCRC32(uint8_t *buf, uint64_t lenght, uint32_t crc = 0);
#endif
