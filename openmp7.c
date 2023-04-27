#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TAM 100000000

// Escreva um programa, com OpenMP, que dado um conjunto aleatório de dados (floats), calcule o desvio padrão deste conjunto de valores.

double desvioPadraoSerial(double *array) {
    // Passo 1: Calcular a média dos valores
    double average = 0.0, variance = 0.0, standard_deviation = 0.0;
    double* difference_vector = (double*)malloc(sizeof(double) * TAM);
    double start, end, time = 0.0;

    printf("Iniciando cálculo em serial!\n");
    start = omp_get_wtime();

    for(int i = 0; i < TAM; i++) {
        average += array[i];
    }

    average = average/TAM;
    printf("Média: %f\n", average);

    // Passo 2: Calcular o quadrado da diferença de cada um dos elementos em relação à média.
    for(int i = 0; i < TAM; i++) {
        difference_vector[i] = (array[i] - average) * (array[i] - average);
    }

    // Passo 3: Calcular a média entre esses valores encontrados (Variância)
    for(int i = 0; i < TAM; i++) {
        variance += difference_vector[i];
    }

    variance = variance/TAM;
    printf("Variância: %f\n", variance);

    // Passo 4: Por fim, o desvio-padrão é a raiz quadrada da variância.
    standard_deviation = sqrt(variance);

    end = omp_get_wtime();
    printf("Desvio padrão em serial = %f \n", standard_deviation);
    time = end - start;
    printf("Tempo em serial: %f\n\n", time);
    printf("====================================================\n\n\n");

    free(difference_vector);
    return time;
}

double desvioPadraoParalelo(double *array) {
    // Passo 1: Calcular a média dos valores
    double average = 0.0, variance = 0.0, standard_deviation = 0.0;
    double* difference_vector = (double*)malloc(sizeof(double) * TAM);
    double start, end, time = 0.0;

    printf("Iniciando cálculo em paralelo!\n");
    start = omp_get_wtime();

    #pragma omp parallel num_threads(4)
    {
        double average_private = 0.0;
        #pragma omp for 
        for(int i = 0; i < TAM; i++) {
            average_private += array[i];
        }

        #pragma omp critical
        {
            average += average_private;
        }
    }

    average = average/TAM;
    printf("Média: %f\n", average);

    // Passo 2: Calcular o quadrado da diferença de cada um dos elementos em relação à média.

    #pragma omp parallel for schedule(static) num_threads(4) 
        for(int i = 0; i < TAM; i++) {
            difference_vector[i] = (array[i] - average) * (array[i] - average);
        }
    
    // Passo 3: Calcular a média entre esses valores encontrados (Variância)

    #pragma omp parallel num_threads(4)
    {
        double variance_private = 0.0;
        #pragma omp for 
        for(int i = 0; i < TAM; i++) {
            variance_private += difference_vector[i];
        }

        #pragma omp critical
        {
            variance += variance_private;
        }
    }
    
    variance = variance/TAM;
    printf("Variância: %f\n", variance);

    // Passo 4: Por fim, o desvio-padrão é a raiz quadrada da variância.
    standard_deviation = sqrt(variance);

    end = omp_get_wtime();
    printf("Desvio padrão em paralelo = %f \n", standard_deviation);
    time = end - start;
    printf("Tempo em paralelo: %f\n\n", time);
    printf("====================================================\n\n\n");

    free(difference_vector);

    return time;
}

void preencherVetorEstatico(double* array) {
    srand(time(NULL));

    double val;
    double start, end, time1;

    start = omp_get_wtime();

    #pragma omp parallel for schedule(static) num_threads(4) 
        for(int i = 0; i < TAM; i++) {
            val = (float)random() / 100000;
            array[i] = val;
        }

    
    end = omp_get_wtime();
    time1 = end - start;
    printf("Tempo de preenchimento estático: %f\n", time1);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    double* array = (double*)malloc(sizeof(double) * TAM);

    printf("Gerando valores aleatórios: \n\n");
    preencherVetorEstatico(array);
    printf("====================================================\n\n");

    double time = desvioPadraoSerial(array)/desvioPadraoParalelo(array);
    printf("Speedup: %f seconds\n", time);
    printf("Eficiência: %f seconds\n", time/4);
    printf("====================================================\n\n\n");
    free(array);

    return 0;
}