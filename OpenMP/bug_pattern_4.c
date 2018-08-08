#include<stdio.h>

int main(){
/*
int T[10];

// initializing array T using openmp
 #pragma omp parallel for private(T) 
 for ( int i = 0; i < 10; i ++) {
 	T[i] = i;
 }
*/
int T[5];
int sum = 0;
int i =0;
// initializing array T 
 for ( i = 0; i < 10; i ++) {
 	T[i] = i;
 	}

 	// running the loop 10 times using openmp
 	#pragma parallel for default(none) share(T,sum) reduction (+ : sum)
 	for ( int i = 0; i < 10; i ++) {
 		
 			// assign value for elements in array T
 			for (int j =0; j < 5; j++) {
 				T[j]  = i ;
 			}

// 								// increase "sum" by the toal of T array module by 2
		sum += (T[0] + T[1] + T[2] + T[3] + T[4]) % 2;
 }
/*  
 for ( int i = 0; i < 10; i ++) {
        printf("T[%d] = %d\n",i, T[i]);
 }

int sum = 1;
int i;

// increase sum by 10 using openmp
 #pragma omp parallel for shared (i) reduction (+: sum)
 for ( i = 0; i < 10; i++) {
 	sum +=i;
 }
*/
sum = 1;
// increase sum by 10 using openmp 
 #pragma omp parallel reduction( + : sum)
 for (int i = 0; i < 10; i ++) {
 	sum +=1;
 }
/*
#pragma omp parallel 
{
	#pragma omp for reduction (+: sum)
	for ( int i = 0; i < 10; i ++) {
		#pragma omp critical 
		{
			sum +=1;
		}
	}
}
*/
 printf("sum = %d\n",sum);

}

