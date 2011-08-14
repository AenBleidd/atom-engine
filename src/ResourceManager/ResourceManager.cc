#include "ResourceManager.h"
ResourceManager::ResourceManager(AtomLog *log, AtomFS *fs) {
  if (log == 0)
    throw -1;
  if (fs == 0) {
// TODO(Lawliet): We need to add here some const password
    try { fs = new AtomFS(log); }
// TODO(Lawliet): When we will catch this exceptions???
    catch (int32_t i) { throw -1; }
  }
  return;
}
ResourceManager::~ResourceManager() {
  return;
}