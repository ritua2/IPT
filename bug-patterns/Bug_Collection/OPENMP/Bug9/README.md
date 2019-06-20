### Instructions

Requirements:
1. Intel compiler

or

2. gcc and g++


#### Compile on Stampede2 or systems that support icpc:

```bash
# Incorrect version
icpc -qopenmp -o buggy_version buggy_version.c

# Fixed version
icpc -qopenmp -o fixed_version fixed_version.c
```


#### Compile using g++:

```bash
# Incorrect version
g++ -fopenmp -o buggy_version buggy_version.c

# Fixed version
g++ -fopenmp -o fixed_version fixed_version.c
```


### Run using 4 OpenMP threads:

```bash
# Incorrect version
export OMP_NUM_THREADS=4 && ./buggy_version  

# Fixed version
export OMP_NUM_THREADS=4 && ./fixed_version
```
