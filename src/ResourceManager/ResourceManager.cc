#include "ResourceManager.h"
ResourceManager::ResourceManager(AtomLog *log) {
  if (log == 0)
    throw -1;
  return;
}
ResourceManager::~ResourceManager() {
  return;
}