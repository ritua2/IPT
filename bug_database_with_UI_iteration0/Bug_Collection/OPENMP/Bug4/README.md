### Instructions

Requirements:
1. Intel compiler

or

2. gcc


#### Compile on Stampede2 or systems that support icc:

```bash
# Incorrect version
icc -qopenmp -o buggy_version buggy_version.c

# Fixed version
icc -qopenmp -o fixed_version fixed_version.c
```


#### Compile using gcc:

```bash
# Incorrect version
gcc -fopenmp -o buggy_version buggy_version.c

# Fixed version
gcc -fopenmp -o fixed_version fixed_version.c
```


### Run using 4 OpenMP threads:

```bash
# Incorrect version
export OMP_NUM_THREADS=4 && ./buggy_version  

# Fixed version
export OMP_NUM_THREADS=4 && ./fixed_version
```
