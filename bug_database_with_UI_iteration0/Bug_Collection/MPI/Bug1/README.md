### Instructions

#### Compile:
mpicc -o buggy_version buggy_version.c

#### Running on Stampede2 or systems that support ibrun:
ibrun -np 4 buggy_version 

#### Running on non-TACC systems or systems that do not support ibrun:
mpirun -np 4 buggy_version 

#### Compare the results of the incorrect and the fixed versions.
The bug version is liklely to produce incorrect results that do not match the results from the fixed version.


