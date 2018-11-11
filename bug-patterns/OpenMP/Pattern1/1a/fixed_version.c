#include<stdio.h>

int main(){
int sum = 1;

// increase sum by 10 using openmp 
 #pragma omp parallel for reduction(+ :sum) 
 for (int i = 0; i < 10; i ++) {
 	sum +=1;
 }
}

