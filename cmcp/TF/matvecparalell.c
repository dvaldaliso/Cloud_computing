#include <stdio.h>
#include <stdlib.h>

#define NREPS 10000

/* 
 * Multiplicación de una matriz banda por un vector
 *  w = A*v, con A matriz cuadrada de dimensión N y ancho de banda b
 *  Algoritmo orientado a filas
 */
void matvec(int N,int b,double *A, double *v, double *w)
{
  int i, j, li, ls;

  for (i=0; i<N; i++) {
    w[i] = 0.0;
    li = i-b<0? 0: i-b;  /* limite inferior */
    ls = i+b>N-1? N-1: i+b;  /* limite superior */
    for (j=li; j<=ls; j++) {
      w[i] += A[i*N+j]*v[j];
    }
  }
}

int main(int argc, char **argv) 
{
  int i, j, k, N=50, b=4;
  double *A, *v, *w;

  /* Extracción de argumentos */
  if (argc > 1) { /* El usuario ha indicado el valor de N */
     if ((N = atoi(argv[1])) < 0) N = 50;
  }
  if (argc > 2) { /* El usuario ha indicado el valor de b */
     if ((b = atoi(argv[2])) < 0) b = 1;
  }
  if (b>=N) { /* Valor de b incorrecto */
    printf("Error: ancho de banda excesivo, N=%d, b=%d\n", N, b);
    exit(1);
  }

  /* Reserva de memoria */
  A = (double*)calloc(N*N,sizeof(double));
  v = (double*)calloc(N,sizeof(double));
  w = (double*)calloc(N,sizeof(double));

  /* Inicializar datos */
  printf("Matriz A\n");
  for (i=0; i<N; i++) A[i*N+i] = 2*b;
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      
      if (i!=j && abs(i-j)<=b){
        //printf("valor i %d, valor N %d, valor j %d, valor combinado = %d\n",i,N,j,(i*N+j));
        A[i*N+j] = -1.0;
      } 
      printf(" [%d,%d]=%lf",i,j, A[i*N+j]);
    }
     printf("\n");
  }
  
  
printf("Matriz v\n");
  for (i=0; i<N; i++) {
    v[i] = 1.0;
    if(i==N-1){
     printf(" [%d]=%lf\n",i, v[i]);   
     continue;
    }
    printf(" [%d]=%lf",i, v[i]);
  }

  /* Multiplicación de matrices */
  for (k=0; k<NREPS; k++) matvec(N,b,A,v,w);
  
  /* Imprimir solución */
  printf("Matriz resultante W\n");
  if (N<100) for (i=0; i<N; i++) printf("w[%d] = %g\n", i, w[i]);

  free(A);
  free(v);
  free(w);

  return 0;
}

