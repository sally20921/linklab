#ifndef __CALLINFO_H__
#define __CALLINFO_H__

#include <stdlib.h>

//
// return the PC of the callsite to the dynamic memory management function
//
//   fname      pointer to character array to hold function name
//   fnlen      length of character array
//   ofs        pointer to offset to hold PC offset into function
//
// returns
//    0         on success
//   <0         on error
//
int get_callinfo(char *fname, size_t fnlen, unsigned long long *ofs);

#endif
