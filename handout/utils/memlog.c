#include <stdarg.h>
#include <stdio.h>

#include "callinfo.h"

int mlog(int pc, const char *fmt, ...)
{
  static unsigned int id = 1;
  va_list ap;
  int res;

  res = fprintf(stderr, "[%04u] ", id++);

  if (pc) {
    char buf[16];
    unsigned long long ofs;
    if (get_callinfo(&buf[0], sizeof(buf), &ofs) != -1) {
      res += fprintf(stderr, "%12s:%-3llx: ", buf, ofs);
    } else {
      res += fprintf(stderr, "%5c%10p : ", ' ', NULL);
    }
  }

  va_start(ap, fmt);
  res += vfprintf(stderr, fmt, ap);
  va_end(ap);

  fprintf(stderr, "\n");

  return res;
}
