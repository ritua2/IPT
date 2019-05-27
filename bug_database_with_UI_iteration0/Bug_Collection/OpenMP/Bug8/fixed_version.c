#include<stdio.h>

int main(){
  int sum = 1;
  int i =1;

	#pragma omp parallel for private(d)
  	{
		for ( i = 0 ; i < nt1 ; i++ ) {
		  d = distancia(tabla1[i],tabla2[j]);
		  if ( d < dm ) {
		    #pragma omp critical
		    if ( d < dm ) {
		      dm = d;
		      im = i;
		    }
		  }
		}
	}
	ps[j] = im;
}
