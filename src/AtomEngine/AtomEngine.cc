#include "AtomEngine.h"

AtomEngine::AtomEngine() {
}

AtomEngine::~AtomEngine() {
}

int32_t AtomEngine::SendMessage(OAtomEngine *recipient, MODULE_TYPE mod_type, 
                                MESSAGE_TYPE msg_type, uint32_t message, 
                                unsigned char prior, uint32_t vl_count, 
                                va_list vl) {
  MESSAGE_QUEUE mq(recipient, mod_type, msg_type, message, prior, vl_count, vl);
  msg_queue.push(mq);
  return 0;
}

int32_t AtomEngine::GetMessage(OAtomEngine *sender, uint32_t message, 
                               unsigned char prior, uint32_t vl_count, va_list vl) {
// no need to call this function
#ifdef ATOM_DEBUG
// we can call this function in debug mode when we want to check queue 
// and only here we return priority!
  if (!msg_queue.empty()) {
    MESSAGE_QUEUE mq = msg_queue.top();
    msg_queue.pop();
    return (int32_t)mq.prior;
  } else {
    return 0;
  }
#else
#error Deprecated call of the function AtomEngine::GetMessage()
  return -1;
#endif  // ATOM_DEBUG
}

int32_t AtomEngine::LoadModule (char *name) {
  return 0;
}
