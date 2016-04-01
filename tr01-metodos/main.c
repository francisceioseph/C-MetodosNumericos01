#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00000001

double** alocaMatriz(int l, int c);
void imprimeMatriz(double **m, int l, int c);
void desalocaMatriz(double  **m, int l);

int main()
{
    double** c = alocaMatriz(5, 5);

    desalocaMatriz(c, 5);

    return 0;
}

// Funcoes genericos de manipulação de matrizes

void imprimeMatriz(double **m, int l, int c) {
    int i, j;

    for (i = 0; i < l; i++){
        for (j = 0; j < c; j++){
            printf("%10.3lf ", m[i][j]);
        }

        printf("\n");
    }
}

double** alocaMatriz(int l, int c) {
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

    if (m != NULL) {
        for (i = 0; i < l; i++) {
            m[i] = malloc(sizeof(double) * c);

            if (m[i] == NULL) {

                for (j = 0; j < i; i++) {
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

    if(m != NULL){

        for (i = 0; i < l; i++){
            free(m[i]);
        }

        free(m);
    }
}

// Funcoes para implementar o metodo da pivotacao completa.

void lerMatrizDoArquivo(char *filename) {

}
