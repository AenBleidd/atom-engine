#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_
#include "../AtomError/AtomError.h"
#include "../GameFS/gamefs.h"
#include "../preproc.h"

class ResourceManager {
public:
  explicit ResourceManager(AtomLog *log);
  ~ResourceManager();
  void* Allocate(uint64_t size);
  void* Reallocate(void* mem, uint64_t size);
  void Release(void* mem);
};

#endif  // _RESOURCE_MANAGER_H_