#include <omp.h>
#include <stdio.h>

int main()
{
  FILE *f;
  int a[100];
  f = fopen("test.txt","r");

#pragma omp parallel default(none) shared(a, f)
{

  #pragma omp for
  for (int i = 0; i < 100; i++) {
    fscanf(f,"%d\n",&a[i]);
    //#pragma omp single
    printf("%d\n",a[i]);
  }
}
fclose(f);
return 0;
}
