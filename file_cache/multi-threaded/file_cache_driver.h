#ifndef _DRIVER_CACHE_H_
#define _DRIVER_CACHE_H_

#include <stdbool.h>
#include <pthread.h>

#define FILESIZE 10 * 1024 // 10KB
typedef struct _file_ {
  int pins; // number of pins on the file
  const char *file_name; // file name
  bool dirty; // dirty flag on the file
  char *content; // content of the file
} file;

typedef struct file_cache_t {
  int max_size; // max_size of the cache, set in constructor
  int actual_size; // actual entries in cache. Useful to check entries in cache while dealing with multiple threads and prevent cache_destroy from multiple threads at the same time (NOT CURRENTLY USED).
  file **file_struct; // file structrues that exist in the cache
  pthread_mutex_t mutex;
} file_cache;

#include "file_cache.h"

void write_to_disk(file*);
void read_from_disk(file*);
file* find_in_cache(file_cache*, const char*);
int findspot_in_cache(file_cache*);


#endif  // _DRIVER_CACHE_H_
