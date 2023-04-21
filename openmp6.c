#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Escreva um programa em C, com OpenMP, que encontre o maior valor de um vetor aleatório.
#define TAM 100000000

double maiorValorSerial(float* vetor) {
    int indice = 0;
    float maior = vetor[0];
    double start, end, time = 0.0;

    printf("Iniciando busca em serial!\n");
    start = omp_get_wtime();

    for(int i = 1; i < TAM; i++) {
        if(maior < vetor[i]) {
            maior = vetor[i];
            indice = i;
        }
    }

    end = omp_get_wtime();
    printf("O maior valor no vetor é de %f\n", maior);
    printf("Indice: %d\n", indice);
    time = end - start;
    printf("Tempo em serial: %f\n\n", time);
    printf("====================================================\n\n\n");
    
    return time;
}

double maiorValorParalelo(float* vetor) {
    int indice = 0;
    float maior = vetor[0];
    double start, end, time = 0.0;

    printf("Iniciando busca em paralelo!\n");
    start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic) num_threads(4)
        for(int i = 1; i < TAM; i++) {
            if(maior < vetor[i]) {
                maior = vetor[i];
                indice = i;
            }
        }

    end = omp_get_wtime();
    printf("O maior valor no vetor é de %f\n", maior);
    printf("Indice: %d\n", indice);
    time = end - start;
    printf("Tempo em serial: %f\n\n", time);
    printf("====================================================\n\n\n");
    
    return time;
}

void preencherVetorEstatico(float* vetor) {
    float val;
    double start, end, time1;

    start = omp_get_wtime();

    #pragma omp parallel for schedule(static) num_threads(4) 
        for(int i = 0; i < TAM; i++) {
            val = (float)random()/10000;
            vetor[i] = val;
        }

    
    end = omp_get_wtime();
    time1 = end - start;
    printf("Tempo de preenchimento estático: %f\n", time1);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    float* vetor = malloc(sizeof(float) * TAM);

    printf("Gerando valores aleatórios: \n\n");
    preencherVetorEstatico(vetor);

    double time = maiorValorSerial(vetor)/maiorValorParalelo(vetor);
    printf("Speedup: %f seconds\n", time);
    printf("Eficiência: %f seconds\n", time/4);
    printf("====================================================\n\n\n");

    free(vetor);
    return 0;

}