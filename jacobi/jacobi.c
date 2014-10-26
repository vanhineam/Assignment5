#define N = 100
#define M = 100
#define TOTAL_TIME 5000

#include <paraguin.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

double computeValue(double A[][M], int, int);
printValues(double A[][M]);

int main()
{
	int i, j, n, M, max_iteartons;
	double A[2][N][M];
	int __guin_current = 0;
	double elapsed_time;
    struct timeval tv1, tv2;

    // Initialize Array
    for(i = 0; i < N; i++) {
    	for(j = 0; j < N; j++) {
    		A[0][i][j] = 0;
    		A[1][i][j] = 0;
    	}
    }

    // Set the walls to 20C
    for(i = 0; i < N; i++) {
    	A[0][0][i] = 20.0;
    	A[0][i][0] = 20.0;
    	A[0][N-1][i] = 20.0;
    	A[0][i][N-1] = 20.0;
    }

    // Set the starting and ending points of the fireplace
    double fp_start, fp_end;
    fp_start = 0.3 * N;
    fp_end = 0.7 * N;

    // Set the fireplace 

	#pragma paraguin begin_parallel
    #pragma paraguin bcast error
    if (error) return -1;
    #pragma paraguin end_parallel

    #pragma paraguin begin_parallel
        // This barrier is here so that we can take a time stamp
        // Once we know all processes are ready to go.
        #pragma paraguin barrier
    #pragma paraguin end_parallel

    // Take a time stamp
    gettimeofday(&tv1, NULL);

    #pragma paraguin begin_parallel
	// A[0] is initialized with data somehow and duplicated into A[1]

    n = N;
    m = M;
    max_iterations = TOTAL_TIME;

    #pragma paraguin stencil A n m max_iterations computeValue

    #pragma end_parallel

	// Final result is in A[max_iterations % 2]

	// Take a time stamp.  This won't happen until after the master
    // process has gathered all the input from the other processes.
    gettimeofday(&tv2, NULL);

    elapsed_time = (tv2.tv_sec - tv1.tv_sec) +
                       ((tv2.tv_usec - tv1.tv_usec) / 1000000.0);

    printf ("elapsed_time=\t%lf (seconds)\n", elapsed_time);

    // print result
    print_results("C = ", c);
}

double computeValue(double A[][M], int i, int j) {
	return (A[i-1][j] + A[i+1][j] + A[i][j-1] + A[i][j+1]) * 0.25;
}

printValues(double A[N][M]) {
	int i, j;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("%d", A[i][j]);
		}
		printf("\n");
	}
}