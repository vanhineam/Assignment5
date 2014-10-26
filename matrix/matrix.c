#define N 512

#include <paraguin.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

print_results(char *prompt, double a[N][N]);

int main(int argc, char *argv[])
{
    int i, j, k, error = 0;
    double a[N][N], b[N][N], c[N][N];
    char *usage = "Usage: %s file\n";
    FILE *fd;
    double elapsed_time;
    struct timeval tv1, tv2;
    if (argc < 2) {
        fprintf (stderr, usage, argv[0]);
        error = -1;    
    }

    if ((fd = fopen (argv[1], "r")) == NULL) {
        fprintf (stderr, "%s: Cannot open file %s for reading.\n", 
                            argv[0], argv[1]);
        fprintf (stderr, usage, argv[0]);
        error = -1;
    }

    #pragma paraguin begin_parallel
    #pragma paraguin bcast error
        if (error) return -1;
    #pragma paraguin end_parallel

    // Read input from file for matrices a and b.
    // The I/O is not timed because this I/O needs
    // to be done regardless of whether this program
    // is run sequentially on one processor or in 
    // parallel on many processors. Therefore, it is
    // irrelevant when considering speedup.

    for (i = 0; i < N; i++) 
        for (j = 0; j < N; j++) 
            fscanf (fd, "%lf", &a[i][j]);

    for (i = 0; i < N; i++) 
        for (j = 0; j < N; j++) 
            fscanf (fd, "%lf", &b[i][j]);

    fclose (fd);
    ;
    #pragma paraguin begin_parallel
        // This barrier is here so that we can take a time stamp
        // Once we know all processes are ready to go.
        #pragma paraguin barrier
    #pragma paraguin end_parallel

    // Take a time stamp
    gettimeofday(&tv1, NULL);

    #pragma paraguin begin_parallel
    #pragma paraguin scatter a
    #pragma paraguin bcast b
    // Parallelize the following loop nest assigning iterations
    // of the outermost loop (i) to different partitions.
    #pragma paraguin forall
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
        	c[i][j] = 0;
        	for (k = 0; k < N; k++) {
        		c[i][j] += a[i][k] * b[k][j];
        	}
        }
    }
    ;
    #pragma paraguin gather  c  
    #pragma paraguin end_parallel

    // Take a time stamp.  This won't happen until after the master
    // process has gathered all the input from the other processes.
    gettimeofday(&tv2, NULL);

    elapsed_time = (tv2.tv_sec - tv1.tv_sec) +
                       ((tv2.tv_usec - tv1.tv_usec) / 1000000.0);

    printf ("elapsed_time=\t%lf (seconds)\n", elapsed_time);

    // print result
    print_results("C = ", c);
}


print_results(char *prompt, double a[N][N])
{
    int i, j;

    printf ("\n\n%s\n", prompt);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf(" %.2lf", a[i][j]);
        }
        printf ("\n");
    }
    printf ("\n\n");
}