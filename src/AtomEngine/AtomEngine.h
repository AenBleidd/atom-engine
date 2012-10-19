#ifndef _ATOM_ENGINE_H_
#define _ATOM_ENGINE_H_

#include "OAtomEngine.h"

#include <unordered_set>
#include <queue>
#include <vector>

typedef std::unordered_multiset<MODULE_TYPE, OAtomEngine*> MODULE_PARAM;

class MESSAGE_QUEUE {
public:
  OAtomEngine *recipient;
  MODULE_TYPE mod_type;
  MESSAGE_TYPE msg_type;
  uint32_t message;
  unsigned char prior;
  uint32_t vl_count;
  va_list vl;
  MESSAGE_QUEUE (OAtomEngine *i_recipient, MODULE_TYPE i_mod_type, 
                MESSAGE_TYPE i_msg_type, uint32_t i_message, 
                unsigned char i_prior, uint32_t i_vl_count, va_list i_vl) {
    recipient = i_recipient;
    mod_type = i_mod_type;
    msg_type = i_msg_type;
    message = i_message;
    prior = i_prior;
    vl_count = i_vl_count;
    vl = i_vl;
  }

  bool operator < (const MESSAGE_QUEUE &y) const {
    return prior > y.prior;
  }
};

class AtomEngine : public OAtomEngine {
  public:
    AtomEngine();
    ~AtomEngine();
    int32_t SendMessage(OAtomEngine *recipient, MODULE_TYPE mod_type, 
                        MESSAGE_TYPE msg_type, uint32_t message, 
                        unsigned char prior, uint32_t vl_count, va_list vl);
    int32_t GetMessage(OAtomEngine *sender, uint32_t message, 
                       unsigned char prior, uint32_t vl_count, va_list vl);
    int32_t LoadModule (char *name);
  private:
    MODULE_PARAM modules; // loaded modules and their types
    std::priority_queue<MESSAGE_QUEUE, std::vector<MESSAGE_QUEUE>> msg_queue;

};

#endif  // _ATOM_ENGINE_H_
