#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

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

// Implementação do Bubble Sort paralelo com OpenMP
void bubbleSortParallel(int* vetor, int size, int num_threads) {
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (vetor[j] > vetor[j + 1]) {
                    trocar(&vetor[j], &vetor[j + 1]);
                }
            }
        }
    }
}

int main() {
    int vetor[SIZE];
    preencherVetor(vetor, SIZE);

    double start_time, end_time, serial_time, parallel_time;
    int num_threads;

    // Ordenação serial
    start_time = omp_get_wtime();
    bubbleSortSerial(vetor, SIZE);
    end_time = omp_get_wtime();
    serial_time = end_time - start_time;
    printf("Tempo de execução serial: %.6f segundos\n", serial_time);

    // Ordenação paralela com 1 thread
    //num_threads = 1;
    //start_time = omp_get_wtime();
    //bubbleSortParallel(vetor, SIZE, num_threads);
    //end_time = omp_get_wtime();
    //parallel_time = end_time - start_time;
    //printf("Tempo de execução paralela (%d thread): %.6f segundos\n", num_threads, parallel_time);
    //printf("Speedup (%d thread): %.6f\n", num_threads, serial_time / parallel_time);
    //printf("Eficiência (%d thread): %.6f\n", num_threads, (serial_time / parallel_time) / num_threads);

    // Ordenação paralela com 2 threads
    //num_threads = 2;
    //tart_time = omp_get_wtime();
    //bubbleSortParallel(vetor, SIZE, num_threads);
    //end_time = omp_get_wtime();
    //parallel_time = end_time - start_time;
    //printf("Tempo de execução paralela (%d threads): %.6f segundos\n", num_threads, parallel_time);
    //printf("Speedup (%d threads): %.6f\n", num_threads, serial_time / parallel_time);
    //printf("Eficiência (%d threads): %.6f\n", num_threads, (serial_time / parallel_time) / num_threads);

    // Ordenação paralela com 4 threads
    num_threads = 4;
    start_time = omp_get_wtime();
    bubbleSortParallel(vetor, SIZE, num_threads);
    end_time = omp_get_wtime();
    parallel_time = end_time - start_time;
    printf("Tempo de execução paralela (%d threads): %.6f segundos\n", num_threads, parallel_time);
    printf("Speedup (%d threads): %.6f\n", num_threads, serial_time / parallel_time);
    printf("Eficiência (%d threads): %.6f\n", num_threads, (serial_time / parallel_time) / num_threads);

    return 0;
}