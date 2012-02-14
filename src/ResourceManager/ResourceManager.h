#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_
#include "../AtomError/AtomError.h"
#include "../GameFS/gamefs.h"
#include "../preproc.h"

const size_t start_mem_block = 128*1024*1024;

// Structure with parameters
struct RESMAN_PARAM {
// size of memory which will be allocated at start up
  size_t start_mem_size;
};

class ResourceManager {
public:
  explicit ResourceManager(AtomLog *log, RESMAN_PARAM *param = 0);
  ~ResourceManager();
  void* Allocate(size_t size);
  void* Reallocate(void* mem, size_t size);
  void Release(void* mem);
private:
  void *MemBuff;
};

#endif  // _RESOURCE_MANAGER_H_