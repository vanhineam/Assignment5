/*
 * This file was created automatically from SUIF
 *   on Fri Oct 24 11:58:20 2014.
 *
 * Created by:
 * s2c 1.3.0.1 (plus local changes) compiled Wed Jun 19 10:52:12 EDT 2013 by cferner on babbage.local
 *     Based on SUIF distribution 1.3.0.1
 *     Linked with:
 *   libsuif1 1.3.0.1 (plus local changes) compiled Tue Jun 18 11:54:53 EDT 2013 by cferner on babbage.local
 *   libuseful 1.3.0.1 (plus local changes) compiled Wed Aug 22 20:31:58 EDT 2012 by cferner on babbage.local
 */


#include <mpi.h>
/* file set entry annotes: */
/* history: "/share/apps/suifhome/x86_64-redhat-linux/bin/snoot /tmp/scc31547_0.i /tmp/scc31547_1.snt" */
/* history: "/share/apps/suifhome/x86_64-redhat-linux/bin/porky -defaults /tmp/scc31547_1.snt /tmp/scc31547_2.0" */
/* history: "/share/apps/suifhome/x86_64-redhat-linux/bin/porky -defaults /tmp/scc31547_2.0 /tmp/scc31547_3.1" */
/* history: "/share/apps/suifhome/x86_64-redhat-linux/bin/porky -kill-redundant-line-marks /tmp/scc31547_3.1 /tmp/scc31547_4.spd" */
/* history: "/share/apps/suifhome/x86_64-redhat-linux/bin/paraguin /tmp/scc31547_4.spd -o /tmp/scc31547_5.spd" */
/* history: "/share/apps/suifhome/x86_64-redhat-linux/bin/porky -unused-syms -unused-types /tmp/scc31547_5.spd /tmp/scc31547_6.2" */
/* history: "/share/apps/suifhome/x86_64-redhat-linux/bin/s2c -annotes-named history /tmp/scc31547_6.2 hello.out.c" */

struct ompi_communicator_t;
struct ompi_status_public_t;

extern int MPI_Init();
extern int MPI_Finalize();
extern int MPI_Comm_rank();
extern int MPI_Comm_size();
extern int printf(const char *, ...);

extern int __guin_rank;

extern int main(int, char **);
extern int gethostname();

extern int __guin_NP;
extern int __guin_chunksz;
extern int __guin_cycle;
extern int __guin_nCycles;
extern int (*__guin_sendsizes)[1];
extern int (*__guin_sendoffsets)[1];
extern struct ompi_status_public_t __guin_status;
extern int __guin_p;
extern int __guin_iteration;
extern int __guin_current;
extern int __guin_next;
extern void *__guin_dataPtr;
extern int __guin_size;

int __guin_rank;
int __guin_NP;
int __guin_chunksz;
int __guin_cycle;
int __guin_nCycles;
int (*__guin_sendsizes)[1];
int (*__guin_sendoffsets)[1];
struct ompi_status_public_t __guin_status;
int __guin_p;
int __guin_iteration;
int __guin_current;
int __guin_next;
void *__guin_dataPtr;
int __guin_size;

extern int main(int argc, char **argv)
  {
    char hostname[256];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &__guin_NP);
    MPI_Comm_rank(MPI_COMM_WORLD, &__guin_rank);
    if (__guin_rank == 0)
      {
        printf("Master process %d starting.\n", __guin_rank);
      }
    gethostname(hostname, 255);
    printf("Hello world from process %3d on machine %s.\n", __guin_rank, hostname);
    if (__guin_rank == 0)
      {
        printf("Goodbye world from process %d.\n", __guin_rank);
      }
    MPI_Finalize();
    return 0;
  }
