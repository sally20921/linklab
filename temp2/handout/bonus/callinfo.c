#include "callinfo.h"

int get_callinfo(char *fname, size_t fnlen, unsigned long long *ofs)
{
  unw_context_t context;
  unw_cursor_t cursor;
  unw_word_t off;

/* int unw_get_proc_name(unw_cursor_t *cp, char *bufp, size_t len, unw_word_t *offp);
   -get the name of current procedure
   -returns the name of the procedure that created the stack frame identified by argument cp*/

/*int unw_getcontext(unw_context_t *ucp);
  int unw_init_local(unw_cursor_t *c, unw_cotext_t *ctxt);
  int unw_step(unw_cursor_t *cp); */

 unw_getcontext(&context);
 unw_init_local(&cursor, &context);

 if(unw_step(&cursor) < 0) {
   return -1;
 };
 if( unw_step(&cursor) < 0) {
   return -1;
 };
 if( unw_step(&cursor) < 0) {
   return -1;
 };
 if( unw_step(&cursor) < 0) {
   return -1;
 };


 char function_name[fnlen];
 unw_get_proc_name(&cursor,function_name, fnlen, &off);
// printf("%s", function_name);

/* char key[] = "__libc_start_main";
 int i = 0;
 while (i <fnlen) {
  if (function_name[i] != key[i]) {
    break;
  }
  i++;
 }
 
 if (i == fnlen-1) {
    *fname++ = 'm';
    *fname++ = 'a';
    *fname++ = 'i';
    *fname++ = 'n';
 }
 else {
    i = 0;
    while (function_name!='\0' && i < fnlen) {
       *fname++ = *(function_name+i);
       i++;
    }
 }*/
 *fname = function_name[0];
 *ofs = (unsigned long long) off;
 

 return 0;


}
