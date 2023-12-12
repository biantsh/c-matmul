# c-matmul
A comparison and time complexity analysis of the standard and Strassen algorithms for matrix multiplication in C.

# Instructions

To compile, make sure you have both *make* and *gcc* installed and simply run `make`. This will generate two executable
files named `standard` and `strassen`.

**Example usage:**

```bash
# ./standard {matrix-size} {num-threads}
./standard 2048 2

> Time taken: 15.335 seconds
> Matrix size: 2048

# ./strassen {matrix-size} {num-threads} {leaf-size}
./strassen 2048 2 32

> Time taken: 12.544 seconds
> Matrix size: 2048
```

*Note:* The matrices are filled automatically with random integers between 0 and 9. This is simply meant as a demo to
illustrate the execution time of these algorithms at different scales.
