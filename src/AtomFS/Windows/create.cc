#include <AtomFS/atomfs.h>
bool Create(char **input, uint64_t count, char *filename) {
  AtomFSHeader *header = new AtomFSHeader(0, magic, version, 0);
  uint64_t filetableAddress = 15; // size of the header
  return true;
}