#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TAM 100000

//Gerar o vetor binário com TAM + 1 elementos, sendo o último deles = 0
int* gerar_vetor_binario(int tam) {
    int *vetor = (int*) malloc(sizeof(int)*(tam + 1));

    for(int i = 0; i < tam; i++) {
        vetor[i] = rand() % 2;
    }

    vetor[tam] = 0;

    return vetor;
}


int main() {
    setlocale(LC_ALL, "Portuguese");
    MPI_Init(NULL, NULL);

    int nprocs, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *vetor;

    if(rank == 0) {
        srand(time(NULL));
        printf("Gerando vetor com MPI!\n");
        printf("====================================================\n");
        vetor = gerar_vetor_binario(TAM);
    }

    int sub_tam = TAM/nprocs, qtd_parcial_1 = 0;
    int *sub_vetor = (int*)malloc(sizeof(int)*sub_size);

    MPI_Scatter(vetor, sub_tam, MPI_INT8_T, sub_vetor, sub_tam, MPI_INT8_T, 0, MPI_COMM_WORLD);

    for(int i = 0; i < sub_tam; i++) 
        if(sub_vetor[i] == 1) {
            qtd_parcial_1++;
        }

    int *qtd_1;

    if(rank == 0) {
        qtd_1 = (int*)malloc(sizeof(int)*nprocs);
    }

    MPI_Gather(&qtd_parcial_1, 1, MPI_INT, qtd_1, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        int qtd_total = 0;

        for(int i = 0; i < nprocs; i++) {
            qtd_total += qtd_1[i];
        }
        printf("Quantidade de 1's no vetor: %d\n", qtd_total);
        printf("====================================================\n\n\n");

        if(qtd_total % 2 != 0) {     
            printf("Paridade: Ímpar\nAdicionando 1 ao final do vetor...\n");
            printf("====================================================\n\n\n");       
            vetor[TAM] = 1;
        }
        else {
            printf("Paridade: Par\nTudo certo!\n");
            printf("====================================================\n\n\n");
        }
    }

    MPI_Finalize();
}