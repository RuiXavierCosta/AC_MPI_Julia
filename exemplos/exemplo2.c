#include <stdio.h>
#include <string.h>
#include "mpi.h" /* i n c l u d e s MPI l i b r a r y code s p e c s */

#define MAXSIZE 100

int main (int argc ,char *argv[]) {
    int myRank; /* rank ( i d e n t i t y ) o f p r o c e s s */
    int numProc; /* number o f p r o c e s s o r s */
    int source; /* rank o f sen de r */
    int dest; /* rank o f d e s t i n a t i o n */
    int tag = 0 ; /* t a g t o d i s t i n g u i s h messages */
    char mess[MAXSIZE]; /* message ( o t h e r t y p e s p o s s i b l e ) */
    int count; /* number o f i tem s in message */
    MPI_Status status; /* s t a t u s o f message r e c e i v e d */
    MPI_Init(&argc ,&argv); /* s t a r t MPI */
    /* g e t number o f p r o c e s s e s */
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    /* g e t rank o f t h i s p r o c e s s */
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    /* code t o send , r e c e i v e and p r o c e s s messages */
    if (myRank != 0 ) { /* a l l p r o c e s s e s send t o r o o t */
        /* c r e a t e message */
        sprintf( mess , "Hello from %d" , myRank ) ;
        dest = 0 ; /* d e s t i n a t i o n i s r o o t */
        count = strlen( mess ) + 1; /* i n c l u d e ’ \ 0 ’ in message */
        MPI_Send( mess , count , MPI_CHAR,
            dest , tag , MPI_COMM_WORLD) ;
    } else{/* r o o t ( 0 ) p r o c e s s r e c e i v e s and p r i n t s messages */
        /* from each p r o c e s s o r in rank o r de r */
        for(source = 1 ;source < numProc ;source++){
            MPI_Recv ( mess , MAXSIZE, MPI_CHAR,
                source , tag , MPI_COMM_WORLD, &status );
            printf( "%s \n" , mess );
        }
    }
    MPI_Finalize(); /* s h u t down MPI */
}