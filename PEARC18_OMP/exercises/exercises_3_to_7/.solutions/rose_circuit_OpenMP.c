#include <omp.h> 
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
int main(int argc,char *argv[]);
int circuit_value(int n,int bvec[]);
void i4_to_bvec(int i4,int n,int bvec[]);
void timestamp();
/******************************************************************************/

int main(int argc,char *argv[])
/******************************************************************************/
/*
  Purpose:
    MAIN is the main program for SATISFY.
  Licensing:
    This code is distributed under the GNU LGPL license.
  Modified:
    20 March 2009
  Author:
    John Burkardt
  Reference:
    Michael Quinn,
    Parallel Programming in C with MPI and OpenMP,
    McGraw-Hill, 2004,
    ISBN13: 978-0071232654,
    LC: QA76.73.C15.Q55.
*/
{
# define N 23
  int bvec[23UL];
  int i;
  int ihi;
  int j;
  int n = 23;
  int solution_num;
  int value;
  printf("\n");
  timestamp();
  printf("\n");
  printf("SATISFY\n");
  printf("  C version\n");
  printf("  We have a logical function of N logical arguments.\n");
  printf("  We do an exhaustive search of all 2^N possibilities,\n");
  printf("  seeking those inputs that make the function TRUE.\n");
/*
  Compute the number of binary vectors to check.
*/
  ihi = 1;
  for (i = 1; i <= n; i++) {
    ihi = (ihi * 2);
  }
  printf("  The number of logical variables is N = %d\n",n);
  printf("  The number of input vectors to check is %d\n",ihi);
  printf("\n");
  printf("   #       Index    ---------Input Values------------------------\n");
  printf("\n");
/*
  Check every possible input vector.
*/
  solution_num = 0;
  
#pragma omp parallel default(none) shared(solution_num,ihi,n) private(i,value,j) firstprivate(bvec)
{
  
#pragma omp for reduction ( + :solution_num)
  for (i = 0; i < ihi; i++) {
    i4_to_bvec(i,n,bvec);
    value = circuit_value(n,bvec);
    if (value == 1) {
      solution_num = (solution_num + 1);
      printf("  %2d  %10d:  ",solution_num,i);
      for (j = 0; j < n; j++) {
        printf(" %d",bvec[j]);
      }
      printf("\n");
    }
  }
}
//  Report.
  printf("\n");
  printf("  Number of solutions found was %d\n",solution_num);
/*
  Shut down.
*/
  printf("\n");
  printf("SATISFY\n");
  printf("  Normal end of execution.\n");
  printf("\n");
  timestamp();
  return 0;
# undef N
}
/******************************************************************************/

int circuit_value(int n,int bvec[])
/******************************************************************************/
/*
  Purpose:
    CIRCUIT_VALUE returns the value of a circuit for a given input set.
  Licensing:
    This code is distributed under the GNU LGPL license.
  Modified:
    20 March 2009
  Author:
    John Burkardt
  Reference:
    Michael Quinn,
    Parallel Programming in C with MPI and OpenMP,
    McGraw-Hill, 2004,
    ISBN13: 978-0071232654,
    LC: QA76.73.C15.Q55.
  Parameters:
    Input, int N, the length of the input vector.
    Input, int BVEC[N], the binary inputs.
    Output, int CIRCUIT_VALUE, the output of the circuit.
*/
{
  int value;
  value = (((((((((((((((((((((((((((((((bvec[0] != 0) || (bvec[1] != 0)) && (!(bvec[1] != 0) || !(bvec[3] != 0))) && ((bvec[2] != 0) || (bvec[3] != 0))) && (!(bvec[3] != 0) || !(bvec[4] != 0))) && ((bvec[4] != 0) || !(bvec[5] != 0))) && ((bvec[5] != 0) || !(bvec[6] != 0))) && ((bvec[5] != 0) || (bvec[6] != 0))) && ((bvec[6] != 0) || !(bvec[15] != 0))) && ((bvec[7] != 0) || !(bvec[8] != 0))) && (!(bvec[7] != 0) || !(bvec[13] != 0))) && ((bvec[8] != 0) || (bvec[9] != 0))) && ((bvec[8] != 0) || !(bvec[9] != 0))) && (!(bvec[9] != 0) || !(bvec[10] != 0))) && ((bvec[9] != 0) || (bvec[11] != 0))) && ((bvec[10] != 0) || (bvec[11] != 0))) && ((bvec[12] != 0) || (bvec[13] != 0))) && ((bvec[13] != 0) || !(bvec[14] != 0))) && ((bvec[14] != 0) || (bvec[15] != 0))) && ((bvec[14] != 0) || (bvec[16] != 0))) && ((bvec[17] != 0) || (bvec[1] != 0))) && ((bvec[18] != 0) || !(bvec[0] != 0))) && ((bvec[19] != 0) || (bvec[1] != 0))) && ((bvec[19] != 0) || !(bvec[18] != 0))) && (!(bvec[19] != 0) || !(bvec[9] != 0))) && ((bvec[0] != 0) || (bvec[17] != 0))) && (!(bvec[1] != 0) || (bvec[20] != 0))) && (!(bvec[21] != 0) || (bvec[20] != 0))) && (!(bvec[22] != 0) || (bvec[20] != 0))) && (!(bvec[21] != 0) || !(bvec[20] != 0))) && ((bvec[22] != 0) || !(bvec[20] != 0)));
  return value;
}
/******************************************************************************/

void i4_to_bvec(int i4,int n,int bvec[])
/******************************************************************************/
/*
  Purpose:
    I4_TO_BVEC converts an integer into a binary vector.
  Licensing:
    This code is distributed under the GNU LGPL license.
  Modified:
    20 March 2009
  Author:
    John Burkardt
  Parameters:
    Input, int I4, the integer.
    Input, int N, the dimension of the vector.
    Output, int BVEC[N], the vector of binary remainders.
*/
{
  int i;
  for (i = (n - 1); 0 <= i; i--) {
    bvec[i] = (i4 % 2);
    i4 = (i4 / 2);
  }
}
/******************************************************************************/

void timestamp()
/******************************************************************************/
/*
  Purpose:
    TIMESTAMP prints the current YMDHMS date as a time stamp.
  Example:
    31 May 2001 09:45:54 AM
  Licensing:
    This code is distributed under the GNU LGPL license.
  Modified:
    24 September 2003
  Author:
    John Burkardt
  Parameters:
    None
*/
{
# define TIME_SIZE 40
  static char time_buffer[40UL];
  const struct tm *tm;
  size_t len;
  time_t now;
  now = time(0);
  tm = (localtime((&now)));
  len = strftime(time_buffer,40,"%d %B %Y %I:%M:%S %p",tm);
  printf("%s\n",time_buffer);
# undef TIME_SIZE
}
