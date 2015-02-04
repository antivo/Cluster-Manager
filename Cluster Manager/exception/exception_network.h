#ifndef EXCEPTION_NETWORK
#define EXCEPTION_NETWORK

#include "exception_common.h"

namespace exception {
  // Create network exception that cathches last network error
  Exception make_network_exception();
}

#endif