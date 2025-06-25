
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define TAMANHO_TABULEIRO 10
#define TAMANHO_PADRAO 5

#define AGUA 0
#define NAVIO 3
#define HABILIDADE_CONE 5
#define HABILIDADE_CRUZ 7
#define HABILIDADE_OCTAEDRO 9

// Função para limpar o tabuleiro
void limparTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Função para imprimir o tabuleiro
void imprimirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\n   ");
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (visivel[i][j] == 0) {
                printf(" ~ ");  // Posição oculta (água não atingida)
            } else {
                switch (tabuleiro[i][j]) {
                    case NAVIO: printf(" 3 "); break;
                    case HABILIDADE_CONE: printf(" 5 "); break;
                    case HABILIDADE_CRUZ: printf(" 7 "); break;
                    case HABILIDADE_OCTAEDRO: printf(" 9 "); break;
                    default: printf(" 0 "); break; // Água revelada
                }
            }
        }
        printf("\n");
    }
}

// Função para criar padrão de habilidade
void criarPadrao(int matriz[TAMANHO_PADRAO][TAMANHO_PADRAO], const char *tipo) {
    int centro = TAMANHO_PADRAO / 2;
    for (int i = 0; i < TAMANHO_PADRAO; i++)
        for (int j = 0; j < TAMANHO_PADRAO; j++)
            matriz[i][j] = 0;

    if (strcmp(tipo, "cone") == 0) {
        for (int i = 0; i <= centro; i++)
            for (int j = centro - i; j <= centro + i; j++)
                matriz[i + centro][j] = 1;
    } else if (strcmp(tipo, "cruz") == 0) {
        for (int i = 0; i < TAMANHO_PADRAO; i++) {
            matriz[centro][i] = 1;
            matriz[i][centro] = 1;
        }
    } else if (strcmp(tipo, "octaedro") == 0) {
        for (int i = 0; i < TAMANHO_PADRAO; i++)
            for (int j = 0; j < TAMANHO_PADRAO; j++)
                if (abs(i - centro) + abs(j - centro) <= 2)
                    matriz[i][j] = 1;
    }
}

// Função para aplicar habilidade no tabuleiro
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                       int centroX, int centroY, int padrao[TAMANHO_PADRAO][TAMANHO_PADRAO], int valorHabilidade) {
    int offset = TAMANHO_PADRAO / 2;

    for (int i = 0; i < TAMANHO_PADRAO; i++) {
        for (int j = 0; j < TAMANHO_PADRAO; j++) {
            int x = centroX + (j - offset);
            int y = centroY + (i - offset);

            if (x >= 0 && x < TAMANHO_TABULEIRO && y >= 0 && y < TAMANHO_TABULEIRO) {
                if (padrao[i][j] == 1) {
                    visivel[y][x] = 1;
                    if (tabuleiro[y][x] == AGUA) {
                        tabuleiro[y][x] = valorHabilidade;
                    }
                }
            }
        }
    }
}

// Função para posicionar navios
void posicionarNavios(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int quantidade;
    printf("\nQuantos navios deseja posicionar? ");
    scanf("%d", &quantidade);

    for (int i = 0; i < quantidade; i++) {
        int x, y;
        int valido = 0;
        while (!valido) {
            printf("Posicione o navio %d (linha coluna): ", i + 1);
            scanf("%d %d", &y, &x);

            if (x >= 0 && x < TAMANHO_TABULEIRO && y >= 0 && y < TAMANHO_TABULEIRO) {
                if (tabuleiro[y][x] == AGUA) {
                    tabuleiro[y][x] = NAVIO;
                    valido = 1;
                } else {
                    printf("Já existe algo nessa posição.\n");
                }
            } else {
                printf("Posição inválida.\n");
            }
        }
    }
}

// Menu para escolha de habilidades
void menuHabilidades(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int opcao;
    do {
        printf("\n--- Menu de Habilidades ---\n");
        printf("1. Cone\n");
        printf("2. Cruz\n");
        printf("3. Octaedro\n");
        printf("4. Sair\n");
        printf("Escolha uma habilidade: ");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= 3) {
            int x, y;
            printf("Informe as coordenadas (linha coluna): ");
            scanf("%d %d", &y, &x);

            if (x >= 0 && x < TAMANHO_TABULEIRO && y >= 0 && y < TAMANHO_TABULEIRO) {
                int padrao[TAMANHO_PADRAO][TAMANHO_PADRAO];
                const char *nome = (opcao == 1) ? "cone" : (opcao == 2) ? "cruz" : "octaedro";
                int valor = (opcao == 1) ? HABILIDADE_CONE : (opcao == 2) ? HABILIDADE_CRUZ : HABILIDADE_OCTAEDRO;

                criarPadrao(padrao, nome);
                aplicarHabilidade(tabuleiro, visivel, x, y, padrao, valor);
                imprimirTabuleiro(tabuleiro, visivel);
            } else {
                printf("Coordenadas inválidas.\n");
            }
        } else if (opcao != 4) {
            printf("Opção inválida.\n");
        }

    } while (opcao != 4);
}

// Programa principal
int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO] = {0};

    limparTabuleiro(tabuleiro);

    printf("***** Batalha Naval - MateCheck *****\n");

    imprimirTabuleiro(tabuleiro, visivel);
    posicionarNavios(tabuleiro);
    imprimirTabuleiro(tabuleiro, visivel);
    menuHabilidades(tabuleiro, visivel);

    printf("\nTabuleiro final:\n");
    imprimirTabuleiro(tabuleiro, visivel);

    return 0;
}

// Desafio Batalha Naval - MateCheck
// Este código inicial serve como base para o desenvolvimento do sistema de Batalha Naval.
// Siga os comentários para implementar cada parte do desafio.

    // Nível Novato - Posicionamento dos Navios
    // Sugestão: Declare uma matriz bidimensional para representar o tabuleiro (Ex: int tabuleiro[5][5];).
    // Sugestão: Posicione dois navios no tabuleiro, um verticalmente e outro horizontalmente.
    // Sugestão: Utilize `printf` para exibir as coordenadas de cada parte dos navios.

    // Nível Aventureiro - Expansão do Tabuleiro e Posicionamento Diagonal
    // Sugestão: Expanda o tabuleiro para uma matriz 10x10.
    // Sugestão: Posicione quatro navios no tabuleiro, incluindo dois na diagonal.
    // Sugestão: Exiba o tabuleiro completo no console, mostrando 0 para posições vazias e 3 para posições ocupadas.

    // Nível Mestre - Habilidades Especiais com Matrizes
    // Sugestão: Crie matrizes para representar habilidades especiais como cone, cruz, e octaedro.
    // Sugestão: Utilize estruturas de repetição aninhadas para preencher as áreas afetadas por essas habilidades no tabuleiro.
    // Sugestão: Exiba o tabuleiro com as áreas afetadas, utilizando 0 para áreas não afetadas e 1 para áreas atingidas.

    // Exemplos de exibição das habilidades:
    // Exemplo para habilidade em cone:
    // 0 0 1 0 0
    // 0 1 1 1 0
    // 1 1 1 1 1
    
    // Exemplo para habilidade em octaedro:
    // 0 0 1 0 0
    // 0 1 1 1 0
    // 0 0 1 0 0

    // Exemplo para habilidade em cruz:
    // 0 0 1 0 0
    // 1 1 1 1 1
    // 0 0 1 0 0


