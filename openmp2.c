#include <stdio.h>
#include <omp.h>
#include <locale.h>

// Escreva um programa concorrente (em C com OpenMP), que calcula a aproximação do valor de pi.
#define STEPS 100000000

void pi_Serial() {
    setlocale(LC_ALL, "Portuguese");

    int i;
    double pi, x, num_x, sum = 0.0;
    double start, end, time;

    start = omp_get_wtime();
    num_x = 1.0/(double)STEPS;

    for(i = 0; i < STEPS; i++) {
        x = i * num_x; //x = (a + i) * num_x (Largura)
        sum = sum + 4.0 / (1.0 + x*x); // Área
    }
    pi = num_x * sum;

    end = omp_get_wtime(); 
    time = end - start;
    printf("Valor de Pi: %f\n", pi);
    printf("Serial time: %f\n\n", time);

}

double pi_Parallel() {
    setlocale(LC_ALL, "Portuguese");

    int i, num_procs = omp_get_num_procs();
    double pi, x, num_x, sum = 0.0;
    double start, end, time;

    printf("Número de Threads: %d\n", num_procs);
    start = omp_get_wtime();

    #pragma omp parallel num_threads(num_procs)
    {
        start = omp_get_wtime();
        num_x = 1.0/(double)STEPS;

        #pragma omp for reduction (+:sum)
            for(i = 0; i < STEPS; i++) {
                x = i * num_x; //x = (a + i) * num_x (Largura)
                sum = sum + 4.0 / (1.0 + x*x); // Área
            }
        pi = num_x * sum;
    }

    end = omp_get_wtime(); 
    time = end - start;
    printf("Valor de Pi: %f\n", pi);
    printf("Parallel time: %f\n\n", time);
    

}

int main() {
    setlocale(LC_ALL, "Portuguese");

    int i;
    double x, pi, sum = 0.0;

    pi_Serial();
    pi_Parallel();

}
