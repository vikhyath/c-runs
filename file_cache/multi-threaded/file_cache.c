/*
 * Assumptions:
 * a) When a cache is destroyed, even if there are pins on files, they are ignored and dirty files are written to disk.
 * b) If there are multiple calls to write to the file_memory, when the file is unpinned by one call, it will be written to disk only if the number of pins on the file are 0.
 * c) file_cache_destroy will flush all dirty buffers to disk whether or not there are other pins to files.
 * d) FILESIZE is ALWAYS 10KB, whether it is a user supplied file or if it is a file that is created by filling 0s.
 * e) to unpin a file is to reduce the number of pin count inside the file structrue (please see header file), a file is *unpinned* if the pin count on the file is 0.
 * f) the client application is responsible for ensuring it does not lock all files in the cache for writing and thus making the files not usable by other threads.
 * g) multiple threads may read/write from/to a file and as per the given problem statement, the program *DOES NOT* need to handle this case
 * h) destroy can ONLY be called by one thread, since the interface takes the argument file_cache *, but not the actual address of file_cache (file_cache ** type) which is to be used to set file_cache * to NULL across all threads
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "file_cache_driver.h"

/*
 * Create the file cache of size max_cache_entries
 * @params integer max_cache_entries
 * @return file_cache type pointer
 */
file_cache* file_cache_construct(int max_cache_entries)
{
    // assign memory to the file_cache struct
    file_cache *fct = malloc(sizeof(*fct));
    fct->max_size = max_cache_entries;
    // init the cache mutex and set actual size to 0
    pthread_mutex_init(&(fct->mutex), NULL);
    pthread_mutex_lock(&(fct->mutex));
    fct->actual_size = 0;
    pthread_mutex_unlock(&(fct->mutex));
    size_t i = 0;
    // create sufficient file pointers in the cache for storing the files
    fct->file_struct = malloc(max_cache_entries * sizeof(file));
    while (i < fct->max_size) {
        // set to NULL by default
        (fct->file_struct)[i]= NULL;
        i++;
    }
    return fct;
}

/*
 * Write file to disk
 * @params target file
 * @return void
 */
void write_to_disk(file *target)
{
    FILE *fptr;
    fptr = fopen(target->file_name, "w");

    if (NULL == fptr) {
        printf("File Write Error\n!");
        exit(1);
    }
    fprintf(fptr, "%s", target->content);
    fclose(fptr);
}

/*
 * Read file from disk, create with 10KB zeros if needed
 * @params target file
 * @return void
 */
void read_from_disk(file *target)
{
    FILE *fptr;
    fptr = fopen(target->file_name, "r");
    if (NULL == fptr) {
        FILE *filewr;
        filewr = fopen(target->file_name, "w");
        if (NULL == filewr) {
            printf("File Write Error\n!");
            exit(1);
        }
        size_t i;
        for (i = 0; i < FILESIZE; i++) {
            fputc('0', filewr);
        }
        fclose(filewr);
        fptr = fopen(target->file_name, "r");
        if (NULL == fptr) {
            printf("Error reading from 0s initialized 10KB file\n");
            exit(1);
        }
    }
    target->content = malloc(sizeof(char) * FILESIZE);
    // read entire file, including the last \0
    fgets(target->content, FILESIZE + 1, fptr);
    fclose(fptr);
}

/*
 * Destroy the file cache. Free file contents, file_structure and the file_cache itself
 * @params file_cache type structure
 * @return void
 */
void file_cache_destroy(file_cache *fct)
{
    // make sure no other thread is in between doing something else
    pthread_mutex_lock(&(fct->mutex));
    size_t i;
    // for every file in cache, if it is dirty, write contents to disk
    for (i = 0; i < fct->max_size; i++) {
      if (NULL != fct->file_struct[i] && true == fct->file_struct[i]->dirty) {
          write_to_disk(fct->file_struct[i]);
          fct->file_struct[i]->dirty = false;
      }
      // if content is not NULL then free
      if (NULL != fct->file_struct[i] && NULL != fct->file_struct[i]->content) {
          free(fct->file_struct[i]->content);
      }
      // free file_struct
      free(fct->file_struct[i]);
    }
    fct->max_size = 0;
    // unlock mutex and free file_cache
    pthread_mutex_unlock(&(fct->mutex));
    free(fct);
    fct = NULL;
}

/*
 * File file in cache, if it exists
 * @params file_cache file_cache, file_name
 * @return file_struct if any or NULL
 */
file* find_in_cache(file_cache *fct, const char *name)
{
    size_t i = 0;
    // check if the file already exists in cache. If yes, then return
    while (i < fct->max_size) {
        if (NULL != fct->file_struct[i] && strcmp(name, fct->file_struct[i]->file_name) == 0) {
            return fct->file_struct[i];
        }
        i++;
    }
    return NULL;
}

/*
 * Find a suitable spot for a new insertion in the cache
 * @params file_cache file_cache
 * @return index in cache to insert OR -1 if no possibility
 */
