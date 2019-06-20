#! /bin.bash
mpicc -o buggy_version buggy_version.c
buggy_value=$(mpirun -np 2 buggy_version)



mpicc -o fixed_version fixed_version.c
fixed_value=$(mpirun -np 2 fixed_version)

if [[ $buggy_value != $fixed_value ]]; then
	echo "Test failed"
	echo "---------------------------------------"
	echo "Output of buggy version is:"
	echo "$buggy_value"

	echo "---------------------------------------"

	echo "Output of fixed_version is:"
	echo "$fixed_value"
	echo "---------------------------------------"
else
	echo "Test successed"
fi



