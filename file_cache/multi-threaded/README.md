### File Caching

#### testdriver.c
    Assertions plus driver to test file_cache.c that implements the file cache.

#### file_cache.c
    File cache implementation.

#### file_cache.h
    Given interface that needs to be implemented.

#### file_cache_driver.h
    Header file containing declarations for file_cache.c and the driver program.

#### Assumptions
 * a) When a cache is destroyed, even if there are pins on files, they are ignored and dirty files are written to disk.
 * b) If there are multiple calls to write to the file_memory, when the file is unpinned by one call, it will be written to disk only if the number of pins on the file are 0.
 * c) file_cache_destroy will flush all dirty buffers to disk whether or not there are other pins to files.
 * d) FILESIZE is ALWAYS 10KB, whether it is a user supplied file or if it is a file that is created by filling 0s.
 * e) to unpin a file is to reduce the number of pin count inside the file structrue (please see header file), a file is *unpinned* if the pin count on the file is 0.
 * f) the client application is responsible for ensuring it does not lock all files in the cache for writing and thus making the files not usable by other threads.
 * g) multiple threads may read/write from/to a file and as per the given problem statement, the program *DOES NOT* need to handle this case
 * h) destroy can ONLY be called by one thread, since the interface takes the argument file_cache *, but not the actual address of file_cache (file_cache ** type) which can be used to set file_cache * to NULL across all threads

#### Improvements if there was more time
 * a) Further optimize mutex_lock and mutex_unlock on cache (i.e., reduce code block size between the lock & unlock operations) that can boost cache performance.
 * b) Handle destroy called on same file_cache* by multiple threads. That is, point to the address of file_cache* and set to NULL. Once a destroy is called the second time, check if the file_cache* is NULL. If yes, then just return. The variable file_cache->actual_size can be used to check active cached file entries before doing a destroy.
 * c) Instead of freeing memory held by a previous file entry in the cache and mallocing it again for a new entry, we can overwrite the old memory and save us the trouble of doing a free + malloc operation again.
 * d) Instead of doing a sleep() waiting for resource, we can use additional mutexes to sync threads. This will save processor time. 
 * e) Better handle file read/write operations.
