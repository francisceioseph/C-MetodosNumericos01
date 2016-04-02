#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00000001

double** alocaMatriz(int l, int c);
void imprimeMatriz(double **m, int l, int c);
void desalocaMatriz(double  **m, int l);

void pivotacao(double **m, int n);
int procuraMaior(double *m, int tam);
void inicializaMatrizDoArquivo(char *filename, double **m);

int main()
{
    int tam = obtemTamanhoMatriz("numbers.txt");
    double **m = alocaMatriz(tam, tam + 1);

    inicializaMatrizDoArquivo("numbers.txt", m);
    imprimeMatriz(m, tam, tam + 1);

    printf("\n\n\n");

    pivotacao(m, tam);
    imprimeMatriz(m, tam, tam + 1);

    return 0;
}

void pivotacao(double **m, int n)
{

    // Esta função implementa o metodo da pivotação completa

    int i, j, k;
    int indicePivo;
    double mult;

    for (i = 0; i < n; i++)
    {

        indicePivo = procuraMaior(m[i], n);

        if (fabs(m[i][indicePivo]) > EPSILON)
        {
            for (j = i + 1; j < n; j++)
            {
                mult = - m[j][indicePivo]/m[i][indicePivo];
                m[j][indicePivo] = 0;

                for (k = 0; k<=n; k++)
                {
                    if (k != indicePivo)
                    {
                        m[j][k] += m[i][k] * mult;
                    }
                }
            }
        }
    }
}


int procuraMaior(double *m, int tam)
{

    /* Dado um vetor, esta função procura o maior elemento
     * os indices 0 e tam - 1.
     * A função retorna o indice o maior elemento.
     */

    int i;

    int indiceMaior = 0;
    double maior = m[0];

    for(i = 1; i < tam; i++)
    {
        if(m[i] > maior)
        {
            maior = m[i];
            indiceMaior = i;
        }
    }

    return indiceMaior;
}

void inicializaMatrizDoArquivo(char *filename, double **m)
{
    int tam;
    int i, j;
    double buffer;

    FILE *file = fopen(filename, "r");

    if (file != NULL)
    {
        fscanf(file, "%d", &tam);

        for (i = 0; i < tam; i++)
        {
            for (j = 0; j < tam + 1; j ++)
            {
                fscanf(file, "%lf", &buffer);
                m[i][j] = buffer;
            }

            printf("\n");
        }

        fclose(file);

    }
}

int obtemTamanhoMatriz(char *nomeArquivo)
{

    int tam = 0;
    FILE *file = fopen(nomeArquivo, "r");

    if (file != NULL)
    {
        fscanf(file, "%d", &tam);
        fclose(file);
    }

    return tam;

}

// Funcoes genericos de manipulação de matrizes

void imprimeMatriz(double **m, int l, int c)
{
    int i, j;

    for (i = 0; i < l; i++)
    {
        for (j = 0; j < c; j++)
        {
            printf("%12.8lf ", m[i][j]);
        }

        printf("\n");
    }
}

double** alocaMatriz(int l, int c)
{
    /*
     * Se houver memoria disponivel, alocal uma matriz com
     * de tamanho l por c. E devolve o endereco
     * base da matriz. Caso contrário, ele devolverá um
     * ponteiro nulo.
     *
     */

    int i, j;
    double **m;

    m = malloc(sizeof(double*) * l);

    if (m != NULL)
    {
        for (i = 0; i < l; i++)
        {
            m[i] = malloc(sizeof(double) * c);

            if (m[i] == NULL)
            {
                for (j = 0; j < i; i++)
                {
                    free(m[j]);
                }

                free(m);
                return NULL;
            }
        }
    }

    return m;
}

void desalocaMatriz(double **m, int l)
{
    /*
     * Desaloca uma matriz previamente alocada
     * com a função aloca_matriz.
     */

    int i;

    if(m != NULL)
    {

        for (i = 0; i < l; i++)
        {
            free(m[i]);
        }

        free(m);
    }
}
