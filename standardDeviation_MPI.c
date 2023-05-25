#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define TAMANHO 10000
#define SUBTAMANHO 5000

//Calcular a média dos valores
double calculateAverage(int* array) {
    double sum = 0;
    for(int i = 0; i < TAMANHO; i++) {
        sum += array[i];
    }
    return sum/TAMANHO;
}

//Calcular o quadrado da diferença de cada um dos elementos em relação à média.
//Calcular a média entre esses valores encontrados (Variância)
double calculateVariance(int* array, double media) {
    double variance = 0.0;

    for(int i = 0; i < SUBTAMANHO; i++) {
        variance += pow(((double)array[i] - media), 2);
    }
    variance = variance/TAMANHO;
    return variance;
}

void desvioPadraoSerial(int *array) {
    // Passo 1: Calcular a média dos valores
    double average = 0.0, variance = 0.0, standard_deviation = 0.0;
    int* difference_vector = (int*)malloc(sizeof(int) * TAMANHO);

    printf("Iniciando cálculo em serial!\n");
    printf("====================================================\n");

    for(int i = 0; i < TAMANHO; i++) {
        average += array[i];
    }

    average = average/TAMANHO;
    printf("Média: %f\n", average);

    // Passo 2: Calcular o quadrado da diferença de cada um dos elementos em relação à média.
    for(int i = 0; i < TAMANHO; i++) {
        difference_vector[i] = (array[i] - average) * (array[i] - average);
    }

    // Passo 3: Calcular a média entre esses valores encontrados (Variância)
    for(int i = 0; i < TAMANHO; i++) {
        variance += difference_vector[i];
    }

    variance = variance/TAMANHO;
    printf("Variância: %f\n", variance);

    // Passo 4: Por fim, o desvio-padrão é a raiz quadrada da variância.
    standard_deviation = sqrt(variance);

    printf("Desvio padrão em serial = %f \n", standard_deviation);
    printf("====================================================\n\n\n");

    free(difference_vector);
}

int main(int argc, char * argv[ ] ) {
    setlocale(LC_ALL, "Portuguese");

    MPI_Init(NULL, NULL);
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *array = NULL, *subarray = NULL;
    double media = 0.0, variance = 0.0;
    double *desvioPadrao = NULL;

    if (rank == 0) {
        srand(time(NULL));
        array = malloc(TAMANHO*sizeof(int));
        for (int i = 0; i < TAMANHO; i++) {
            int num = (rand() % TAMANHO);
            array[i] = num;
        }
        desvioPadraoSerial(array);
        printf("Iniciando cálculo com MPI!\n");
        printf("====================================================\n");
        media = calculateAverage(array);
        printf("Média: %f\n", media);
    }

    subarray = malloc(SUBTAMANHO*sizeof(int));

    MPI_Scatter(array, SUBTAMANHO, MPI_INT, subarray,SUBTAMANHO, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&media, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    variance = calculateVariance(subarray, media);

    if(rank == 0) {
        desvioPadrao = (double*) malloc(sizeof(double) * nprocs);
    }
    MPI_Gather(&variance, 1, MPI_DOUBLE, desvioPadrao, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        double desvioPadraoFinal = 0;

        for (int i = 0; i < nprocs; i++) {
            desvioPadraoFinal += desvioPadrao[i];
        }

        printf("Variância: %f\n", desvioPadraoFinal);
        desvioPadraoFinal = sqrt(desvioPadraoFinal);

        printf("Desvio Padrão com MPI: %f\n", desvioPadraoFinal);
        printf("====================================================\n\n\n");
    }

    MPI_Finalize();
    return 0;
}