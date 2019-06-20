#include <omp.h>
int main() {
  int i,j=0;
#pragma omp parallel
  for(i=0;i<1000;i++)
  {
#pragma omp barrier
    j+= j^i;
  }
  return j;
}