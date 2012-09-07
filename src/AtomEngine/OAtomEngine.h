#ifndef _OATOMENGINE_H_
#define _OATOMENGINE_H_

#include <preproc.h>

class OAtomEngine {
  public:
    virtual int32_t SendMessage(OAtomEngine *recipient, uint32_t message, va_list vl) = 0;
    virtual int32_t GetMessage(OAtomEngine *sender, uint32_t message, va_list vl) = 0;
};

#endif  // _OATOMENGINE_H_
