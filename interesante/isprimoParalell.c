#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
    int isprime(int n) {
    int m = (int)(sqrt(n));
    for (int j=2; j<=m; j++)
    if (!(n % j)) return 0;
    return 1;
    }

 int main(int argc, char **argv) {
    MPI_Init(&argc,&argv);
    
    int myrank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    int n2, primesh=0, primes, chunk=100000,
    n1 = myrank*chunk;
    while(1) {
       n2 = n1 + chunk;
       for (int n=n1; n<n2; n++) {
           if (isprime(n)) primesh++;
       }
       MPI_Reduce(&primesh,&primes,1,MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);
       n1 += size*chunk;
       if (!myrank) printf("pi( %d) = %d\n",n1,primes);
    }
    
    MPI_Finalize();
 }