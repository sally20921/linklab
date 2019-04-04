#ifndef __MEMLIST_H__
#define __MEMLIST_H__

#include <stddef.h>

//
// list element holding information about an allocated memory block
//
//   ptr        pointer to block
//   size       size of block
//   cnt        allocate count
//   fname      name of function allocating block
//   ofs        offset of instruction in function
//
//   next       pointer to next item in linked list
//
typedef struct __item {
  void *ptr;
  size_t size;
  int cnt;
  char fname[32];
  unsigned long long ofs;
  struct __item *next;
} item;


//
// initialize a new list.
// The first element is a dummy element
//
item *new_list(void);

//
// free a list
//
void free_list(item *list);

//
// add information about newly allocated block to list
//
//
//   list       pointer to list
//   ptr        pointer to newly allocated block
//   size       size of newly allocated block
//
// returns
//    item*     pointer to item holding information about the block
//
// the reference count is updated automatically. If the block is re-allocated
// (i.e., there already is an item to ptr) the size of the item is adjusted.
//
item *alloc(item *list, void *ptr, size_t size);

//
// update information on freed block
//
//    list      pointer to list
//    ptr       pointer to freed block
//
// returns
//    item*     pointer to item holding information about freed block
//
// the reference count is updated automatically
//
item *dealloc(item *list, void *ptr);

//
// find information about a block in list
//
//   list       pointer to list
//   ptr        pointer of block
//
// returns
//    item*     pointer to item holding information about the block
//
item *find(item *list, void *ptr);

//
// dump (print) the list in human-readable form to stdout
//
//   list       pointer to list
//
void dump_list(item *list);

#endif
