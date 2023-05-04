# Programação Concorrente
#### 04/05/2023


Nome: Luma Araujo de Oliveira
Matrícula: 2021007580

## O que é MPI?
MPI é um padrão de interface para a troca de mensagens em máquinas paralelas com memória distribuída. Consiste em uma biblioteca de **Message Passing** ¹ desenvolvida para ambientes de memória distribuída, máquinas paralelas massivas, NOWs (network of workstations) e redes heterogêneas.
1. O modelo de **Message Passing** é um conjunto de processos que possuem acesso à memória local. As informações são enviadas da memória local do processo para a memória local do processo remoto.


No padrão MPI, uma aplicação é constituída por um ou mais processos que se comunicam, acionando-se funções para o envio e recebimento de mensagens entre os processos. Inicialmente, na maioria das implementações, um conjunto fixo de processos é criado. Porém, esses processos podem executar diferentes programas. Por isso, o padrão MPI é algumas vezes referido como MPMD (multiple program multiple data).

##### MPI - Básico
Embora o MPI seja um sistema complexo, um amplo conjunto de problemas pode ser resolvido usando-se apenas 6 funções, que servem basicamente para: iniciar, terminar, executar e identificar processos, enviando e recebendo mensagens.
#
| Função |  |
| :------: | ------ |
|MPI_Init | Inicia uma execução MPI|
| MPI_Finalize | Finaliza a execução |
| MPI_Comm_size | Determina o número de processos |
| MPI_Comm_rank | Determina a identificação de processos |
| MPI_Send | Envia mensagens |
| MPI_Recv | Recebe mensagens |

Todos os procedimentos acima, exceto os dois primeiros, possuem um manipulador de comunicação como argumento. Esse manipulador identifica o grupo de processos e o contexto das operações que serão executadas.Os comunicadores proporcionam o mecanismo para identificar um subconjunto de processos, durante o desenvolvimento de programas modulares, assegurando que as mensagens, planejadas para diferentes propósitos, não sejam confundidas.

#### Como funciona MPI?

##### Instalação
**Linux(baseados em Ubuntu/debian)**
```sh
sudo apt-get install openmpi-bin
```
**Compilar**
```sh
mpicc fonte.c -o executavel
```
**Executar**
```sh
mpiexec -n número_de_processos executavel
```

Os problemas são divididos em pequenas partes e essas partes são distribuídas para que outras máquinas do cluster façam o cálculo em cimas dessas partes. Os resultados obtidos das outras máquinas são enviadas a uma receptora.

As funções `MPI_INIT` e `MPI_FINALIZE` são usadas, respectivamente, para iniciar e finalizar uma execução MPI. A `MPI_INIT` deve ser chamada antes de qualquer função MPI e deve ser acionada por cada processador. Depois de ser acionada a `MPI_FINALIZE`, não se pode acessar outras funções da biblioteca.

A função `MPI_COMM_SIZE` determina o número de processos da execução corrente e a `MPI_COMM_RANK` os identifica, usando um número inteiro. Os processos,pertencentes a um grupo, são identificados com um número inteiro precedido de um 0. As funções `MPI_SEND` e `MPI_RECV`, são usadas para enviar e receber mensagens.

## Exemplos de MPI
#
```sh
#include "mpi.h“
#include <stdio.h>

Int main( int argc, char * argv[ ] ) {
    int processId;                                      /* rank dos processos */
    int noProcesses;                                    /* Número de processos */
    int nameSize;                                       /* Tamanho do nome */
    char computerName[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Get_processor_name(computerName, &nameSize);
    fprintf(stderr,"Hello from process %d on %s\n", processId, computerName);
    MPI_Finalize( );
    return 0; 
}
```

##### Cálculo do Pi
Calculada em paralelo, dividindo-se o intervalo de integração entre os processos.
```sh
#include “mpi.h”
#include <math.h>

int main () { 
    int n, myid, numprocs, i, rc;
    double mypi, pi, h, x, sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_COMM_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0) {
        printf (“Entre com o número de intervalos: ”);
        scanf(“%d”, &n);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    if (n != 0) {
        h=1.0/(double) n;
        for (i=myid +1; i <= n; i+=numprocs) {
            x = h * ((double) i – 0.5);
            sum += (4.0/(1.0 + x*x));
        }
        mpi = h* sum;
        MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_WORLD_COMM);
        if (myid == 0)
            printf (“valor aproximado de pi: %.16f \n”, pi);
    }
    MPI_Finalize( );
}
```

### Implementações de MPI
Muitas implementações de MPI foram desenvolvidas. Todas elas foram implementadas sobre uma máquina virtual formada por muitos computadores heterogêneos (workstations, PCs, Multiprocessadores), em cada um dos quais se executa um processo, usando-se um controle de mensagem entre essas máquinas. Entre essas implementações, o `MPICH` foi desenvolvido pela **Argonne National Laboratory**, utilizando-se MPI 1.1. padrão. Esta implementação está disponível e pode ser obtido no endereço http://www-unix.mcs.anl.gov/mpi/mpich.

Outra implementação disponível de MPI é o `LAM` que pode ser obtida no endereço http://www.mpi.nd.edu/lam, baseada no MPI 2.0 padrão e desenvolvida pelo laboratório de **Computação Cientifica da Universidade de Notre Dame**. Essa implementação fornece algumas funções de monitoramento, as quais são muito úteis para administrar processos.

###### Outras implementações:
- IBM MPI: Implementação IBM para SP e clusters
- UNIFY: Mississipi State University
- CHIMP: Edinburgh Parallel Computing Center
- PMPIO: NASA
- MPIX: Mississippi State University NSF Engineering Research Center

#### Referências
https://www.scielo.br/j/pope/a/mLv97ppcXCbVYbv9JHgkK4B/?lang=pt#
https://www.lncc.br/~borges/ist/SO2/trabalhos/Introducao%20ao%20MPI.pdf
