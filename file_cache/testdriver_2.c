#include "file_cache_driver.h"
#include <stdio.h>
#include <assert.h>
#define TCOUNT 3

file_cache *fct;

int main(int argc, char **argv)
{

    fct = file_cache_construct(3);

    // create three files, non existent ones on disk
    const char *file_names[] = {"one","two","three"};
    file_cache_pin_files(fct, file_names, 3);
    // verify three files of size 10KB are created on disk!

    // mark "one" file as mutable, thus setting dirty to true
    char *y = file_cache_mutable_file_data(fct, "one");

    // modify its contents, that is set the last char to 1
    y[10239] = '1';

    // check to see if unpin works, this should also write contents to file since there are no other pins on file one
    const char *testfiles[] = {"one"};
    //file_cache_pin_files(fct, testfiles, 1);
    //file_cache_unpin_files(fct, testfiles, 1);
    file_cache_unpin_files(fct, testfiles, 1);

    file_cache_pin_files(fct, testfiles, 1);
    const char *x = file_cache_file_data(fct, "one");
    printf("asserting file 'one' is not empty after writing: ");
    assert(NULL != x);
    printf("TRUE\n");

    const char *z = file_cache_file_data(fct, "invalidfile");
    printf("asserting file 'invalidfile' does not exist: ");
    assert(NULL == z);
    printf("TRUE\n");
    return 0;
}
