#ifndef _OATOMENGINE_H_
#define _OATOMENGINE_H_

#include <preproc.h>

enum MESSAGE_TYPE {
  SEND_TO_SPECIFIC,
  SEND_TO_TYPE,
  SEND_TO_ALL
};

enum MODULE_TYPE {
  NON_TYPE_MODULE,
  LOG_MODULE,
  FS_MODULE,
  CRYPT_MODULE
};

enum MESSAGE_PRIORITY {
  LOW           = 255,
  BELOW_NORMAL  = 200,
  NORMAL        = 150,
  ABOVE_NORMAL  = 100,
  HIGH          = 50,
  CRITICAL      = 0
};

class OAtomEngine {
  public:
    virtual int32_t GetMessage(OAtomEngine *sender, uint32_t message, 
                               unsigned char prior, uint32_t vl_count, 
                               va_list vl) = 0;
};

#endif  // _OATOMENGINE_H_
