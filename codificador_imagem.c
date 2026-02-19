#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LARGURA 1024
#define MAX_ALTURA 768

void printHelp();
int ler_imagem_manual(int imagem[MAX_ALTURA][MAX_LARGURA], int *num_linhas, int *num_colunas);
int ler_imagem_arquivo(const char *nome_arquivo, int imagem[MAX_ALTURA][MAX_LARGURA], int *num_linhas, int *num_colunas);
void codificar_imagem(int imagem[MAX_ALTURA][MAX_LARGURA], int inicio_linha, int fim_linha, int inicio_coluna, int fim_coluna);
int imagem_uniforme(int imagem[MAX_ALTURA][MAX_LARGURA], int inicio_linha, int fim_linha, int inicio_coluna, int fim_coluna);

int main(int argc, char *argv[]) {

    int imagem[MAX_ALTURA][MAX_LARGURA];
    int largura_atual = 0;
    int altura_atual = 0;
    int carregou_certo = 0;

    if (argc < 2) {
        printHelp();
        return 0;
    }

    if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0) {
        printHelp();
        return 0;
    }

    if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--manual") == 0) {
        carregou_certo = ler_imagem_manual(imagem, &altura_atual, &largura_atual);
    }
    else if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0) {

        if (argc < 3) {
            printf("Erro: faltou colocar o nome do arquivo depois do -f.\n");
            printHelp();
            return 1;
        }

        carregou_certo = ler_imagem_arquivo(argv[2], imagem, &altura_atual, &largura_atual);
    }
    else {
        printf("Erro: opcao '%s' invalida.\n", argv[1]);
        printHelp();
        return 1;
    }

    if (!carregou_certo) {
        printf("Nao foi possivel carregar a imagem.\n");
        return 1;
    }

    printf("\nCodigo gerado: ");
    codificar_imagem(imagem, 0, altura_atual - 1, 0, largura_atual - 1);
    printf("\n\nCodificacao concluida.\n");

    return 0;
}

void printHelp() {
    printf("Uso: ImageEncoder [-? | -m | -f ARQUIVO]\n");
    printf("Codifica imagens binarias usando PBM ou entrada manual.\n");
    printf(" -?, --help   : mostra ajuda.\n");
    printf(" -m, --manual : modo manual.\n");
    printf(" -f, --file   : ler imagem de um arquivo PBM.\n");
}

int ler_imagem_manual(int imagem[MAX_ALTURA][MAX_LARGURA], int *num_linhas, int *num_colunas) {
    printf("\n--- Modo Manual ---\n");

    printf("Digite a largura: ");
    scanf("%d", num_colunas);

    printf("Digite a altura: ");
    scanf("%d", num_linhas);

    if (*num_colunas <= 0 || *num_linhas <= 0 || *num_colunas > MAX_LARGURA || *num_linhas > MAX_ALTURA) {
        printf("Erro: dimensoes informadas nao sao validas.\n");
        return 0;
    }

    printf("Digite os pixels (0 para branco ou 1 para preto):\n");

    for (int linha_atual = 0; linha_atual < *num_linhas; linha_atual++) {
        for (int coluna_atual = 0; coluna_atual < *num_colunas; coluna_atual++) {
            scanf("%d", &imagem[linha_atual][coluna_atual]);
        }
    }

    return 1;
}

int ler_imagem_arquivo(const char *nome_arquivo, int imagem[MAX_ALTURA][MAX_LARGURA], int *num_linhas, int *num_colunas) {

    FILE *arquivo;
    char buffer[200];
    int largura_lida, altura_lida;

    printf("\n--- Modo Arquivo ---\n");

    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo %s\n", nome_arquivo);
        return 0;
    }

    fscanf(arquivo, "%s", buffer);
    if (strcmp(buffer, "P1") != 0) {
        printf("Erro: arquivo nao é do tipo P1.\n");
        fclose(arquivo);
        return 0;
    }

    char caractere = fgetc(arquivo);
    while (caractere==' ' || caractere=='\n' || caractere=='\t' || caractere=='\r') {
        caractere = fgetc(arquivo);
    }

    if (caractere=='#') {
        fgets(buffer, 200, arquivo);
    } else {
        ungetc(caractere, arquivo);
    }

    if (fscanf(arquivo, "%d %d", &largura_lida, &altura_lida) != 2) {
        printf("Erro ao ler largura e altura.\n");
        fclose(arquivo);
        return 0;
    }

    *num_colunas = largura_lida;
    *num_linhas  = altura_lida;

    if (*num_colunas > MAX_LARGURA || *num_linhas > MAX_ALTURA) {
        printf("Erro: dimensoes %dx%d excedem o maximo permitido.\n", largura_lida, altura_lida);
        fclose(arquivo);
        return 0;
    }

    for (int linha_atual = 0; linha_atual < *num_linhas; linha_atual++) {
        for (int coluna_atual = 0; coluna_atual < *num_colunas; coluna_atual++) {
            if (fscanf(arquivo, "%d", &imagem[linha_atual][coluna_atual]) != 1) {
                printf("Erro: faltam dados de pixel no arquivo.\n");
                fclose(arquivo);
                return 0;
            }
        }
    }

    fclose(arquivo);
    return 1;
}

int imagem_uniforme(int imagem[MAX_ALTURA][MAX_LARGURA], int inicio_linha, int fim_linha, int inicio_coluna, int fim_coluna) {

    int cor_referencia = imagem[inicio_linha][inicio_coluna];

    for (int linha_atual = inicio_linha; linha_atual <= fim_linha; linha_atual++) {
        for (int coluna_atual = inicio_coluna; coluna_atual <= fim_coluna; coluna_atual++) {
            if (imagem[linha_atual][coluna_atual] != cor_referencia) {
                return -1;
            }
        }
    }

    return cor_referencia;
}

void codificar_imagem(int imagem[MAX_ALTURA][MAX_LARGURA], int inicio_linha, int fim_linha, int inicio_coluna, int fim_coluna) {

    int cor_encontrada = imagem_uniforme(imagem, inicio_linha, fim_linha, inicio_coluna, fim_coluna);

    if (cor_encontrada == 0) {
        printf("B");
        return;
    } 
    else if (cor_encontrada == 1) {
        printf("P");
        return;
    }

    printf("X");

    int meio_linha = (inicio_linha + fim_linha) / 2;
    int meio_coluna = (inicio_coluna + fim_coluna) / 2;

    codificar_imagem(imagem, inicio_linha, meio_linha, inicio_coluna, meio_coluna);

    if (meio_coluna + 1 <= fim_coluna)
        codificar_imagem(imagem, inicio_linha, meio_linha, meio_coluna + 1, fim_coluna);

    if (meio_linha + 1 <= fim_linha)
        codificar_imagem(imagem, meio_linha + 1, fim_linha, inicio_coluna, meio_coluna);

    if (meio_linha + 1 <= fim_linha && meio_coluna + 1 <= fim_coluna)
        codificar_imagem(imagem, meio_linha + 1, fim_linha, meio_coluna + 1, fim_coluna);
}
