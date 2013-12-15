#include "ResourceManager.h"
ResourceManager::ResourceManager(AtomLog *log, RESMAN_PARAM *param) {
  if (log == 0)
    throw -1;
// allock base memory
  if (param != 0 && param->start_mem_size != 0)
    MemBuff = malloc(param->start_mem_size);
  else
    MemBuff = malloc(start_mem_block);
  return;
  
}
ResourceManager::~ResourceManager() {
  if (MemBuff != 0)
    free(MemBuff);
  return;
}