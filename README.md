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

 - Trie: add, find and delete O(m), where m is the size of the word being added<br>
    1. Advantages: BST takes O(mlogn) to add, find and delete words.Because there are logn keys to check and each key has to do m comparisions. Trie is also better than hash because a bad hash collision resolution method with need O(n) comparisions to lookup a key, rather O(mn) if the key is a word and the hash bucket has n keys. Also, we do not need an explicit hash function to resolve buckets and no need of rebuilding hash table if it gets full. We also need less space with tries. Longest prefix matching is easily possible with tries.
    2. Disadvantages: Tries can be slower in some cases when compared to hash tables for lookups if the data is directly accessed from the disk where the random access time is high.
 - integer-array-sum: You have two integer arrays. Treat these arrays as if they were big numbers,
with one digit in each slot. Perform addition on these two arrays and store the results in a new array. Ref: http://www.careercup.com/question?id=6330205329162240
