/*
 * Assumptions:
 * a) When a cache is destroyed, even if there are pins on files, they are ignored and dirty files are written to disk
 * b) If there are multiple calls to write to the file_memory, when the file is unpinned by one call, it will be written to disk only if the number of pins on the file are 0
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#define FILESIZE 10 * 1000 * 1024

typedef struct _file_ {
    int pins; // number of pins on the file
    const char *file_name; // file name
    bool dirty; // dirty flag on the file
    char *content; // content of the file
} file;

typedef struct file_cache {
    int max_size; // max_size of the cache, set in constructor
    int actual_size; // actual entries in cace (NOT USED)
    file **file_struct; // file structrues that exist in the cache
} file_cache_t;

/*
 * Create the file cache of size max_cache_entries
 * @params integer max_cache_entries
 * @return file_cache_t type pointer
 */
file_cache_t* file_cache_construct(int max_cache_entries)
{
    // assign memory to the file_cache struct
    file_cache_t *fct = malloc(sizeof(*fct));
    fct->max_size = max_cache_entries;
    fct->actual_size = 0;
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
 * @params file_cache_t type structure
 * @return void
 */
void file_cache_destroy(file_cache_t *fct)
{
    size_t i;
    // for every file in cache, if it is dirty, write contents to disk
    for (i = 0; i < fct->max_size; i++) {
      if (NULL != fct->file_struct[i] && true == fct->file_struct[i]->dirty) {
          write_to_disk(fct->file_struct[i]);
      }
      // if content is not NULL then free
      if (NULL != fct->file_struct[i] && NULL != fct->file_struct[i]->content) {
          free(fct->file_struct[i]->content);
      }
      // free file_struct
      free(fct->file_struct[i]);
    }
    fct->max_size = 0;
    // free file cache
    free(fct);
    fct = NULL;
}

/*
 * File file in cache, if it exists
 * @params file_cache_t file_cache, file_name
 * @return file_struct if any or NULL
 */
file* find_in_cache(file_cache_t *fct, const char *name)
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
 * @params file_cache_t file_cache
 * @return index in cache to insert OR -1 if no possibility
 */
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


/*
 * Pin given files of size num_files in the file_cache_t file_cache *fct
 * @params file_cache_t type file_cache, list of file names, number of files
 * @return void
 */
void file_cache_pin_files(file_cache_t *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then increment pin count
    //      else try to find a spot to fill the cache entry with file
    size_t idx;
    for (idx = 0; idx < num_files; idx++) {
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
        // pins that can be increased, else dont do anything
        if (spot == -1) {
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
            free(fct->file_struct[idx]);
            fct->file_struct[idx] = NULL;

            // decrement actual cache size
            fct->actual_size--;
        }

        // allocate space
        fct->file_struct[idx] = malloc(sizeof(file));

        // read from disk and store in content
        fct->file_struct[idx]->file_name = files[idx];
        fct->file_struct[idx]->pins = 1;
        fct->file_struct[idx]->dirty = false;
        read_from_disk(fct->file_struct[idx]);
        fct->actual_size++;
    }
}

/*
 * Unpin the given files list in the file cache
 * @params file_cache_t type file_cache, list of files to be pinned and its count num_files
 * @return void
 */
void file_cache_unpin_files(file_cache_t *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then
    //          a) decrease pin count
    //          b) write to disk if dirty and pin count is 0
    //      else undefined behaviour
    size_t idx;
    for (idx = 0; idx < num_files; idx++) {
        // check if file exists in cache
        file *loc = find_in_cache(fct, files[idx]);
        // file found
        if (NULL != loc) {
            (loc->pins)--;
            if (true == loc->dirty && 0 == loc->pins) {
                // write to disk
                write_to_disk(loc);
                loc->dirty = false;
            }
        }
    }
}

/*
 * Get read only memory for the specified file in the file cache
 * @params file_cache_t type file_cache, the file to read
 * @return return read only memory or NULL if the file does not exist
 */
const char *file_cache_file_data(file_cache_t *fct, const char *file_name)
{
    file *loc = find_in_cache(fct, file_name);
    // found file
    if (NULL != loc) {
        return (const char*)loc->content;
    }
    // undefined behaviour
    return NULL;
}

/*
 * Get a writable memory for the specified file in the file_cache
 * @params file_cache_t type file_cache, the file to write to
 * @return writable memory of NULL if the file does not exist
 */
char *file_cache_mutable_file_data(file_cache_t *fct, const char *file_name)
{
    file *loc = find_in_cache(fct, file_name);
    // found file, set the dirty flag to true on the file
    if (NULL != loc) {
        loc->dirty = true;
        return loc->content;
    }
    // undefined behaviour
    return NULL;
}

int main(int argc, char **argv)
{
    file_cache_t *fct = file_cache_construct(3);
    const char *file_names[] = {"one","two","three"};
    file_cache_pin_files(fct, file_names, 3);
    char *y = file_cache_mutable_file_data(fct, "one");
    const char *testfiles[] = {"one"};
    //file_cache_unpin_files(fct, testfiles, 1);
    const char* newfile[] = {"four"};
    file_cache_pin_files(fct, newfile, 1);
    file_cache_destroy(fct);

    /*int i = 0;
    for (i = 0; i < 3; i++) {
      printf("file %s\n", fct->file_struct[i]->file_name);
      getchar();
      printf("content is %s\n", fct->file_struct[i]->content);

      }*/
    /*    write_to_disk(fct->file_struct[0]);
    const char *x = file_cache_file_data(fct, "one");
    printf("%p\n", x);
    char *y = file_cache_mutable_file_data(fct, "one");
    y[FILESIZE - 1] = '1';
    write_to_disk(fct->file_struct[0]);
    y = file_cache_mutable_file_data(fct, "one");
    // printf("%s\n", y);
    const char* newfile[] = {"four"};
    file_cache_pin_files(fct, newfile, 1);
    */
    return 0;
}
