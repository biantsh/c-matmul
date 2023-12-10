# c-matmul
A comparison and time complexity analysis of the standard and Strassen algorithms for matrix multiplication in C.

# Instructions

To compile, make sure you have both *make* and *gcc* installed and simply run `make`. This will generate two executable
files named `standard` and `strassen`.

**Example usage:**

```bash
# ./standard {matrix-size}
./standard 512

> Time taken: 0.260 seconds
> Matrix size: 512

# ./strassen {matrix-size} {leaf-size}
./strassen 512 32

> Time taken: 0.195 seconds
> Matrix size: 512
```

*Note:* The matrices are filled automatically with random integers between 0 and 9. This is simply meant as a demo to
illustrate the execution time of these algorithms at different scales.
