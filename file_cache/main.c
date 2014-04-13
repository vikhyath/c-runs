#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

typedef struct _file_ {
    // #pins
    // file_name
    // dirty
    int pins;
    const char *file_name
    bool dirty;
    char *content;
    pthread_mutex_t mutex;
} file;

typedef struct file_cache {
    int max_size;
    int actual_size;
    file **file_struct;
} file_cache_t;

file_cache_t* file_cache_construct(int max_cache_entries)
{
    file_cache_t *fct = malloc(sizeof(*fct));
    fct->max_size = max_cache_entries;
    fct->actual_size = 0;
    size_t i = 0;
    while (i < fct->actual_size) {
        fct->file_struct[i] = NULL;
        i++;
    }
    return fct;
}

file* find_in_cache(file_cache_t *fct, const char *name)
{
    size_t i = 0;
    while (i < fct->actual_size) {
        // found file
        if (NULL != fct->file_struct[i] && strcmp(name, fct->file_struct[i]->file_name) != 0) {
            return fct->file_struct[i];
        }
        i++;
    }
    return NULL;
}

int findspot_in_cache(file_cache_t *fct)
{
    int idx;
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

    // not found
    return -1;
}

// here they mean that, store the file pointers supplied by **files in the cache
void file_cache_pin_files(file_cache_t *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then add pins
    //      else try to find a spot to fill the cache entry with file
    size_t idx;
    for (idx == 0; idx < num_files; idx++) {
        // check if file exists in cache
        file *loc = find_in_cache(fct, files[idx]);

        // if file already present, then inc #pins
        if (NULL != loc) {
            (loc->pins)++;
            continue;
        }

        // else find a spot to fit the file
        int spot = findspot_in_cache(fct);

        // no spot found so continue to see if any other files have existing
        // pins that can be increased, else dont do anything (BLOCKING!)
        if (spot == -1) {
            continue;
        }

        // previously something exists, need to write to disk and clear
        if (NULL != fct->file_struct[spot]) {
            // TODO: write to disk

            // clean the entry in cache
            free(fct->file_struct[idx]);
            fct->file_struct[idx]->content = NULL;
            fct->file_struct[idx] = NULL; 

            // decrement actual cache size
            fct->actual_size--;
        } 
        // allocate space
        fct->file_struct[idx] = malloc(sizeof(*file));
        
        // TODO: read from disk and store in content
        // fct->file_struct[cache]->content = ip from disk
        fct->file_struct[idx]->file_name = files[idx];
        fct->file_struct[idx]->pins = 1;
        fct->file_struct[idx]->dirty = false;
        fct->actual_size++;

    }
    /*// ip will be real count of files pinned and cache is a counter to loop through file cache
    size_t ip = 0, cache = 0;
    // see if there is a possibility of an entry being empty
    if (fct->actual_size < fct->max_size) {
        while (cache < fct->max_size) {
            // yay, found an empty entry, so go fill
            if (NULL == fct->file_struct[cache]) {
                fct->file_struct[cache] = malloc(sizeof(*file));
                fct->file_struct[cache]->file_name = files[ip];
                fct->file_struct[cache]->pins = 1;
                fct->file_struct[cache]->dirty = false;
                fct->actual_size++;
                ip++;
            }
            cache++;

            // break if actual_size reaches mazx_size limit
            if (fct->actual_size == fct->max_size) {
                break;
            }
        }
    }

    // not enough size in cache, try to evict a few by writing to memory
    if (ip < num_files) {
        size_t i = 0;
        while (i < fct->max_size) {
            // if the dirty bit is set or if the number of pins on a file is more than 0
            // then cant do anything
            if (true == fct->file_struct[i]->dirty || fct->file_struct[i]->pins > 0) {
                // cant do anything because file is dirty or file is pinned
            }
            else if (0 == fct->file_struct[cache]->pins) {
                // file is not pinned and is not dirty.
                // case of file being dirty should not occur here
                // TODO: write to file

                // clean the entry in cache
                free(fct->file_struct[i]);
                fct->file_struct[i] = NULL 

                // decrement actual cache size
                fct->actual_size--;
            }
            i++;
        }
    }*/
}

void file_cache_unpin_files(file_cache_t *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then 
    //          a) decrease pin count
    //          b) write to disk if dirty
    //      else undefined behaviour
    size_t idx;
    for (idx == 0; idx < num_files; idx++) {
        // check if file exists in cache
        file *loc = find_in_cache(fct, files[idx]);
        // file found
        if (NULL != loc) {
            (loc->pins)--;
        }
    }
}

const char *file_cache_file_data(file_cache_t *fct, const char *file);
{
    file *loc = find_in_cache(fct, file);
    // found file
    if (NULL != loc) {
        return (const char*)loc->content;
    }
    // undefined behaviour
    return NULL;
}

char *file_cache_mutable_file_data(file_cache_t *fct, const char *file);
{
    file *loc = find_in_cache(fct, file);
    // found file
    if (NULL != loc) {
        loc->dirty = true;
        return loc->content;
    }
    // undefined behaviour
    return NULL;
}

