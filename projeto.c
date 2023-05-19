#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100000 //valores de 0 a 100.000
#define TAMANHO_VETOR 100000000 //tamanho do vetor 100 milhoes

int *gerar_vetor(int tamanho);
void mostrar_vetor(int *v, int tamanho);
void bubbleSort(int *v, int tamanho);
void bubbleSortParallel(int *v, int tamanho);

int main() {
    int *vetor;
    int i, j, temp;
    double fimParalelo, fimSerial, inicioParalelo, inicioSerial, speedup, eficiencia, tempoSerial, tempoParalelo;

    vetor = (int *)malloc(TAMANHO_VETOR * sizeof(int));

    if (vetor == NULL) {
        printf("Erro ao alocar memória\n");
        return 1;
    }

    vetor = gerar_vetor(TAMANHO_VETOR);

    //printf("Vetor original: ");
    //mostrar_vetor(vetor, TAMANHO_VETOR);

    int *vetorOriginal = vetor;
    int *vetorOriginal2 = vetor;
    inicioSerial = omp_get_wtime();

    for(i = 0; i < TAMANHO_VETOR - 1; i++) {
        for (j = 0; j < TAMANHO_VETOR - i - 1; j++) {
            if (vetor[j] > vetor[j + 1]) {
                // Troca os elementos
                temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }

    fimSerial = omp_get_wtime();

    tempoSerial = fimSerial - inicioSerial;

    //printf("Vetor ordenado: ");
    //mostrar_vetor(vetor, TAMANHO_VETOR);

    inicioParalelo = omp_get_wtime();

    #pragma omp parallel num_threads(3)
    {
        #pragma omp for private(i)
        for (i = 0; i < TAMANHO_VETOR - 1; i++){
            for (j = 0; j < TAMANHO_VETOR - i - 1; j++){
                if (vetorOriginal[j] > vetorOriginal[j + 1]){
                    // Troca os elementos
                    temp = vetorOriginal[j];
                    vetorOriginal[j] = vetorOriginal[j + 1];
                    vetorOriginal[j + 1] = temp;
                }
            }
        }
    }

    fimParalelo = omp_get_wtime();

    //printf("Vetor ordenado: ");
    //mostrar_vetor(vetorOriginal, TAMANHO_VETOR);

    tempoParalelo = fimParalelo - inicioParalelo;

    speedup = tempoSerial / tempoParalelo;
    eficiencia = speedup / 3.0;

    printf("Tempo serial: %.6f segundos\n", tempoSerial);
    printf("Tempo paralelo: %.6f segundos\n", tempoParalelo);
    printf("Speedup: %.2f\n", speedup);
    printf("Eficiência: %.2f\n", eficiencia);

    return 0;
}

int *gerar_vetor(int n) {
    int *vetor;
    int i;
    vetor = (int *)malloc(sizeof(int) * n);
    for (i=0;i<n;i++) {
        int num = (rand() % MAX);
        vetor[i] = num;
    }
    return vetor;
}


void mostrar_vetor(int *v,int tamanho) {
    int i;
    for (i=0;i<tamanho;i++) {
        printf("[%d]",v[i]);
    }
    printf("\n");
}
