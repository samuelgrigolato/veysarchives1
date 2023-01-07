#include <stdio.h>
#include <stdarg.h>
#include "core/log.h"


void logInfo(const char* format, ...) {
  va_list args;
  va_start(args, format);
  printf("INFO: ");
  vprintf(format, args);
  printf("\n");
  va_end(args);
}

void logError(const char* format, ...) {
  va_list args;
  va_start(args, format);
  printf("ERROR: ");
  vprintf(format, args);
  printf("\n");
  va_end(args);
}
