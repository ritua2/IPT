#ifdef __PW__
  #include "pw.h"
#else
  #include <math.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <time.h>
#endif

# if _OPENMP
  #include <omp.h>
# endif

//**************************************************
// Define the problem setup
//**************************************************
//#define MAX(a, b) ( ((a) > (b)) ? (a) : (b))
#define NUMBER_VERTICES 20
#define NUMBER_ELEMENTS 500000
#define MAX_ITERATIONS  5
#define MAX_TOLERANCE   1.0e-5

//**************************************************
// Timing functions
//**************************************************
#ifdef _OPENMP
#define getClock() omp_get_wtime()
#else
#define getClock() ((double)clock() / CLOCKS_PER_SEC)
#endif

//**************************************************
// Debug options
//**************************************************
#define DEBUG_ENABLED 0
