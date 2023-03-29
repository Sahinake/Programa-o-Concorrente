#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// Exercício 3: Média de um vetor
// Escreva um programa sequencial paralelo em C com OpenMP, que calcula a média aritmética de um vetor com 500.000.000 (quinhentos milhões) de elementos aleatórios. Os valores variam entre 1 e 100.000. Calcule o speedup e eficiência do seu programa.
double media_serial(int *vetor) {
    setlocale(LC_ALL, "Portuguese");

    int i; 
    double soma = 0;
    double start, end, time;

    start = omp_get_wtime();

    for(i = 0; i < 500000000; i ++) {
        soma = soma + vetor[i];
    }
    end = omp_get_wtime(); 
    printf("Média aritmética em serial: %f\n", soma/500000000);
    time = end - start;
    printf("Serial time: %f\n\n", time);
    return time;
}

double media_paralela(int *vetor, int nprocs) {
    setlocale(LC_ALL, "Portuguese");

    int i; 
    double soma = 0;
    double start, end, time;

    start = omp_get_wtime();
    #pragma omp parallel num_threads(nprocs)
    {
        #pragma omp for reduction (+:soma)
            for(i = 0; i < 500000000; i++) {
                soma = soma + vetor[i];
            }
    }
    end = omp_get_wtime(); 
    printf("Média aritmética em paralelo: %f\n", soma/500000000);
    time = end - start;
    printf("Parallel time: %f\n\n", time);
    return time;
}

int main() {
    srand(time(NULL));
    int i;
    int *vetor = malloc(500000000 * sizeof(int));
    int nprocs = (int)omp_get_num_procs()/2;

    printf("Gerando valores aleatórios: \n\n");
    for(i = 0; i < 500000000; i++) {
        vetor[i] = random() % 100000;
    }

    double time = media_serial(vetor)/media_paralela(vetor, nprocs);
    printf("Speedup: %f seconds\n", time);
    printf("Eficiência: %f seconds\n", time/4);

    return 0;
}