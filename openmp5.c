#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Escreva um programa serial e paralelo em C, com OpenMP, que dado um vetor de inteiros e um valor x, 
// encontrar quantas vezes x ocorre no vetor. Calcular as métricas de desempenho.

#define TAM 100000000

double ocorrenciaSerial(int* vetor, int x) {
    int qtd = 0;
    double start, end, time = 0.0;

    printf("Iniciando busca de ocorrência em serial!\n");
    start = omp_get_wtime();

    for(int i = 0; i < TAM; i++) {
        if(vetor[i] == x) {
            qtd++;
        }
    }

    end = omp_get_wtime();
    printf("O valor x = %d ocorre %d no vetor!\n", x, qtd);
    time = end - start;
    printf("Tempo em serial: %f\n\n", time);
    printf("====================================================\n\n\n");
    
    return time;
}

double ocorrenciaParaleloCritico(int* vetor, int x) {
    int qtd = 0;
    double start, end, time = 0.0;

    printf("Iniciando busca de ocorrência em paralelo!\n");
    start = omp_get_wtime();

    #pragma omp parallel num_threads(4)
    {
        int soma = 0;
        #pragma omp for 
        for(int i = 0; i < TAM; i++) {
            if(vetor[i] == x) {
                soma++;
            }
        }

        #pragma omp critical
        {
            qtd += soma;
        }
    }


    end = omp_get_wtime();
    printf("O valor x = %d ocorre %d no vetor!\n", x, qtd);
    time = end - start;
    printf("Tempo em paralelo: %f\n\n", time);
    printf("====================================================\n\n\n");
    
    return time;
}

double ocorrenciaParaleloEstatico(int* vetor, int x) {
    int qtd = 0;
    double start, end, time = 0.0;

    printf("Iniciando busca de ocorrência em paralelo estático!\n"); 
    start = omp_get_wtime();

    #pragma omp parallel for schedule(static) num_threads(4) reduction(+:qtd)
        for(int i = 0; i < TAM; i++) {
            if(vetor[i] == x) {
                qtd++;
            }
        }

    end = omp_get_wtime();
    printf("O valor x = %d ocorre %d no vetor!\n", x, qtd);
    time = end - start;
    printf("Tempo em paralelo estático: %f\n\n", time);
    printf("====================================================\n\n\n");
    
    return time;
}

double ocorrenciaParaleloDinamico(int* vetor, int x) {
    int qtd = 0;
    double start, end, time = 0.0;

    printf("Iniciando busca de ocorrência em paralelo dinâmico!\n");
    start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic) num_threads(4) reduction(+:qtd)
        for(int i = 0; i < TAM; i++) {
            if(vetor[i] == x) {
                qtd++;
            }
        }

    end = omp_get_wtime();
    printf("O valor x = %d ocorre %d no vetor!\n", x, qtd);
    time = end - start;
    printf("Tempo em paralelo dinâmico: %f\n\n", time);
    printf("====================================================\n\n\n");
    
    return time;
}

void preencherVetorEstatico(int* vetor) {
    srand(time(NULL));

    int val;
    double start, end, time1;

    start = omp_get_wtime();

    #pragma omp parallel for schedule(static) num_threads(4) 
        for(int i = 0; i < TAM; i++) {
            val = random() % 100000;
            vetor[i] = val;
        }

    
    end = omp_get_wtime();
    time1 = end - start;
    printf("Tempo de preenchimento estático: %f\n", time1);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    int x;
    int* vetor = malloc(sizeof(int) * TAM);

    printf("Gerando valores aleatórios: \n\n");
    preencherVetorEstatico(vetor);
    
    printf("Gerando valor aleatório de x: ");
    srand(time(NULL));
    x = random() % 100000;
    printf("%d\n\n", x);
    printf("====================================================\n\n");

    double time = ocorrenciaSerial(vetor, x)/ocorrenciaParaleloCritico(vetor, x);
    ocorrenciaParaleloEstatico(vetor, x);
    ocorrenciaParaleloDinamico(vetor, x);
    printf("Speedup: %f seconds\n", time);
    printf("Eficiência: %f seconds\n", time/4);
    printf("====================================================\n\n\n");

    free(vetor);
    return 0;
}