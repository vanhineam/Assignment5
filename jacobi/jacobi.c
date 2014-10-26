#define N 100
#define M 100
#define TOTAL_TIME 5000

#include <paraguin.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

double computeValue(double A[N][M], int, int);
printValues(double A[N][M]);

int main(int argc, char** argv)
{
	int i, j, n, m, max_iterations, done;
	double A[2][N][M], *aPtr, diff, max_diff, tol;
	int __guin_current = 0;
    int __guin_next = 1;
	double elapsed_time;
    struct timeval tv1, tv2;
    double fp_start, fp_end;

    // Initialize Array
    for(i = 0; i < N; i++) {
    	for(j = 0; j < M; j++) {
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
    fp_start = 0.3 * N;
    fp_end = 0.7 * N;

    // Set the fireplace 
    for(i = fp_start; i < fp_end; i++) {
        A[0][0][i] = 100.0;
    }

    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            A[1][i][j] = A[0][i][j];
        }
    }

    #pragma paraguin begin_parallel
        // This barrier is here so that we can take a time stamp
        // Once we know all processes are ready to go.
        #pragma paraguin barrier
    #pragma paraguin end_parallel

    // Take a time stamp
    gettimeofday(&tv1, NULL);

    #pragma paraguin begin_parallel

	// A[0] is initialized with data somehow and duplicated into A[1]

    tol = 0.0001;
    n = N;
    m = M;
    max_iterations = TOTAL_TIME;

    #pragma paraguin bcast A
    done = 0; // false
    while (!done) {

        ; // This is to make sure the following pragma is inside the while 
        #pragma paraguin stencilLite A n m max_iterations computeValue
        max_diff = 0.0;
        #pragma paraguin forall
        for (i = 1; i < n - 1; i++) {
            for (j = 1; j < n - 1; j++) {
                diff = fabs(A[__guin_current][i][j] - A[__guin_next][i][j]);
                if (diff > max_diff) max_diff = diff;
            }
        }
        ; // This is needed to prevent the pragma from being located in the
          // above loop nest

        // Reduce the max_diff's from all processors
        #pragma paraguin reduce max max_diff diff


        // Broadcast the diff so that all processors will agree to continue
        // or terminate
        #pragma paraguin bcast diff

        // Termination condition if the maximum change in values is less
        // than the tolerance.
        if (diff <= tol) done = 1; // true
    }

    aPtr = &A[__guin_current][1][0];
    n = (N - 2) * M * sizeof(double);

    #pragma paraguin gather aPtr( n )

    #pragma paraguin end_parallel

    // Final result is in A[__guin_current]
    // Cannot use max_iterations % 2

	// Final result is in A[max_iterations % 2]

	// Take a time stamp.  This won't happen until after the master
    // process has gathered all the input from the other processes.
    gettimeofday(&tv2, NULL);

    elapsed_time = (tv2.tv_sec - tv1.tv_sec) +
                       ((tv2.tv_usec - tv1.tv_usec) / 1000000.0);

    printf ("elapsed_time=\t%lf (seconds)\n", elapsed_time);

    // print result
    printValues(A[max_iterations % 2]);
}

double computeValue(double A[N][M], int i, int j) {
	return (A[i-1][j] + A[i+1][j] + A[i][j-1] + A[i][j+1]) * 0.25;
}

printValues(double A[N][M]) {
	int i, j;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf(" %.2f", A[i][j]);
		}
		printf("\n");
	}
}