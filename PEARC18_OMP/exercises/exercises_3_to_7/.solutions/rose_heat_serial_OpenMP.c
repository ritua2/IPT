#include <omp.h> 
/* This code si contributed by Richard T. Evans at the Texas Advanced computing Center
 * The University of Texas at Austin
 *
 * To compile: icc -o heat heat_serial.c calc_up.c
*/
#include <stdio.h>
#include <sys/time.h>
#include "calc_up.h"

int main()
{
  int Nx;
  int Ny;
  int Nt;
  int t;
  int x;
  int y;
  Nx = 1000;
  Ny = 1000;
  Nt = 1000;
  double u[Nx][Ny];
  double up[Nx][Ny];
  struct timeval start;
  struct timeval end;
  float delta;
// Boundary conditions
  for (x = 0; x < Nx; x++) 
    for (y = 0; y < Ny; y++) {
      if (x == 0) 
        u[x][y] = 1.0;
      else 
        u[x][y] = 0.0;
    }
  gettimeofday(&start,0);
////////////////////////////////////////////////////////////////////////
// Finite difference algorithm - iterate over time to reach steady state
////////////////////////////////////////////////////////////////////////
  for (t = 0; t < Nt; t++) {
    
#pragma omp parallel default(none) shared(u,up,Nx,Ny) private(x,y)
{
    
#pragma omp for
    for (x = 1; x < (Nx - 1); x++) 
      for (y = 1; y < (Ny - 1); y++) 
        calc_up(x,y,Nx,Ny,u,up);
}
    
#pragma omp parallel default(none) shared(u,up,Nx,Ny) private(x,y)
{
    
#pragma omp for
    for (x = 1; x < (Nx - 1); x++) 
      for (y = 1; y < (Ny - 1); y++) 
        u[x][y] = up[x][y];
}
  }
  gettimeofday(&end,0);
  delta = (((((end.tv_sec - start.tv_sec) * 1000000u) + end.tv_usec) - start.tv_usec) / 1.e6);
  double sum = 0;
  for (y = 0; y < Ny; y++) {
    for (x = 0; x < Nx; x++) {
      sum += u[x][y];
    }
  }
  printf("run time    = %fs\n",delta);
  printf("sum of u    = %f\n",sum);
  return 0;
}
