#ifndef _ATOM_ENGINE_H_
#define _ATOM_ENGINE_H_

#include "OAtomEngine.h"

#include <unordered_set>
#include <queue>

typedef std::unordered_multiset<MODULE_TYPE, OAtomEngine*> MODULE_PARAM;

struct MESSAGE_QUEUE {
  OAtomEngine *recipient;
  MODULE_TYPE mod_type;
  MESSAGE_TYPE msg_type;
  uint32_t message;
  MESSAGE_PRIORITY prior;
  uint32_t vl_count;
  va_list vl;
};

class AtomEngine : public OAtomEngine {
  public:
    AtomEngine();
    ~AtomEngine();
    int32_t SendMessage(OAtomEngine *recipient, MODULE_TYPE mod_type, MESSAGE_TYPE msg_type, uint32_t message, MESSAGE_PRIORITY prior, uint32_t vl_count, va_list vl);
    int32_t GetMessage(OAtomEngine *sender, uint32_t message, MESSAGE_PRIORITY prior, uint32_t vl_count, va_list vl);
    int32_t LoadModule (char *name);
  private:
    MODULE_PARAM modules; // loaded modules and their types
    std::priority_queue<MESSAGE_QUEUE> msg_queue;

};

#endif  // _ATOM_ENGINE_H_
