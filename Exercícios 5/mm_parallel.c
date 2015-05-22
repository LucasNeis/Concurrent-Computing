#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NRA 1024                 /* number of rows in matrix A */
#define NCA 1024                 /* number of columns in matrix A */
#define NCB 1024                 /* number of columns in matrix B */

int main (int argc, char *argv[])
{
  int i, j, k;

  /* matrix A to be multiplied */
  double **a = (double **) malloc(sizeof(double *) * NRA);
  double **c = (double **) malloc(sizeof(double *) * NRA);
  double **b = (double **) malloc(sizeof(double *) * NCA);
  #pragma omp parallel private(i, j, k) shared(a, b, c)
  #pragma for schedule(static) nowait
  {
  for (i = 0; i < NRA; i ++)
    a[i] = (double *) malloc(sizeof(double) * NCA);

  /* matrix B to be multiplied */
  #pragma omp for schedule(static) nowait
  for (i = 0; i < NCA; i ++)
    b[i] = (double *) malloc(sizeof(double) * NCB);

  /* result matrix C */
  #pragma omp for schedule(static) nowait
  for (i = 0; i < NRA; i ++)
    c[i] = (double *) malloc(sizeof(double) * NCB);
  
  // #pragma omp single
  // printf("Initializing matrices...\n");

  /*** Initialize matrices ***/
  #pragma omp for schedule(static) nowait
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= i+j;

  #pragma omp for schedule(static) nowait
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= i*j;

  #pragma omp for schedule(static)
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;

  /*** Do the matrix-matrix multiplication ***/
  #pragma omp for schedule(static)
  for (i=0; i<NRA; i++)
    for(j=0; j<NCB; j++)
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];

  /*** Print results ***/
 // #pragma omp single
 //  printf("******************************************************\n");
 //   #pragma omp single
 //  printf("Result Matrix:\n");
 //  #pragma omp single
 //  for (i=0; i<NRA; i++)
 //  {
 //    for (j=0; j<NCB; j++)
 //      printf("%10.2f  ", c[i][j]);
 //    printf("\n");
 //  }
 //   #pragma omp single
 //  printf("******************************************************\n");
 //   #pragma omp single
 //  printf ("Done.\n");

  #pragma omp for schedule(static)
  for (i = 0; i < NRA; i ++)
    free(a[i]);

  #pragma omp for schedule(static)
  for (i = 0; i < NCA; i ++)
    free(b[i]);

  #pragma omp for schedule(static)
  for (i = 0; i < NRA; i ++)
    free(c[i]);
  }
  
  #pragma omp single
  {
  free(a);
  free(b);
  free(c);
  }
  return 0;
}
