#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char** argv) {
    // Definições
    double x, y, z, pi;
    int i, count = 0, mycount, iterations = 100000;
    int rank, size, proc;
    MPI_Status status;

    // Iniciar MPI
    MPI_Init(&argc, &argv);

    // Número de Processos
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        count = 0;
        for(proc = 1; proc < size; proc++) {
            MPI_Recv(&mycount, 1, MPI_REAL, proc, 1, MPI_COMM_WORLD, &status);
            count += mycount;
        }
        
        pi = (double) 4*count/((size - 1)*iterations);
        printf("Quantidade de iterações: %d\n",(size - 1)*iterations);
        printf("Valor de PI estimado: %g", pi);
    }
    else {
        srand(rank);
        mycount = 0;
        for(i = 0; i < iterations; i++) {
            // Coordenada x
            x =(double) rand()/RAND_MAX;
            // Coordenada y
            y = (double) rand()/RAND_MAX;
            // Distância até o centro
            z = (x*x) + (y*y);
            // Verificar se a coordenada está dentro do círculo
            if(z <= 1)
                mycount++;
        }
        printf("Processo %d enviando uma contagem total de %d\n", rank, mycount);
        // Enviar contagem ao processo
        MPI_Send(&mycount, 1, MPI_REAL, 0, 1, MPI_COMM_WORLD);
    }
    // Finalizar MPI
    MPI_Finalize();
    return 0;
}