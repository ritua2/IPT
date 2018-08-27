#include <stdio.h>

__global__ void kernel0(int n, float a, float *x, float *y){

  int i = blockIdx.x*blockDim.x + threadIdx.x;
 


  //comment out this for-loop and uncomment the code in the main function for getting correct results
  for (int i = 0; i < n; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }

  if (i < n){
	 y[i] = a*x[i] + y[i];
  }
}

int main(){
  int N = 1048576;

  //float x[N];

  float y[N], *d_x, *d_y;

  cudaMalloc(&d_x, N*sizeof(float)); 
  cudaMalloc(&d_y, N*sizeof(float));

  //for (int i = 0; i < N; i++) {
  //  x[i] = 1.0f;
  //  y[i] = 2.0f;
  //}

  //cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice);
  //cudaMemcpy(d_y, y, N*sizeof(float), cudaMemcpyHostToDevice);

  kernel0<<<(N+255)/256, 256>>>(N, 2.0f, d_x, d_y);

  cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);

  float maxError = 0.0f;
  for (int i = 0; i < N; i++)
    maxError = max(maxError, abs(y[i]-4.0f));
  printf("Max error: %f\n", maxError);

  cudaFree(d_x);
  cudaFree(d_y);
}
