#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100000 //valores de 0 a 100.000
#define TAMANHO_VETOR 100000 //tamanho do vetor 100 milhoes

int *gerar_vetor(int tamanho);
void mostrar_vetor(int *v, int tamanho);

int main() {
    int *vetor;
    int i, j, temp;
    double fimParalelo, fimSerial, inicioParalelo, inicioSerial, speedup, eficiencia, tempoSerial, tempoParalelo;
    vetor = (int *)malloc(TAMANHO_VETOR * sizeof(int)); //alocando vetor na memoria
    if (vetor == NULL) {
        printf("Erro ao alocar memória\n");
        return 1;
    }

    printf("gerando vetor!");
    vetor = gerar_vetor(TAMANHO_VETOR);
    //printf("vetor gerado!");

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
    //printf("Vetor ordenado: ");
    //mostrar_vetor(vetorOriginal, TAMANHO_VETOR);

    printf("Tempo serial: %.6f segundos\n", tempoSerial);
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