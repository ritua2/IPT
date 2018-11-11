void __global__ kernel0(...,int device_N, int64_t* dNpairs,...){

 int64_t i =  blockIdx.x * blockDim.x + threadIdx.x;
 for (j=0; j<N*N; j++)
 {
  dNpairs[j]  = 0;
  
 }
 if((i< N))
 {
  for(j=(i+1);j<N;j++) 
  {
   // other code
   if (flag) 
   {
    //other computation            
    dNpairs[mbin+1]++;
   } 
   //other calculation
  }
 }  
}
