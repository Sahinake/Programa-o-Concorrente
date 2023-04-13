#include <stdio.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Escreva um programa em C, com OpenMP, que dado um número maior do que 1.000.000.000 (um bilhão), calcule a soma de todos os seus divisores.

    Utilize os métodos:
        - Explicitando uma região crítica
        - Redução com for estático
        - Redução com for dinâmico
    
    Quais variáveis são compartilhadas entre as threads, e quais são privadas?*/

    // Sequencial
double somaDivisoresSequencial(long int num) {
    long int soma = 1 + num;
    double start, end, time;

    printf("Checando valores em serial: \n\n");
    start = omp_get_wtime();

    for(int i = 2; i < num/2; i++) {
        if(num%i == 0) {
            soma += i;
        }
    }

    end = omp_get_wtime(); 
    printf("Soma dos divisores de %ld: %ld\n", num, soma);
    time = end - start;
    printf("Tempo Sequencial: %f\n\n", time);
    return time;
}

double somaDivisoresParaleloCritico(long int num) {
    double start, end, time;
    long int soma = 1 + num;
    printf("Checando valores em paralelo crítico: \n\n");
    start = omp_get_wtime();
    
    #pragma omp parallel num_threads(4)
    {   
        long int somaParcial = 0;
        #pragma omp for
        for (int i = 2;i < num/2; i ++){
            if(num%i == 0) {
                somaParcial += i;
            }
        }

        #pragma omp critical
        {
            soma += somaParcial;
        }
    }

    end = omp_get_wtime(); 
    printf("Soma dos divisores de %ld: %ld\n", num, soma);
    time = end - start;
    printf("Tempo Paralelo Crítico: %f\n\n", time);
    return time;
}

double somaDivisoresParaleloEstatico(long int num) {
    double start, end, time;
    long int soma = 1 + num;
    printf("Checando valores em paralelo estático: \n\n");
    start = omp_get_wtime();
    
    #pragma omp parallel for schedule(static) num_threads(4) reduction(+:soma)
        for (int i = 2;i < num/2; i ++){
            if(num%i == 0) {
                soma += i;
            }
        }

    end = omp_get_wtime(); 
    printf("Soma dos divisores de %ld: %ld\n", num, soma);
    time = end - start;
    printf("Tempo Paralelo Estático: %f\n\n", time);
    return time;
}

double somaDivisoresParaleloDinamico(long int num) {
    double start, end, time;
    long int soma = 1 + num;
    printf("Checando valores em paralelo dinâmico: \n\n");
    start = omp_get_wtime();
    
    #pragma omp parallel for schedule(dynamic) num_threads(4) reduction(+:soma)
        for (int i = 2;i < num/2; i ++){
            if(num%i == 0) {
                soma += i;
            }
        }

    end = omp_get_wtime(); 
    printf("Soma dos divisores de %ld: %ld\n", num, soma);
    time = end - start;
    printf("Tempo Paralelo Dinâmico: %f\n\n", time);
    return time;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));
    printf("Gerando valor aleatório: \n\n");

    long int num = (random() % 100) * pow(10,7);
    printf("Número aleatório: %ld\n\n", num);

    double time = somaDivisoresSequencial(num)/somaDivisoresParaleloEstatico(num);
    somaDivisoresParaleloCritico(num);
    somaDivisoresParaleloDinamico(num);

    printf("Speedup: %f seconds\n", time);
    printf("Eficiência: %f seconds\n", time/4);
}


