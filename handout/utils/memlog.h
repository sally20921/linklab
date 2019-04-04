#ifndef __MEMLOG_H__
#define __MEMLOG_H__

#include <stdarg.h>
#include <stdio.h>

//
// log a call to one of the dynamic memory management functions to stderr
//
//   res        result pointer (if any)
//
//   nmemb,
//   size,      correspond to the parameters of the respective function call
//   ptr
//
// returns the number of characters printed
//

#define LOG_MALLOC(size, res)         mlog(1, "malloc( %zu ) = %p", size, res)
#define LOG_CALLOC(nmemb, size, res)  mlog(1, "calloc( %zu , %zu ) = %p", nmemb, size, res)
#define LOG_REALLOC(ptr, size, res)   mlog(1, "realloc( %p , %zu ) = %p", ptr, size, res)
#define LOG_FREE(ptr)                 mlog(1, "free( %p )", ptr)


//
// log statistics
//
#define LOG_STATISTICS(alloc_total, alloc_avg, free_total) \
  { mlog(0, ""); \
    mlog(0, "Statistics"); \
    mlog(0, "  allocated_total      %lu", alloc_total); \
    mlog(0, "  allocated_avg        %lu", alloc_avg); \
    mlog(0, "  freed_total          %lu", free_total); \
  }

//
// log statistics about memory blocks
//
#define LOG_NONFREED_START() \
  { mlog(0, ""); \
    mlog(0, "Non-deallocated memory blocks"); \
    mlog(0, "  %-16s   %-8s   %-7s   %-16s", "block", "size", "ref cnt", "caller"); \
  }
#define LOG_BLOCK(ptr, size, cnt, fn, ofs) mlog(0, "  %-16p   %-8zd   %-7d   %s:%llx", ptr, size, cnt, fn, ofs)

//
// log invalid deallocation requests
//
#define LOG_DOUBLE_FREE()             mlog(0, "%10c  *** DOUBLE_FREE  *** (ignoring)", ' ')
#define LOG_ILL_FREE()                mlog(0, "%10c  *** ILLEGAL_FREE *** (ignoring)", ' ')

//
// log start/end messages
//
#define LOG_START()                   mlog(0, "Memory tracer started.")
#define LOG_STOP() \
  { mlog(0, ""); \
    mlog(0, "Memory tracer stopped."); \
  }

//
// do not use this directly. Invoke through one of the macros above
//
int mlog(int pc, const char *fmt, ...);

#endif
