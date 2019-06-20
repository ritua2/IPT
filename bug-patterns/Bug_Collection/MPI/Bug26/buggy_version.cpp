#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
    int rank, size, ball_value, ball_present;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    srandom(rank);
        int delta = rand() % 13;
    int random = rand() % 5;
        if (random == 0) delta = -1*delta;
    if (rank == 0) {
        ball_present = 1;
        ball_value = 0;
    }
    else ball_present = 0;
    while (1) {
        if(ball_present == 0) 
            MPI_Recv(&ball_value, 30, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ball_present = 1;
        printf("Task %d has Ball with value %d\n", rank, ball_value);
        if (ball_value == 1000) break;
        if (abs(ball_value) > 100) {
            int send_1000 = 1000;
            int i;
            for (i = 0; i < size; i++)
                if (i != rank) MPI_Send(&send_1000, 30, MPI_INT, i, 10, MPI_COMM_WORLD); //Broadcast to all others
            break;  
        }
        ball_value += delta;
        int next_to_send = rand() % size;
        if (next_to_send != rank) {
            printf("Sending ball to %d\n", next_to_send);
            MPI_Send(&ball_value, 30, MPI_INT, next_to_send, 10, MPI_COMM_WORLD);
            ball_present = 0;
            }
    }
    MPI_Finalize();
    return 0;
}