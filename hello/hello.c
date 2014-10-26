#include <paraguin.h>
#include <stdio.h>

int __guin_rank = 0;

int main(int argc, char** argv) {
	char hostname[256];

	printf("Master process %d starting.\n", __guin_rank);

	#pragma paraguin begin_parallel

	gethostname(hostname, 255);
	printf("Hello world from process %3d on machine %s.\n",
									__guin_rank, hostname);

	#pragma paraguin end_parallel

	printf("Goodbye world from process %d.\n",
												__guin_rank);

	return 0;
}
