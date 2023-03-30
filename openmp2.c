#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>

// Exercício 2: Aproximação do PI
// OpenMP: Redução (Reduction)
#define STEPS 100000000

double pi_sequencial() {
    int i;
    double step, x, pi, sum = 0.0;
    double start, end, time;
    
    printf("Iniciando método sequencial...\n");
    start = omp_get_wtime();
    step = 1.0/(double)STEPS;

    for(i = 0; i < STEPS; i++) {
        x = i * step;
        sum = sum + 4.0/(1.0 + x * x);
    }
    pi = step * sum;
    
    end = omp_get_wtime();
    time = end - start;
    printf("Pi: %f\n", pi);
    printf("Tempo Sequencial: %f\n\n", time);
    return time;
}

double pi_paralelo() {
    int i;
    double step, x, pi, sum = 0.0;
    double start, end, time;
    
    printf("Iniciando método paralelo...\n");
    start = omp_get_wtime();
    step = 1.0/(double)STEPS;

    #pragma omp parallel private(x) num_threads(4)
    {
        #pragma omp for reduction(+:sum)
            for(i = 0; i < STEPS; i++) {
                x = i * step;
                sum = sum + 4.0/(1.0 + x * x);
            }
        pi = step * sum;
    }
    
    end = omp_get_wtime();
    time = end - start;
    printf("Pi: %f\n", pi);
    printf("Tempo Paralelo: %f\n\n", time);
    return time;
}

int main() {
    printf("Calculando o valor de pi...\n\n");
    double time = pi_sequencial()/pi_paralelo();
    printf("Speedup: %f seconds\n", time);
    printf("Eficiência: %f seconds\n", time/4);
}