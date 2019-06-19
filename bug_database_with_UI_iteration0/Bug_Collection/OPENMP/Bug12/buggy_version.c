int main(int argc, char *argv[]){
  if(argc < 2){
    printf("Usage: %s matrix/vector_size\n", argv[0]);
    return 0;
  }

  int size = atoi(argv[1]);
  double **matrix2 = (double **)malloc(sizeof(double*)*size);
  double **matrix = (double **)malloc(sizeof(double*)*size);
  double **result_sq = (double **)malloc(sizeof(double*)*size);
  double **result_pl = (double **)malloc(sizeof(double*)*size);
  int t;
  for (t =0; t<size; t++) {
      matrix[t]= (double *)malloc(sizeof(double)*size);
      matrix2[t]= (double *)malloc(sizeof(double)*size);
      result_pl[t]= (double *)malloc(sizeof(double)*size);
      result_sq[t]=(double *)malloc(sizeof(double)*size);
  }
  matrix_vector_gen(size, matrix, matrix2);