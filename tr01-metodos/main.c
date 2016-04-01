#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00000001

double** aloca_matriz(int l, int c);
void imprime_matriz(double **m, int l, int c);
void desaloca_matriz(double  **m, int l);

int main()
{
    double** c = aloca_matriz(5, 5);

    desaloca_matriz(c, 5);

    return 0;
}

void imprime_matriz(double **m, int l, int c) {
    int i, j;

    for (i = 0; i < l; i++){
        for (j = 0; j < c; j++){
            printf("%10.3lf ", m[i][j]);
        }

        printf("\n");
    }
}

double** aloca_matriz(int l, int c) {
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

void desaloca_matriz(double **m, int l) {
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
