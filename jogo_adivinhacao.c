#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int dado;
    int n;
    int contador;

    contador = 0;

    srand(time(NULL));

    dado = 1 + rand() % 100;

    printf("Tente adivinhar o número sorteado\n");
    scanf("%d", &n);

    while (n < 1 || n > 100)
    {
        printf("Escolha outro número\n");
        scanf("%d", &n);
    }

    while (contador < 5)
    {
        if (n < dado)
        {
            ++contador;
            if (contador == 5) break;
            printf("Você chutou muito baixo!\n");
            printf("Tente outro número:\n");
            scanf("%d", &n);
        }
        else if (n > dado)
        {
            ++contador;
            if (contador == 5) break;
            printf("Você chutou muito alto!\n");
            printf("Tente outro número:\n");
            scanf("%d", &n);
        }
        else if (dado == n)
        {
            ++contador;
            printf("Parabéns, você acertou!\nNúmero de tentativas: %d\n", contador);
            return 0; 
        }
    }

    if (contador == 5 && n != dado)
    {
        printf("Você excedeu o número de tentativas! O número era %d.\n", dado);
    }

    return 0;
}
