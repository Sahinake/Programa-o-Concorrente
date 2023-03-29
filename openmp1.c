#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>

// Exercício 1: Busca Sequencial - Escreva um programa com threads (c ou python ou os dois), que dado um vetor com 10.000.000 de posições, com elementos aleatórios, procure um valor n fornecido pelo usuário. Divida a tarefa entre 4 threads concorrentes.
// OpenMP: Redução (Reduction)
int main() {
    setlocale(LC_ALL, "Portuguese");

    int i;

    printf("Gerando 10.000.000 valores aleatórios: \n\n");

    #pragma omp parallel num_threads(4)
    {
        int *vetor;
        // Região Paralela
        #pragma omp for 
            for(i = 0; i < 10000000; i++) {
                vetor[i] = random() % 10000000;
                printf("%d\n", vetor[i]);
            }
    }
}