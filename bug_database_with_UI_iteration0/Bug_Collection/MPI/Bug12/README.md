### Instructions

Requirements:
1. Intel compiler and an MPI implementation, Intel MPI is valid

or

2. gcc, g++, and an MPI implementation (mpich, OpenMPI)


#### Compile:

```bash
# Incorrect version
mpicxx -o buggy_version buggy_version.cpp

# Fixed version
mpicxx -o fixed_version fixed_version.cpp
```



### Run on Stampede2 or systems that support ibrun:

```bash
# Incorrect version
ibrun -np 2 buggy_version  

# Fixed version
ibrun -np 2 fixed_version
```


### Run on non-TACC systems or systems that do not support ibrun

```bash
# Incorrect version
mpirun -np 2 buggy_version  

# Fixed version
mpirun -np 2 fixed_version
```
