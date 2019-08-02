### Instructions

Requirements:
1. Intel compiler

or

2. gcc



**Note**  
Two fixed versions are provided for this example, one utilizing OpenMP single operations, the second using a double for-loop with OpenMP parallelizing the outer loop.


#### Compile on Stampede2 or systems that support icc:

```bash
# Incorrect version
icc -qopenmp -o buggy_version buggy_version.c

# Fixed version
icc -qopenmp -o fixed_version fixed_version.c

# Fixed version 2
icc -qopenmp -o fixed_version2 fixed_version2.c
```


#### Compile using gcc:

```bash
# Incorrect version
gcc -fopenmp -o buggy_version buggy_version.c

# Fixed version
gcc -fopenmp -o fixed_version fixed_version.c

# Fixed version 2
gcc -fopenmp -o fixed_version2 fixed_version2.c
```


### Run using 4 OpenMP threads:

```bash
# Incorrect version
export OMP_NUM_THREADS=4 && ./buggy_version  

# Fixed version
export OMP_NUM_THREADS=4 && ./fixed_version

# Fixed version 2
export OMP_NUM_THREADS=4 && ./fixed_version2
```
