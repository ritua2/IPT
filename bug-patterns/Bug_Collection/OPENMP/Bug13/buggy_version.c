#include <stdio.h>
#include <math.h>


#define N 729
#define reps 1000 
#include <omp.h> 


double a[N][N], b[N][N], c[N];
int jmax[N];  


void init1(void);
void init2(void);
void runloop(int); 
void loop1chunk(int, int);
void loop2chunk(int, int);
void valid1(void);
void valid2(void);
int affinity(int*, int*, int, int, float, int*, int*);

int main(int argc, char *argv[]) { 

    double start1,start2,end1,end2;
    int r;

  init1(); 

  start1 = omp_get_wtime(); 

  for (r=0; r<reps; r++){ 
    runloop(1);
  } 

  end1  = omp_get_wtime();  

  valid1(); 

  printf("Total time for %d reps of loop 1 = %f\n",reps, (float)(end1-start1)); 


  init2(); 

  start2 = omp_get_wtime(); 

  for (r=0; r<reps; r++){ 
    runloop(2);
  } 

  end2  = omp_get_wtime(); 

  valid2(); 

  printf("Total time for %d reps of loop 2 = %f\n",reps, (float)(end2-start2)); 

} 

void init1(void){
  int i,j; 

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      a[i][j] = 0.0; 
      b[i][j] = 3.142*(i+j); 
    }
  }

}

void init2(void){ 
  int i,j, expr; 

  for (i=0; i<N; i++){ 
    expr =  i%( 3*(i/30) + 1); 
    if ( expr == 0) { 
      jmax[i] = N;
    }
    else {
      jmax[i] = 1; 
    }
    c[i] = 0.0;
  }

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      b[i][j] = (double) (i*j+1) / (double) (N*N); 
    }
  }

}  


void runloop(int loopid)
{
  int nthreads = omp_get_max_threads(); // we set it before the parallel region, using opm_get_num_threads() will always return 1 otherwise                             
    int ipt = (int) ceil((double)N/(double)nthreads);           
    float chunks_fraction = 1.0 / nthreads;                                 
    int threads_lo_bound[nthreads];                                         
    int threads_hi_bound[nthreads];                                     

    #pragma omp parallel default(none) shared(threads_lo_bound, threads_hi_bound, nthreads, loopid, ipt, chunks_fraction)
    {
        int myid = omp_get_thread_num();
        int lo = myid * ipt;
    int hi = (myid+1)*ipt;
    if (hi > N) hi = N; 

        threads_lo_bound[myid] = lo;
        threads_hi_bound[myid] = hi;

        int current_lower_bound = 0;
        int current_higher_bound = 0;
        int affinity_steal = 0;

        while(affinity_steal != -1)
        {
            switch(loopid)
            {
                case 1: loop1chunk(current_lower_bound, current_higher_bound); break;
                case 2: loop2chunk(current_lower_bound, current_higher_bound); break;
            }

            #pragma omp critical
            {
                affinity_steal = affinity(threads_lo_bound, threads_hi_bound, nthreads, myid, chunks_fraction, &current_lower_bound, &current_higher_bound);
            }
        }
    }
}

int affinity(int* threads_lo_bound, int* threads_hi_bound, int num_of_thread, int thread_num, float chunks_fraction, int *current_lower_bound, int *current_higher_bound)
{
    int current_pos;

    if (threads_hi_bound[thread_num] - threads_lo_bound[thread_num] > 0)
    {
        current_pos = thread_num;
    }
    else
    {
        int new_pos = -1;
        int jobs_remain = 0;
    int i;
        for (i = 0; i < num_of_thread; i++)
        {
            int diff = threads_hi_bound[i] - threads_lo_bound[i];
            if (diff > jobs_remain)
            {
                new_pos = i;
                jobs_remain = diff;
            }
        }

        current_pos = new_pos;
    }

    if (current_pos == -1) return -1;

    int remaining_iterations = threads_hi_bound[current_pos] - threads_lo_bound[current_pos];
    int iter_size_fractions = (int)ceil(chunks_fraction * remaining_iterations);

    *current_lower_bound = threads_lo_bound[current_pos];
    *current_higher_bound = threads_lo_bound[current_pos] + iter_size_fractions;
    threads_lo_bound[current_pos] = threads_lo_bound[current_pos] + iter_size_fractions;

    return current_pos;
}

void loop1chunk(int lo, int hi) { 
  int i,j; 

  for (i=lo; i<hi; i++){ 
    for (j=N-1; j>i; j--){
      a[i][j] += cos(b[i][j]);
    } 
  }

} 


void loop2chunk(int lo, int hi) {
  int i,j,k; 
  double rN2; 

  rN2 = 1.0 / (double) (N*N);  

  for (i=lo; i<hi; i++){ 
    for (j=0; j < jmax[i]; j++){
      for (k=0; k<j; k++){ 
    c[i] += (k+1) * log (b[i][j]) * rN2;
      } 
    }
  }

}

void valid1(void) { 
  int i,j; 
  double suma; 

  suma= 0.0; 
  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      suma += a[i][j];
    }
  }
  printf("Loop 1 check: Sum of a is %lf\n", suma);

} 


void valid2(void) { 
  int i; 
  double sumc; 

  sumc= 0.0; 
  for (i=0; i<N; i++){ 
    sumc += c[i];
  }
  printf("Loop 2 check: Sum of c is %f\n", sumc);
}      