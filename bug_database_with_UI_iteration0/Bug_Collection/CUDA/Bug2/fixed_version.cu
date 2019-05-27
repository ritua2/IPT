#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

void __global__ kernel0(int64_t Npart,int64_t* totalNpairs, int64_t* npairs){

 int64_t i =  blockIdx.x * blockDim.x + threadIdx.x;
 if(i< Npart) {
  for(int64_t j = 0;j < Npart;j++) {
      totalNpairs[(i*Npart)+j]+=7;
   }
 }
 __syncthreads();
 // the code below for reduction is incorrect and should be commented, while the commented code in the main function should be uncommented
  if(i==0){
     for(int64_t k=0; k<Npart; k++){
       for (int64_t j=0; j<Npart; j++){
              npairs[j] += totalNpairs[(k*Npart)+ j];
       }
     }
  }
 
}


void __global__ kernel1(int64_t *npairs, int64_t* totalNpairs, int64_t Npart){

 int64_t i =  blockIdx.x * blockDim.x + threadIdx.x;
 
 if(i==0){
     for(int64_t k=0; k<Npart; k++){
      for (int64_t j=0; j<Npart; j++){
              npairs[j] += totalNpairs[(k*Npart)+ j];
      }
     }

 }

}


int main(int argc,char **argv){
  int64_t *device_npairs;
  int64_t *device_totalNpairs;

  int64_t Npart=10000;
  
  int64_t *npairs = (int64_t *) (calloc(Npart,sizeof((*npairs))));

  int64_t *totalNpairs = (int64_t *) (calloc((Npart*Npart),sizeof(( *npairs))));


  int D_rows = (Npart > 1024 ) ? Npart/1024 : Npart;
  int D_cols = (Npart > 1024 ) ? 1024 : 1;
  if ( Npart % 1024 && Npart > 1024){
    D_rows++;
  }

  printf("\nD_rows:%d, D_cols:%d\n",D_rows, D_cols);

  dim3 dimGrid(D_rows,1);
  dim3 dimBlock(D_cols,1);

  for (int64_t j=0; j<Npart; j++){
    npairs[j]  = 5;
  }
  cudaMalloc((void **) &device_npairs,(Npart)*sizeof(int64_t));
  cudaMemcpy(device_npairs,npairs,(Npart)*sizeof(int64_t),cudaMemcpyHostToDevice);


  for (int64_t j=0; j<Npart*Npart; j++){
    totalNpairs[j]  = 0;
  }

  cudaMalloc((void **) &device_totalNpairs,(Npart*Npart)*sizeof(int64_t));
  cudaMemcpy(device_totalNpairs,totalNpairs,(Npart*Npart)*sizeof(int64_t),cudaMemcpyHostToDevice);


  kernel0<<<dimGrid,dimBlock>>>(Npart, device_totalNpairs, device_npairs);

 
  cudaMemcpy(totalNpairs,device_totalNpairs,(Npart*Npart)*sizeof(int64_t), cudaMemcpyDeviceToHost); 
  
  kernel1<<<dimGrid,dimBlock>>>(device_npairs,device_totalNpairs, Npart);

  cudaMemcpy(npairs,device_npairs,(Npart)*sizeof(int64_t), cudaMemcpyDeviceToHost);

  cudaFree(device_npairs);
  cudaFree(device_totalNpairs);

  //only printing the first 100 elements
  for (int i = 0; i < 100; i++) {
    fprintf(stdout,"%lu\t ",npairs[i]);
  }

  free(npairs);
  free(totalNpairs);
  return 0;
}
