#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define SIZE 1000000

// Função para preencher o vetor com números aleatórios
void preencherVetor(int* vetor, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        vetor[i] = rand() % 100001;
    }
}

// Função para imprimir o vetor
void imprimirVetor(int* vetor, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

// Função de troca de elementos
void trocar(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Implementação do Bubble Sort serial
void bubbleSortSerial(int* vetor, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (vetor[j] > vetor[j + 1]) {
                trocar(&vetor[j], &vetor[j + 1]);
            }
        }
    }
}

// Implementação do Bubble Sort com OpenMPI
void bubbleSortParallel(int* vetor, int size, int rank, int num_procs) {
    int chunk_size = size / num_procs;
    int* chunk = (int*)malloc(chunk_size * sizeof(int));

    // Distribuir o vetor entre os processos
    MPI_Scatter(vetor, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Ordenar o chunk localmente
    bubbleSortSerial(chunk, chunk_size);

    // Juntar os chunks ordenados
    int* sorted = NULL;
    if (rank == 0) {
        sorted = (int*)malloc(size * sizeof(int));
    }
    MPI_Gather(chunk, chunk_size, MPI_INT, sorted, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Ordenar o vetor completo no processo raiz
    if (rank == 0) {
        bubbleSortSerial(sorted, size);
        // Agora o vetor completo está ordenado em 'sorted'
        // Faça o que for necessário com ele
        // ...

        free(sorted);
    }

    free(chunk);
}

int main(int argc, char** argv) {
    int rank, num_procs;
    double start_time, end_time, serial_time, parallel_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int vetor[SIZE];
    preencherVetor(vetor, SIZE);

    // Ordenação serial
    if (rank == 0) {
        start_time = MPI_Wtime();
        bubbleSortSerial(vetor, SIZE);
        end_time = MPI_Wtime();
        serial_time = end_time - start_time;
        printf("Tempo de execução serial: %.6f segundos\n", serial_time);
    }

    // Sincronizar os processos
    MPI_Barrier(MPI_COMM_WORLD);

    // Ordenação paralela
    start_time = MPI_Wtime();
    bubbleSortParallel(vetor, SIZE, rank, num_procs);
    end_time = MPI_Wtime();
    parallel_time = end_time - start_time;

    // Tempo total de execução
    double total_time;
    MPI_Reduce(&parallel_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Tempo de execução paralela: %.6f segundos\n", total_time);
        printf("Speedup: %.6f\n", serial_time / total_time);
    }

    MPI_Finalize();
    return 0;
}
