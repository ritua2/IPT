#include <iostream>
#include <stdio.h>
#include <cstdlib>
using namespace std;
int main(int argc, char** argv){
  //int ** mult = (int**)malloc (sizeof(int)*M);
  //int **a = (int**) malloc (sizeof(int)*M);
  //int **b = (int**) malloc (sizeof(int)*K);
  const int M = 10;
  const int N = 10;
  const int K = 10;
  
  int mult[M][N];
  int a[M][K];
  int b[K][N];
  //a = &a_t;
  //b = &b_t;
  //mult = &mult_t;
  cout << "here\n";
  for (int i = 0; i < M; ++i) {
    for ( int k = 0; k < K; k++) {
		  a[i][k] = i;
    }
  } 
  for (int k = 0; k < K; ++k) {
      
    for ( int j = 0; j < N; j++) {
      b[k][j] = j;
    }
  }     
  for (int i = 0; i < M; ++i) {
       
    for ( int j = 0; j < N; j++) {
      mult[i][j] = 0;
    }
  } 

  for(int i = 0; i < M; ++i) {
        for(int j = 0; j < N; ++j ) {
            for(int k = 0; k < K; ++k)
            {
                mult[i][j] += a[i][k] * b[k][j];
            }
	}
}
  for (int i = 0; i < M; ++i) {
        for ( int j = 0; j < N; j++) {
                cout << mult[i][j] << " ";
        }
	cout << "\n";
  }
  return 0;

}
