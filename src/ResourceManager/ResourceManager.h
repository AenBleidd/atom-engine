#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_
#include "../AtomError/AtomError.h"
#include "../GameFS/gamefs.h"
#include "../preproc.h"

class ResourceManager {
public:
  explicit ResourceManager(AtomLog *log);
  ~ResourceManager();
};

#endif  // _RESOURCE_MANAGER_H_