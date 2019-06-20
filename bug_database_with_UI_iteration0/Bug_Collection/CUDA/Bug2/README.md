### Instructions

Requirements:
1. CUDA drivers
2. nvcc compiler



#### Compile:

```bash
# Incorrect version
nvcc buggy_version.cu -o buggy_version

# Fixed version
nvcc fixed_version.cu -o fixed_version
```



### Run

```bash
# Incorrect version
./buggy_version

# Fixed version
./fixed_version
```