int findspot_in_cache(file_cache *fct)
{
    // try to get the lock, BLOCKING call until space is found for the thread
    while (1) {
        int idx = 0;
        for (idx = 0; idx < fct->max_size; idx++) {
            // vacant spot
            if (NULL == fct->file_struct[idx]) {
                return idx;
            }

            // dirty or pins > 0, keep looking
            if (true == fct->file_struct[idx]->dirty || fct->file_struct[idx]->pins > 0) {
                continue;
            }

            // pins == 0, valid entry, return idx
            if (0 == fct->file_struct[idx]->pins) {
                return idx;
            }
        }

        // not found, so sleep after releasing lock
        pthread_mutex_unlock(&(fct->mutex));
        sleep(1);
        // take back lock and continue search
        pthread_mutex_lock(&(fct->mutex));
    }
}


/*
 * Pin given files of size num_files in the file_cache file_cache *fct
 * @params file_cache type file_cache, list of file names, number of files
 * @return void
 */
void file_cache_pin_files(file_cache *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then increment pin count
    //      else try to find a spot to fill the cache entry with file
    size_t idx;
    for (idx = 0; idx < num_files; idx++) {
        // check if file exists in cache, lock down to prevent duplicate additions
        pthread_mutex_lock(&(fct->mutex));
        file *loc = find_in_cache(fct, files[idx]);
        // if file already present, then inc #pins
        if (NULL != loc) {
            (loc->pins)++;
            pthread_mutex_unlock(&(fct->mutex));
            continue;
        }

        // else find a spot to fit the file
        int spot = findspot_in_cache(fct);

        // no spot found so continue to see if any other files have existing
        // pins that can be increased, else dont do anything
        if (spot == -1) {
            pthread_mutex_unlock(&(fct->mutex));
            continue;
        }

        // previously something exists, need to write to disk if dirty and clear
        if (NULL != fct->file_struct[spot]) {
            if (true == fct->file_struct[spot]->dirty) {
                write_to_disk(fct->file_struct[spot]);
            }

            // clean the entry in cache
            free(fct->file_struct[idx]->content);
            fct->file_struct[idx]->content = NULL;
            // decrement actual cache size
            fct->actual_size--;
            // free that entry
            free(fct->file_struct[idx]);
            fct->file_struct[idx] = NULL;
        }
        // Yep! this can be improved by simply overwriting the old memory
        // allocate space
        fct->file_struct[idx] = malloc(sizeof(file));

        // initialize name, pins, dirty flag and read contents
        fct->file_struct[idx]->file_name = files[idx];
        fct->file_struct[idx]->pins = 1;
        fct->file_struct[idx]->dirty = false;
        // read contents
        read_from_disk(fct->file_struct[idx]);
        // increment cache size
        fct->actual_size++;
        pthread_mutex_unlock(&(fct->mutex));
    }
}

/*
 * Unpin the given files list in the file cache
 * @params file_cache type file_cache, list of files to be pinned and its count num_files
 * @return void
 */
void file_cache_unpin_files(file_cache *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then
    //          a) decrease pin count
    //          b) write to disk if dirty and pin count is 0
    //      else undefined behaviour
    size_t idx;
    for (idx = 0; idx < num_files; idx++) {
        // lockdown to prevent duplicate removals
        pthread_mutex_lock(&(fct->mutex));
        // check if file exists in cache
        file *loc = find_in_cache(fct, files[idx]);
        // file found and it has been pinned before
        if (NULL != loc && loc->pins > 0) {
            (loc->pins)--;
            if (true == loc->dirty) {
                loc->dirty = false;
                if (0 == loc->pins) {
                    write_to_disk(loc);
                }
            }
            if (0 == loc->pins) {
                // also decrement the actual cached files count since this is technically a clean entry which can be reused for caching other files
                fct->actual_size--;
            }
        }
        pthread_mutex_unlock(&(fct->mutex));
    }
}

/*
 * Get read only memory for the specified file in the file cache
 * @params file_cache type file_cache, the file to read
 * @return return read only memory or NULL if the file does not exist
 */
const char *file_cache_file_data(file_cache *fct, const char *file_name)
{
    // Need a lock on cache because some other thread might try to clear the cache entry before we can even return the data
    pthread_mutex_lock(&(fct->mutex));
    file *loc = find_in_cache(fct, file_name);
    const char *data;
    // found file and it has been pinned
    if (NULL != loc && loc->pins > 0) {
        data = (const char*)loc->content;
    } else { // undefined behavior
        data = NULL;
    }
    pthread_mutex_unlock(&(fct->mutex));
    return data;
}

/*
 * Get a writable memory for the specified file in the file_cache
 * @params file_cache type file_cache, the file to write to
 * @return writable memory of NULL if the file does not exist
 */
char *file_cache_mutable_file_data(file_cache *fct, const char *file_name)
{
    // Need a lock on cache because some other thread might try to clear the cache entry before we can even return the data
    pthread_mutex_lock(&(fct->mutex));
    file *loc = find_in_cache(fct, file_name);
    // found file and it has been pinned, set the dirty flag to true on the file
    char *data;
    if (NULL != loc && loc->pins > 0) {
        loc->dirty = true;
        data = loc->content;
    } else { // undefined behavior
        data = NULL;
    }
    pthread_mutex_unlock(&(fct->mutex));
    return data;
}
