#include "file_cache_driver.h"
#include <stdio.h>
#include <assert.h>
#define TCOUNT 3

file_cache *fct;

void test(void *x)
{
    int *idx = (int*) x;
    const char *file_names[] = {"five","six","seven"};
    file_cache_pin_files(fct, file_names, 3);
    /*
      if (*idx == 0) {
          file_cache_destroy(fct);
      }
    */

    // file_cache_unpin_files(fct, file_names, 3);

    // test to simulate blocking call on a thread that is going to find a valid entry in the file_cache to put a new file called "eight"
    if (*idx == 0) {
        printf("Simulating blocking call on a thread that is looking for a valid cache entry to load a new file...\n");
        const char *test[] = {"eight"};
        file_cache_pin_files(fct, test, 1);
        printf("Found a spot, done loading new file in the cache\n");
    }

    // supporting simulation of a blocking call. over here, we sleep for 5 seconds and then unpin file "five" three times to get the pin count of file "five" to 0, which then makes thread 0 insert file "eight" in the cache.
    if (*idx == 2) {
        sleep(5);
        const char *test[] = {"five"};
        printf("Creating a vacancy in the cache by unpinning file 'five'\n");
        file_cache_unpin_files(fct, test, 1);
        file_cache_unpin_files(fct, test, 1);
        file_cache_unpin_files(fct, test, 1);
    }
}

int main(int argc, char **argv)
{

    fct = file_cache_construct(3);

    // create three files, non existent ones on disk
    const char *file_names[] = {"one","two","three"};
    file_cache_pin_files(fct, file_names, 3);
    // verify three files of size 10KB are created on disk!

    // mark "one" file as mutable, thus setting dirty to true
    char *y = file_cache_mutable_file_data(fct, "one");

    // check to see if unpin works, this should also write contents to file since there are no other pins on file one
    const char *testfiles[] = {"one"};
    file_cache_unpin_files(fct, testfiles, 1);

    // if unpin worked, then  according to the cache replacement algorithm, the file "one" should be replaced in cache with file "four"
    const char* newfile[] = {"four"};
    file_cache_pin_files(fct, newfile, 1);

    // now get file one to see if it stil exists in cache, which it should not
    const char *x = file_cache_file_data(fct, "one");
    printf("asserting file 'one' has not been pinned in cache: ");
    assert(NULL == x);
    printf("TRUE\n");

    // check if file "two", "three" and "four" have been pinned in cache
    printf("asserting file 'two', ''three' and 'four' have been pinned in cache: ");
    x = file_cache_file_data(fct, "two");
    assert(NULL != x && 1 == fct->file_struct[0]->pins);

    x = file_cache_file_data(fct, "three");
    assert(NULL != x && 1 == fct->file_struct[1]->pins);

    x = file_cache_file_data(fct, "four");
    assert(NULL != x && 1 == fct->file_struct[2]->pins);
    printf("TRUE\n");

    const char *twothreefour[] = {"two", "three", "four"};
    file_cache_unpin_files(fct, twothreefour, 3);
    printf("asserting file 'two', 'three' and 'four' are not pinned in cache: ");
    x = file_cache_file_data(fct, "four");
    assert(NULL == x);

    x = file_cache_file_data(fct, "two");
    assert(NULL == x);

    x = file_cache_file_data(fct, "three");
    assert(NULL == x);
    printf("TRUE\n");

    // file "four", pinned twice
    file_cache_pin_files(fct, newfile, 1);
    file_cache_pin_files(fct, newfile, 1);
    y = file_cache_mutable_file_data(fct, "four");
    y = file_cache_mutable_file_data(fct, "four");
    printf("assert that the pin count on file 'four' is two: ");
    assert(2 == fct->file_struct[0]->pins);
    printf("TRUE\n");
    // unpin once, BUT this should not write to file, since there are
    // other pins on the file, so check the timestamp to see if it did not change
    file_cache_unpin_files(fct, newfile, 1);

    // But, destroying the cache would write all dirty buffers to file, even those that have active pins
    // y = file_cache_mutable_file_data(fct, "four");
    // file_cache_destroy(fct);

    pthread_t threads[TCOUNT];
    int arr[TCOUNT] = {[0 ... TCOUNT-1] = 0};
    size_t i = 0;
    for (i = 0; i < TCOUNT; i++) {
        arr[i] = i;
        pthread_create(&threads[i], NULL, (void *)test, (void *)&arr[i]);
    }

    for (i = 0; i < TCOUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
