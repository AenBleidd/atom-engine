#include "AtomEngine.h"

AtomEngine::AtomEngine() {
}

AtomEngine::~AtomEngine() {
}

int32_t AtomEngine::SendMessage(OAtomEngine *recipient, MODULE_TYPE mod_type, MESSAGE_TYPE msg_type, uint32_t message, MESSAGE_PRIORITY prior, uint32_t vl_count, va_list vl) {
  return 0;
}

int32_t AtomEngine::GetMessage(OAtomEngine *sender, uint32_t message, MESSAGE_PRIORITY prior, uint32_t vl_count, va_list vl) {
// no need to call this function
  return -1;
}

int32_t AtomEngine::LoadModule (char *name) {
  return 0;
}
