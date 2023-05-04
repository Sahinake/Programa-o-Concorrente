# Programação Concorrente
#### 04/05/2023


Nome: Luma Araujo de Oliveira
Matrícula: 2021007580

## O que são OpenMP tasks?
Quando OpenMP foi criado, o foco principal eram os casos frequentes em HPC (Computação de alto desempenho)... vetores processados com laços “regulares”. Assim, mesmo um simples passeio por uma lista encadeada é bastante difícil nas versões originais de OpenMP.

Para ir além do mundo baseado em vetores, precisamos suportar estruturas de dados e laços além dos tipos básicos. Por isso, foram adicionadas tasks no OpenMP 3.0.

##### Tasks são unidades de trabalho independentes!
#
Uma `OpenMP task` é uma única linha de código ou um bloco estruturado que é imediatamente “escrito” em uma lista de tarefas. Ela é composta de:
- Código para executar
- Dados do ambiente
- Variáveis de controle interno (ICV)

##### As threads executam o trabalho de cada task.
#
O sistema de execução decide quando as tasks serão executadas:
- Elas podem ser atrasadas
- Ou podem ser executadas imediatamente

> Se a cláusula if for usada e o argumento for avaliado como zero, a tarefa será executada
imediatamente, substituindo qualquer outra coisa que o thread esteja fazendo.

## Como utilizar?
Para adicionar a próxima linha de código (ou um bloco de código) à lista de tarefas utilizamos:
```sh
#pragma omp task
```

Você pode criar uma `barreira de tasks` para garantir que elas estarão completas.

```sh
#pragma omp taskwait
```

Pode também utilizar a `barreira de threads` para isso:

```sh
#pragma omp barrier
```
### Exemplos
##### Observe o exemplo a seguir:
#
```sh
omp_set_num_threads(2);
#pragma omp parallel 
{
    #pragma omp task
        printf(“ A”);
    #pragma omp task
        printf(“ B”);
}
```
##### Possíveis resultados:
#
| Execução | Saída |
| :------: | ------ |
| **1** | B A B A |
| **2** | B B A A |
| **3** | B A A B |
| **4** | A B B A |
- Por que não obtemos sempre a mesma saída?
- Por que temos 4 coisas impressas quando temos apenas 2 declarações de impressão em 2 tarefas?

A menos que você faça alguns arranjos especiais, a ordem de execução das diferentes tarefas é indefinida. A segunda resposta é que, na verdade, pedimos as duas threads que cada executasse duas tasks, num total de quatro. Para direcionarmos a task para apenas uma thread, precisamos usar o `#pragma omp single`.
```sh
omp_set_num_threads( 2 );
#pragma omp parallel default(none)
{
    #pragma omp single
    {
        #pragma omp task
            fprintf( “A\n” );
        #pragma omp task
            fprintf( “B\n” );
    }
}
```

### Exemplo Fibonacci
```sh
int fib (int n) {
    int x,y;
    if(n < 2) return n;
    #pragma omp task shared(x)
        x = fib(n-1);
    #pragma omp task shared(y)
        y = fib(n-2);
    #pragma omp taskwait
    return x+y;
}
```
#  Qual a diferença para as OpenMP Sections?
As `tasks` são muito parecidas com as `OpenMP Sections`. O que as diferenciam é que as `tasks` podem ser criadas a qualquer momento e em qualquer número, sob controle da lógica do programa, enquanto as `sections` são estáticas, ou seja, o número de sections é definido quando você escreve o código.

##### Tasks tem melhor desempenho em recursão do que Sections.
Veja a seguinte discussão sobre a diferença entre a utilização de `tasks x sections` no [StackOverflow](https://stackoverflow.com/questions/13788638/difference-between-section-and-task-openmp).



## Referências
https://web.engr.oregonstate.edu/~mjb/cs575/Handouts/tasks.1pp.pdf

