#include <stdio.h>
#include <sys/time.h>
#include "calc_up.h"


int main() {

  int Nx,Ny,Nt;

  int t,x,y;

  Nx=1000;
  Ny=1000;
  Nt=1000;
  double u[Nx][Ny];
  double up[Nx][Ny];

  struct timeval start,end;
  float delta;

  // Boundary conditions
  for(x=0; x<Nx; x++)
    for(y=0; y<Ny; y++) {
      if (x==0)
	u[x][y] = 1.0;
      else
	u[x][y] = 0.0;
    }

  gettimeofday(&start, NULL);


  ////////////////////////////////////////////////////////////////////////
  // Finite difference algorithm - iterate over time to reach steady state
  ////////////////////////////////////////////////////////////////////////
  for(t=0;t<Nt;t++) {
    for(x=1; x<Nx-1; x++)
      for(y=1; y<Ny-1; y++) 
	calc_up(x,y,Nx,Ny,u,up);

    for(x=1; x<Nx-1; x++)
      for(y=1; y<Ny-1; y++) 
	u[x][y] = up[x][y];
  }

  gettimeofday(&end, NULL);
  delta = ((end.tv_sec-start.tv_sec)*1000000u + end.tv_usec-start.tv_usec)/1.e6;


  double sum = 0;
  for(y=0; y<Ny; y++) {
    for(x=0; x<Nx; x++) {
      sum += u[x][y];
    }
  }

  
  printf("run time    = %fs\n", delta);
  printf("sum of u    = %f\n",  sum);
}
