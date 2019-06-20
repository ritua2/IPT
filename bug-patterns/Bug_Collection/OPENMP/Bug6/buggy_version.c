#include<stdio.h>

int main(){
  int T[10];

  // initializing array T 
  for (int  i = 0; i < 10; i ++) {
    T[i] = i;
  }

  // running the loop 10 times using openmp
  // increase all element in array T by 1 each iteraion
#pragma omp parallel for shared (T)
  for ( int i = 0; i < 10; i ++) {
    for (int j =0; j < 10; j++) {
      T[j] +=1;
    }
  }
}

