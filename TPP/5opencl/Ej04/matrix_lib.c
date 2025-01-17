//------------------------------------------------------------------------------
//
//  PROGRAM: Matrix library for the multiplication driver
//
//  PURPOSE: This is a simple set of functions to manipulate
//           matrices used with the multiplcation driver.
//
//  USAGE:   The matrices are square and the order is
//           set as a defined constant, ORDER.
//
//  HISTORY: Written by Tim Mattson, August 2010
//           Modified by Simon McIntosh-Smith, September 2011
//           Modified by Tom Deakin and Simon McIntosh-Smith, October 2012
//           Ported to C by Tom Deakin, 2013
//
//------------------------------------------------------------------------------

#include "matmul.h"

//------------------------------------------------------------------------------
//
//  Function to compute the matrix product (sequential algorithm, dot prod)
//
//------------------------------------------------------------------------------

void seq_mat_mul_sdot(int N, float *A, float *B, float *C)
{
    int i, j, k;
    float tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            tmp = 0.0f;
            for (k = 0; k < N; k++) {
                /* C(i,j) = sum(over k) A(i,k) * B(k,j) */
                tmp += A[i*N+k] * B[k*N+j];
            }
            C[i*N+j] = tmp;
        }
    }
}

//------------------------------------------------------------------------------
//
//  Function to initialize the input matrices A and B
//
//------------------------------------------------------------------------------
void initmat(int N, float *A, float *B, float *C)
{
    int i, j;

    /* Initialize matrices */

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			A[i*N+j] = AVAL;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			B[i*N+j] = BVAL;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			C[i*N+j] = 0.0f;
}

//------------------------------------------------------------------------------
//
//  Function to set a matrix to zero
//
//------------------------------------------------------------------------------
void zero_mat (int N, float *C)
{
    int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			C[i*N+j] = 0.0f;
}

//------------------------------------------------------------------------------
//
//  Function to fill Btrans(N,N) with transpose of B(N,N)
//
//------------------------------------------------------------------------------
void trans(int N, float *B, float *Btrans)
{
    int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
		    Btrans[j*N+i] = B[i*N+j];
}

//------------------------------------------------------------------------------
//
//  Function to compute errors of the product matrix
//
//------------------------------------------------------------------------------
float error(int N, float *C)
{
   int i,j;
   float cval, errsq, err;
   cval = (float) N * AVAL * BVAL;
   errsq = 0.0f;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            err = C[i*N+j] - cval;
            errsq += err * err;
        }
    }
    return errsq;
}

//------------------------------------------------------------------------------
//
//  Function to analyze and output results
//
//------------------------------------------------------------------------------
void results(int N, float *C, double run_time)
{
    float gflops;
    float errsq;

    gflops = 2.0 * N * N * N/(1.0E09 * run_time);
    printf(" %.2f mseconds at %.2f GFLOPS \n",  1000*run_time, gflops);
    errsq = error(N, C);
    if (isnan(errsq) || errsq > TOL) {
        printf("\n Errors in multiplication: %f\n",errsq);
        exit(1);
    }
}

