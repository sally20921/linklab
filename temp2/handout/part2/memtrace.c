//------------------------------------------------------------------------------
//
// memtrace
//
// trace calls to the dynamic memory manager
//
#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memlog.h>
#include <memlist.h>
#include "callinfo.h"

//
// function pointers to stdlib's memory management functions
//
static void *(*mallocp)(size_t size) = NULL;
static void (*freep)(void *ptr) = NULL;
static void *(*callocp)(size_t nmemb, size_t size);
static void *(*reallocp)(void *ptr, size_t size);

//
// statistics & other global variables
//
static unsigned long n_malloc  = 0;
static unsigned long n_calloc  = 0;
static unsigned long n_realloc = 0;
static unsigned long n_allocb  = 0;
static unsigned long n_freeb   = 0;
static item *list = NULL;

//
// init - this function is called once when the shared library is loaded
//
__attribute__((constructor))
void init(void)
{
  char *error;

  LOG_START();

  if (!mallocp) {
   mallocp = dlsym(RTLD_NEXT, "malloc");
   if ((error = dlerror()) != NULL ) {
     fputs(error, stderr);
     exit(1);
   }
  }

 if (!freep) {
   freep = dlsym(RTLD_NEXT, "free");
   if ((error = dlerror()) != NULL ) {
     fputs(error, stderr);
     exit(1);
   }
  }

 if (!callocp) {
   callocp = dlsym(RTLD_NEXT, "calloc");
   if ((error = dlerror()) != NULL ) {
     fputs(error, stderr);
     exit(1);
   }
  }

if (!reallocp) {
   reallocp = dlsym(RTLD_NEXT, "realloc");
   if ((error = dlerror()) != NULL ) {
     fputs(error, stderr);
     exit(1);
   }
  }
  // initialize a new list to keep track of all memory (de-)allocations
  // (not needed for part 1)
  list = new_list();

  // ...
}

void *malloc(size_t size) {
  void *ptr = mallocp(size);
  LOG_MALLOC(size, ptr);

  alloc(list,ptr,size);
  n_malloc++;
  n_allocb+=size;
  return ptr;
}

void free(void *ptr) {
  item* free_item = find(list, ptr);

  /*if (free_item == NULL) {//abnormal realloc
    LOG_ILL_FREE();
    return; //need to do double_free, illegal_free log
  }
  else if (free_item->flag == 0) {
    LOG_DOUBLE_FREE();
    return;
  }*/

  freep(ptr);
  LOG_FREE(ptr);

  dealloc(list, ptr);
  n_freeb += free_item->size;


}

void *calloc(size_t nmemb, size_t size) {
  size_t real_size = nmemb*size;
  void *ptr = callocp(nmemb, size);
  LOG_CALLOC(nmemb, size, ptr);

  alloc(list, ptr, real_size);

  n_calloc++;
  n_allocb += real_size;
  return ptr;
}

void *realloc(void *_ptr, size_t size) {
  void* ptr = reallocp(_ptr, size);
  LOG_REALLOC(_ptr, size, ptr);

  n_realloc++;
  n_allocb += size;

  item* old_item = find(list, _ptr);
  if (old_item == NULL) {//abnormal realloc
    LOG_ILL_FREE();
    return reallocp(NULL, size); //need to do double_free, illegal_free log
  }
  else if (old_item->flag == 0) {
    LOG_DOUBLE_FREE();
    return reallocp(NULL, size);
  }
   //normal alloc
  size_t old_size = old_item->size;
  alloc(list, _ptr, size);

  if (old_size >= size) {
     n_freeb = old_size-size;
  }

  return ptr;
}


//
// fini - this function is called once when the shared library is unloaded
//
__attribute__((destructor))
void fini(void)
{
  // ...
  unsigned long avg = n_allocb/(n_malloc+n_calloc+n_realloc);
  LOG_STATISTICS(n_allocb,avg, n_freeb);

 // dump_list(list);
  LOG_NONFREED_START();
  dump_list(list);
  LOG_STOP();

  // free list (not needed for part 1)
  free_list(list);
}

// ...
