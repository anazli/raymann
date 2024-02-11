#pragma once

#include <cassert>
#include <cstdio>

#define APP_MSG(msg) printf("%s\n", msg);

#define APP_ASSERT(x, msg)      \
  if (!(x)) {                   \
    printf("ERROR: %s\n", msg); \
    abort();                    \
  }
