#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void parallel_jacobi_step(int N, int M, double *x, double *b, double *t, double *local_sum, int rank, int size) {
    int mlocal = M / size; 
    int i, j, ld = mlocal + 2;
    
    double local_s = 0.0;
    int right = rank + 1;
    int left = rank - 1;

    
  
    
    if (rank < size - 1) {
        MPI_Send(&x[mlocal * ld], 1, coltype, right, 0, MPI_COMM_WORLD);
    }

    if (rank > 0) {
        MPI_Recv(&x[0], ld, coltype, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (rank > 0) {
        MPI_Send(&x[ld], ld, coltype, left, 1, MPI_COMM_WORLD);
    }

    if (rank < size - 1) {
        MPI_Recv(&x[(mlocal + 1) * ld], ld, coltype, right, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (i = 1; i <= N; i++) {
        for (j = 1; j <= mlocal; j++) {
            t[i*ld+j] = (b[i*ld+j] + x[(i+1)*ld+j] + x[(i-1)*ld+j] + x[i*ld+(j+1)] + x[i*ld+(j-1)])/4.0;
            local_s += (x[i * ld + j] - t[i * ld + j]) * (x[i * ld + j] - t[i * ld + j]);
        }
    }

    *local_sum = local_s;
}


void parallel_jacobi_poisson(int N, int M, double *x, double *b, int rank, int size) {
    int mlocal = M / size;
    int i, j, k, ld = mlocal + 2, conv = 0, maxit = 10000;
    double *t, tol = 1e-6, local_sum, global_sum = 0.0;
        

    t = (double *)calloc((N+2) * (mlocal + 2), sizeof(double));

    k = 0;

    while (!conv && k < maxit) {
        parallel_jacobi_step(N, M, x, b, t, &local_sum, rank, size);

        MPI_Allreduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        conv = (sqrt(global_sum) < tol);

        if (rank == 0) {
            printf("Error en iteración %d: %g\n", k, sqrt(global_sum));
        }

        MPI_Barrier(MPI_COMM_WORLD);

        k = k + 1;
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= mlocal; j++) {
                x[i * ld + j] = t[i * ld + j];
            }
        }
    }

    free(t);
}

int main(int argc, char **argv) {
    int i, j, N = 60, M = 60, ld;
    double *x, *b, h = 0.01, f = 1.5;

    if (argc > 1) {
        N = atoi(argv[1]);
        if (N <= 0) N = 60;
    }
    if (argc > 2) {
        M = atoi(argv[2]);
        if (M <= 0) M = 60;
    }
    ld = M + 2;
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    int mlocal = N / size;
    MPI_Datatype coltype;
   //MPI_Type_vector(count, length, stride, type, newtype)
    MPI_Type_vector(N, 1, mlocal, MPI_DOUBLE, &coltype);
    MPI_Type_commit(&coltype);

    
    x = (double *)calloc((N+2) * (mlocal + 2), sizeof(double));
    b = (double *)calloc((N+2) * (mlocal + 2), sizeof(double));

    for (i = 1; i <= N; i++) {
        for (j = 1; j <= mlocal; j++) {
            b[i * ld + j] = h * h * f;
        }
    }

    parallel_jacobi_poisson(N, M, x, b, rank, size);

   if (N <= 60) {
    double *X = NULL;
    if (rank == 0) {
        X = (double *)calloc((N+2) * (M + 2), sizeof(double));
    }
    MPI_Gather(x + ld, mlocal * ld, MPI_DOUBLE, X + ld, mlocal * ld, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= M; j++) {
                printf("%g ", X[i * ld + j]);
            }
            printf("\n");
        }
    }
    if (rank == 0) {
        free(X);
    }
}
    free(x);
    free(b);

    MPI_Finalize();

    return 0;
}
