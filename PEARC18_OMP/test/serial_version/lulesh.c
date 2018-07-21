//include <omp.h> 
#include "CORAL_lulesh_2.0.3_mk.h"

void initialize_array_randomly(int *mat,int n,int max_value)
{
  for (int i = 0; i < n; i++) {
    mat[i] = rand() % max_value;
  }
}

void initialize_array_sequentialy(int *mat,int n)
{
  
  for (int i = 0; i < n; i++) {
    mat[i] = i;
  }
}

void initialize_array_to_zero(int *mat,int n)
{
  for (int i = 0; i < n; i++) {
    mat[i] = 0;
  }
}

double checksum(double *mat,int n)
{
  if (!mat) 
    return 0;
  double checkSum = 0.0;
  for (int i = 0; i < n; i++) 
    checkSum += mat[i];
  return checkSum;
}

void compute_energy_for_elems(double *energy,int nelems,int nvertices)
{
  double TMP[nvertices];
  for (int r = 0; r < 3; r++) {
    float pi = 2.0f * (asin(1.0f));
    for (int nver = 0; nver < nvertices; nver++) {
      double tmp = (sin((pi * nver / (nvertices - 1))));
      TMP[nver] = tmp * (exp((-pi)));
    }
  }
  for (int nver = 0; nver < nvertices; nver++) {
    energy[nver] = TMP[nver];
  }
}

void compute_temporary_solution(double *T,double *A_old,int nvertices,int nelems,int *nodes,double *energy)
{
  for (int nver = 0; nver < nvertices; nver++) {
    T[nver] = energy[nver];
  }
  
  {
  
    for (int nel = 0; nel < nelems; nel++) {
      double elem_contribution = 0;
      for (int nver = 1; nver < nvertices - 1; nver++) {
	double T1 = 0;
	for (int order = 0; order < 10; order++) {
	  float pi = 2.0f * (asin(1.0f));
	  double tmp = (sin((pi * nver / (nvertices - 1))));
	  T1 += tmp * (exp((-pi)));
	}
	if (T1 < 1) {
	  elem_contribution += 0;
	}
	else {
	  double T0 = A_old[nver - 1] + A_old[nver] + A_old[nver + 1];
	  elem_contribution += 0.33f * T0;
	}
      }
	T[nodes[nel]] += elem_contribution * 0.0000001f;
      
    }
  }
}

int luleshmk(double *A,double *T,int nvertices,int nelems,int *nodes,double *energy,double tol,int iter_max)
{
  // Timestep loop of the simulation
  int iter = 0;
  double err = tol;
   #if DEBUG_ENABLED
   #endif
  for ((iter = 0 , err = tol); err >= tol && iter < iter_max; iter++) {
    // Dump for debugging purposes only
    //printf(" %i", iter);
    // Compute energy for each element
    compute_energy_for_elems(energy,nelems,nvertices);
    // Compute temporary solution T
    compute_temporary_solution(T,A,nvertices,nelems,nodes,energy);
      #if DEBUG_ENABLED
      #endif
    // Compute the difference between solutions T & A
    double err = 0;
    for (int nver = 0; nver < nvertices; nver++) {
      err += (fabs(T[nver] - A[nver]));
    }
    // Update the solution A with the values of the temporary solution T
    for (int nver = 0; nver < nvertices; nver++) {
      A[nver] = T[nver];
    }
  }
   #if DEBUG_ENABLED
   #endif
  return iter;
}

int main(int argc,char *argv[])
{
  if (argc > 1) {
    printf("Usage: %s \n",argv[0]);
    exit(0);
  }
  // ================================================
  printf("- Configuring the test...\n");
  //NUMBER_VERTICES;
  int nvertices = 100;
  //NUMBER_ELEMENTS;
  int nelements = 200;
  //MAX_ITERATIONS;
  int param_iters = 10;
  //MAX_TOLERANCE;
  double param_tol = 0.01;
  double *A = (double *)(calloc(nvertices,sizeof(double )));
  if (!A) {
    printf("Error: Not enough memory to allocate array <A> ");
    printf("of size nvertices=%i\n",nvertices);
    exit(0);
  }
  double *T = (double *)(calloc(nvertices,sizeof(double )));
  if (!T) {
    printf("Error: Not enough memory to allocate array <T> ");
    printf("of size nvertices=%i\n",nvertices);
    exit(0);
  }
  int *nodes = (int *)(calloc(nelements,sizeof(int )));
  if (!nodes) {
    printf("Error: Not enough memory to allocate array <nodes> ");
    printf("of size nelements=%i\n",nelements);
    exit(0);
  }
  initialize_array_randomly(nodes,nelements,nvertices);
  double *energy = (double *)(calloc(nelements,sizeof(double )));
  if (!nodes) {
    printf("Error: Not enough memory to allocate array <energy> ");
    printf("of size nelements=%i\n",nelements);
    exit(0);
  }
  float pi = 2.0f * (asin(1.0f));
  for (int nver = 0; nver < nvertices; nver++) {
    double tmp = (sin((pi * nver / (nvertices - 1))));
    A[nver] = tmp * (exp((-pi)));
  }
   #if DEBUG_ENABLED
   #endif
  printf("nvertices\t= %i\n",nvertices);
  printf("nelements\t= %i\n",nelements);
  printf("param_iters\t= %i\n",param_iters);
  printf("param_tol\t= %.6f\n",param_tol);
  // ================================================
  printf("- Executing the test...\n");
  double time_start = (getClock());
  luleshmk(A,T,nvertices,nelements,nodes,energy,param_tol,param_iters);
  double time_finish = (getClock());
  // ================================================
  printf("- Verifying the test...\n");
  printf("nvertices\t= %i\n",nvertices);
  printf("nelements\t= %i\n",nelements);
  printf("param_iters\t= %i\n",param_iters);
  printf("param_tol\t= %.6f\n",param_tol);
  printf("time (s)= %.6f\n",time_finish - time_start);
  printf("chksum\t= %.6f\n",(checksum(A,nvertices)));
   #if DEBUG_ENABLED
   #endif
  for (int nver = 0; nver < nvertices; nver++) {
    printf("A[%i] = %.6f\n",nver,A[nver]);
  }
  return 0;
}

