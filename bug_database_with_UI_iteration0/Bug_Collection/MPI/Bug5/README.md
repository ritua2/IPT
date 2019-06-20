### Instructions

Requirements:
1. Intel compiler and an MPI implementation, Intel MPI is valid

or

2. gcc and an MPI implementation (mpich, OpenMPI)

#### Compile:

```bash
# Incorrect version
mpicc -o buggy_version buggy_version.c

# Fixed version
mpicc -o fixed_version fixed_version.c
```



### Run on Stampede2 or systems that support ibrun:

```bash
# Incorrect version
ibrun -np 4 buggy_version  

# Fixed version
ibrun -np 4 fixed_version
```


### Run on non-TACC systems or systems that do not support ibrun

```bash
# Incorrect version
mpirun -np 4 buggy_version  

# Fixed version
mpirun -np 4 fixed_version
```
