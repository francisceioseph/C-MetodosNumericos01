#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSILON 0.00000001

double** alocaMatriz(int l, int c);
void imprimeMatriz(double **m, int l, int c);
void desalocaMatriz(double  **m, int l);

int linha;

int main()
{
    char nome[50];
    printf("Nome do arquivo = ");
    scanf("%s",nome);

    double** c = alocaMatriz(3, 4);

    lerMatrizDoArquivo(c, nome);

    desalocaMatriz(c, 3);

    system("pause");
    return 0;
}

void lerMatrizDoArquivo(double **m, char *filename[]){
     /*lê valores de um arquivo para uma matriz de inteiros com l linhas e c colunas*/
    //char url = filename[1];
    char ch;

    FILE *arq = fopen(filename,"r");
    int i=-1;
    int j=-1;

    if(arq==NULL){
        printf("Erro ao abrir arquivo!\n");
    }else{

        ch=fgetc(arq);
        linha = ch-48; //converte o caractere ch para inteiro

        printf("Ordem da matriz = %d\n",linha);

        while( (ch=fgetc(arq))!= EOF ){

            if(ch!=' ' && ch!='\n'){

                m[2][1]=2;  //adicionar elementos a matriz
                j++;
                printf("elemento = %d\n",ch-48);
            }else if(ch=='\n'){

                printf("muda de linha = %d\n",i);
                i++;
                j=0;
            }
        }
    }

    fclose(arq);
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

    m = malloc(sizeof(double*)*l);

    if (m != NULL) {
        for (i = 0; i < l; i++) {
            m[i] = malloc(sizeof(double*)*c);

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

