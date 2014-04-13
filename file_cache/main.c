#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#define FILESIZE 10 * 1000 * 1024

typedef struct _file_ {
    int pins;
    const char *file_name;
    bool dirty;
    char *content;
}file;

typedef struct file_cache {
    int max_size;
    int actual_size;
    file **file_struct;
}file_cache_t;

file_cache_t* file_cache_construct(int max_cache_entries)
{
    file_cache_t *fct = malloc(sizeof(*fct));
    fct->max_size = max_cache_entries;
    fct->actual_size = 0;
    size_t i = 0;
    fct->file_struct = malloc(max_cache_entries * sizeof(file));
    while (i < fct->max_size) {
        (fct->file_struct)[i]= NULL;
        i++;
    }
    return fct;
}

file* find_in_cache(file_cache_t *fct, const char *name)
{
    printf("trying to find %s\n", name);
    size_t i = 0;
    while (i < fct->max_size) {
        if (NULL != fct->file_struct[i] && strcmp(name, fct->file_struct[i]->file_name) == 0) {
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
            printf("found spot at %d for new file\n", idx);
            return idx;
        }

        // dirty or pins > 0, keep looking
        if (true == fct->file_struct[idx]->dirty || fct->file_struct[idx]->pins > 0) {
          printf("found spot at %d for dirty: %d and pins %d\n", idx, fct->file_struct[idx]->dirty, fct->file_struct[idx]->pins);
            continue;
        }

        // pins == 0, valid entry, return idx
        if (0 == fct->file_struct[idx]->pins) {
            printf("found valid entry with 0 pins at %d\n", idx);
            return idx;
        }
    }

    // not found
    return -1;
}

void write_to_disk(file *target)
{
    FILE *fptr;
    fptr = fopen(target->file_name, "w");
    printf("size of target->content is %lu\n", sizeof(target->content));
    if (NULL == fptr) {
        printf("File Write Error!");
        exit(1);
    }
    fprintf(fptr, "%s", target->content);
    fclose(fptr);
}

void read_from_disk(file *target)
{
    FILE *fptr;
    fptr = fopen(target->file_name, "r");
    if (NULL == fptr) {
        FILE *filewr;
        filewr = fopen(target->file_name, "w");
        if (NULL == filewr) {
            printf("File Write Error!");
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
    fgets(target->content, FILESIZE + 1, fptr);
    fclose(fptr);
}


// here they mean that, store the file pointers supplied by **files in the cache
void file_cache_pin_files(file_cache_t *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then add pins
    //      else try to find a spot to fill the cache entry with file
    size_t idx;
    for (idx = 0; idx < num_files; idx++) {
        // check if file exists in cache
        file *loc = find_in_cache(fct, files[idx]);
        printf("loc is %p\n", loc);
        // if file already present, then inc #pins
        if (NULL != loc) {
            (loc->pins)++;
            printf("pin count is %d for file %s\n", loc->pins, loc->file_name);
            continue;
        }

        // else find a spot to fit the file
        int spot = findspot_in_cache(fct);

        // no spot found so continue to see if any other files have existing
        // pins that can be increased, else dont do anything (BLOCKING!)
        if (spot == -1) {
            continue;
        }
        printf("val is: %p\n", fct->file_struct[spot]);
        // previously something exists, need to write to disk and clear
        if (NULL != fct->file_struct[spot]) {
          printf("in here*** with file %s and flag %d\n", fct->file_struct[spot]->file_name, fct->file_struct[spot]->dirty);
            if (true == fct->file_struct[spot]->dirty) {
                printf("file %s is dirty, writing to disk\n", fct->file_struct[spot]->file_name);
                write_to_disk(fct->file_struct[spot]);
            }

            // clean the entry in cache
            free(fct->file_struct[idx]->content);
            free(fct->file_struct[idx]);
            fct->file_struct[idx]->content = NULL;
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
        printf("done creating file");
    }
}

void file_cache_unpin_files(file_cache_t *fct, const char **files, int num_files)
{
    // for each file in files
    //      if file exists in cache then
    //          a) decrease pin count
    //          b) write to disk if dirty
    //      else undefined behaviour
    size_t idx;
    for (idx = 0; idx < num_files; idx++) {
        // check if file exists in cache
        file *loc = find_in_cache(fct, files[idx]);
        // file found
        if (NULL != loc) {
            (loc->pins)--;
            if (true == loc->dirty) {
                // write to disk
                printf("file %s is dirty, writing to disk\n", loc->file_name);
                write_to_disk(loc);
                loc->dirty = false;
            }
            printf("after unpinning file %s,  pin count is %d\n", loc->file_name, loc->pins);
        }
    }
}

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

char *file_cache_mutable_file_data(file_cache_t *fct, const char *file_name)
{
    file *loc = find_in_cache(fct, file_name);
    // found file
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
