#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>

// Exercício 1: Busca Sequencial - Escreva um programa com threads (c ou python ou os dois), que dado um vetor com 10.000.000 de posições, com elementos aleatórios, procure um valor n fornecido pelo usuário. Divida a tarefa entre 4 threads concorrentes.
// OpenMP: Redução (Reduction)

double busca_sequenciada(int valor, int *vetor) {
    int i;
    int flag = 0;
    double start, end, time;

    start = omp_get_wtime();
    printf("Iniciando Busca Sequenciada...\n");
    for(i = 0; i < 10000000; i++) {
        if(flag == 1) {
            continue;
        }
        if(vetor[i] == valor) {
            printf("O número %d está na posição %d do vetor!\n", valor, i);
            flag = 1;
        }
    }

    if(flag == 0) {
        printf("Número não encontrado no vetor!\n");
    }

    end = omp_get_wtime();
    time = end - start;
    printf("Tempo Sequencial: %f\n\n", time);
    return time;
}

double busca_paralela(int valor, int *vetor) {
    int i;
    int flag = 0;
    double start, end, time;

    start = omp_get_wtime();

    printf("Iniciando Busca Paralela...\n");
    #pragma omp parallel num_threads(4) shared(flag)
    {
        // Região Paralela
        #pragma omp for 
            for(i = 0; i < 10000000; i++) {
                if(flag == 1) {
                    continue;
                }
                if(vetor[i] == valor) {
                    printf("O número %d está na posição %d do vetor!\n", valor, i);
                    flag = 1;
                }

        }
    }
    if(flag == 0) {
        printf("Número não encontrado no vetor!\n");
    }

    end = omp_get_wtime();
    time = end - start;
    printf("Tempo Paralelo: %f\n\n", time);
    return time;
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    int i, valor;
    int *vetor = malloc(10000000 * sizeof(int));

    printf("Gerado 10.000.000 valores aleatórios... \n\n");
    for(i = 0; i < 10000000; i++) {
        vetor[i] = random() % 10000000;
        //printf("%d\n", vetor[i]);
    }

    printf("Insira o valor da busca: ");
    scanf("%d", &valor);
    printf("\n");

    double time = busca_sequenciada(valor, vetor)/busca_paralela(valor, vetor);
    printf("Speedup: %f seconds\n", time);
    printf("Eficiência: %f seconds\n", time/4);
}