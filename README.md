c-runs
======

c exercises

 - interpolation-search: <br>O(log(log(n))), does better than binary search if the elements in the array are uniformly distributed, else O(n) worst than binary search.

- n-way merge sort: <br>Program to do to n-way merge, where n is the SIZE of n*n array. Emulating n-way merge from file through arrays, where each array row corresponds to a sorted file<br>O(n^2 log(n)) run time since min-heap is used, else O(n^3) run time.
    1. Get sizeof(min-heap) elements from n*n array
    2. Put in min heap, remove the root element O(log n)
    3. Fill min heap with a new element from array O(#elements) = O(n^2)
    4. Do steps 2-3 till n*n array is exhausted
    5. Now remove the root from min heap as many times as there are elements left in heap