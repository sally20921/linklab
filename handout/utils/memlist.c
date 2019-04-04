#define _GNU_SOURCE

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "callinfo.h"
#include "memlist.h"

//
// function pointers to stdlib's malloc/free implementation
//
static void *(*callocp)(size_t nelem, size_t size) = NULL;
static void (*freep)(void *ptr) = NULL;

item *new_list(void)
{
  // since we are tracing memory (de-)allocations we cannot use
  // calloc/free directly. We need to make sure that we call the
  // implementations in stdlib in order not to have them caught 
  // by our own tracer library.
  char *error;

  dlerror();

  callocp = dlsym(RTLD_NEXT, "calloc");
  if (((error = dlerror()) != NULL) || (callocp == NULL)) {
    fprintf(stderr, "Error getting symbol 'calloc': %s\n", error);
    exit(EXIT_FAILURE);
  }

  freep = dlsym(RTLD_NEXT, "free");
  if (((error = dlerror()) != NULL) || (freep == NULL)) {
    fprintf(stderr, "Error getting symbol 'free': %s\n", error);
    exit(EXIT_FAILURE);
  }

  // create new list
  return (item*)callocp(1, sizeof(item));
}

void free_list(item *list)
{
  item *next;

  while (list) {
    next = list->next;
    freep(list);
    list = next;
  }
}

item *alloc(item *list, void *ptr, size_t size)
{
  item *prev, *cur, *i;
  int res;

  if (list == NULL) return NULL;

  i = (item*)callocp(1, sizeof(item));
  i->ptr = ptr;
  i->size = size;
  i->cnt = 1;
  if (get_callinfo(&i->fname[0], sizeof(i->fname), &i->ofs) < 0) {
    strncpy(i->fname, "???", sizeof(i->fname));
    i->ofs = 0;
  }

  // check if block already exists
  prev = list; cur = list->next;
  while ((cur != NULL) && (cur->ptr < i->ptr)) {
    prev = cur; cur = cur->next;
  }

  if (cur == NULL) {
    // new block -> insert into list
    prev->next = i;
  } else {
    if (cur->ptr == i->ptr) {
      // existing block -> update size & reference counter
      cur->size = i->size;
      cur->cnt++;
      freep(i);
      i = cur;
    } else {
      // new block -> insert into list
      i->next = cur;
      prev->next = i;
    }
  }

  return i;
}

item *dealloc(item *list, void *ptr)
{
  item *prev, *cur, *i;

  if (list == NULL) return NULL;

  // find block
  prev = list; cur = list->next;
  while ((cur != NULL) && (cur->ptr != ptr)) {
    prev = cur; cur = cur->next;
  }

  // decrement reference count if found
  if (cur != NULL) cur->cnt--;

  return cur;
}

item *find(item *list, void *ptr)
{
  if (list == NULL) return NULL;

  list = list->next;
  while ((list != NULL) && (ptr > list->ptr)) {
    list = list->next;
  }

  if ((list != NULL) && (list->ptr == ptr)) return list;
  else return NULL;
}

void dump_list(item *list)
{
  assert(list != NULL);
  item *i = list->next;

  printf("  %-16s   %-8s   %-3s   %-16s\n",
      "block", "size", "cnt", "caller");
  while (i != NULL) {
    printf("  %-16p   %-8zd   %-3d   %s:%llx\n",
        i->ptr, i->size, i->cnt, i->fname, i->ofs);
    i = i->next;
  }
}
