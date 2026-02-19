#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 10000

void merge(int v1[], int n1, int v2[], int n2, int dest[]) {
    int i1 = 0, i2 = 0, i = 0;
    while (i1 < n1 && i2 < n2) {
        if (v1[i1] < v2[i2]) {
            dest[i] = v1[i1];
            i1++;
        } else {
            dest[i] = v2[i2];
            i2++;
        }
        i++;
    }
    while (i1 < n1) {
        dest[i] = v1[i1];
        i1++;
        i++;
    }
    while (i2 < n2) {
        dest[i] = v2[i2];
        i2++;
        i++;
    }
}

void merge_sort(int v[], int inicio, int fim) {
    if (inicio >= fim) return;

    int meio = (inicio + fim) / 2;
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    int v1[n1], v2[n2];
    for (int i = 0; i < n1; i++) v1[i] = v[inicio + i];
    for (int i = 0; i < n2; i++) v2[i] = v[meio + 1 + i];

    merge_sort(v1, 0, n1 - 1);
    merge_sort(v2, 0, n2 - 1);
    merge(v1, n1, v2, n2, v + inicio);
}

void trocar(int v[], int i, int j) {
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int escolher_pivo(int v[], int inicio, int fim) {
    return (inicio + fim) / 2;
}

int particionar(int v[], int inicio, int fim, int indice_pivo) {
    int pivo = v[indice_pivo];
    trocar(v, indice_pivo, fim);
    int esquerda = inicio;
    int direita = fim - 1;

    while (esquerda <= direita) {
        while (v[esquerda] < pivo) esquerda++;
        while (v[direita] > pivo) direita--;
        if (esquerda <= direita) {
            trocar(v, esquerda, direita);
            esquerda++;
            direita--;
        }
    }
    trocar(v, esquerda, fim);
    return esquerda;
}

void quick_sort(int v[], int inicio, int fim) {
    if (inicio >= fim) return;

    int indice_pivo = escolher_pivo(v, inicio, fim);
    int nova_posicao_pivo = particionar(v, inicio, fim, indice_pivo);

    quick_sort(v, inicio, nova_posicao_pivo - 1);
    quick_sort(v, nova_posicao_pivo + 1, fim);
}

int main() {
    int vmerge[TAM], vquick[TAM];
    int i;
    clock_t inicio, fim;
    double tempo;

    srand(time(NULL));

    for (i = 0; i < TAM; i++) {
        vmerge[i] = rand() % 10000;
        vquick[i] = vmerge[i]; 
    }

    inicio = clock();
    merge_sort(vmerge, 0, TAM - 1);
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo MergeSort: %.5f segundos\n", tempo);

    inicio = clock();
    quick_sort(vquick, 0, TAM - 1);
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo QuickSort: %.5f segundos\n", tempo);

    return 0;
}