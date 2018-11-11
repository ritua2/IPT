void __global__ kernel0(...,int device_N, int64_t* dNpairs,...)
{

  int64_t i =blockIdx.x*blockDim.x+threadIdx.x;

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

/*second kernel*/
void __global__ kernel1(...)
{

  int64_t i =  blockIdx.x * blockDim.x + threadIdx.x;

  if(i==0)
  {
    // do element-wise      
    //reduction of dNpairs
  }
}

int main(){
 //other code
 kernel0<<<dimGrid,dimBlock>>>(..);
 kernel1<<<dimGrid,dimBlock>>>(..);
 //other code
}
