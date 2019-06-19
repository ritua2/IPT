double* vector_vector_addition_parallel(double* a, double* b, int length)
{
  int rank, size, error;
  double* result = (double*)malloc(sizeof(double)*length);

  error = MPI_Init(NULL,NULL);
  error = MPI_Comm_size(MPI_COMM_WORLD,&size);
  error = MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  int sublist_length = (int)(length/size);

  double* sub_a = (double*)malloc(sizeof(double) * sublist_length);
  double* sub_b = (double*)malloc(sizeof(double) * sublist_length);


  error = MPI_Scatter(a, sublist_length, MPI_DOUBLE, sub_a, sublist_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  error = MPI_Scatter(b, sublist_length, MPI_DOUBLE, sub_b, sublist_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double* buffer = (double*)malloc(sizeof(double)*sublist_length);
  for(int i = 0; i < sublist_length; i++)
  {
    buffer[i] = sub_a[i] + sub_b[i];
  }

  error = MPI_Gather(buffer,sublist_length,MPI_DOUBLE,result,length,MPI_DOUBLE,0,MPI_COMM_WORLD);

  error = MPI_Finalize();

  return result;
}

int main(int argc, char** argv)
{
  double a[8] = {1.0,3.0,5.0,7.0,9.0,11.0,13.0,15.0};
  double b[8] = {2.0,4.0,6.0,8.0,10.0,12.0,14.0,16.0};
  double* vec = vector_vector_addition_parallel(a,b,8);
  return 0;
}