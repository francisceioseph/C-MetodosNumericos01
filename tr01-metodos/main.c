#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00000001

double** alocaMatriz(int l, int c);
void imprimeMatriz(double **m, int l, int c);
void desalocaMatriz(double  **m, int l);

void pivotacao(double **m, int n,int v[]);
int procuraMaior(double *m, int tam);
void inicializaMatrizDoArquivo(char *filename, double **m);
int* alocaVetorPosicaoColunas(int n);
int sretro(double **m,int n,double x[]);

int main()
{
    int tam = obtemTamanhoMatriz("numbers2.txt");
    double **m = alocaMatriz(tam, tam + 1);
    int *v = alocaVetorPosicaoColunas(tam);

    inicializaMatrizDoArquivo("numbers2.txt", m);
    //imprimeMatriz(m, tam, tam + 1);

    printf("\n\n\n");

    pivotacao(m, tam,v);
    imprimeMatriz(m, tam, tam + 1);

    return 0;
}

void pivotacao(double **m, int n,int v [])
{

    /*
     * Esta função implementa o metodo da pivotação completa
     *
     * Parametros:
     * **m : uma ponteiro para uma matriz de doubles
     * n   : tamanho da matriz m
     *
     * Variaveis:
     * i, j, k : contadores
     *
     * maior   : maior elemento em uma linha pivotal
     * lm      : linha do maior elemento em uma linha pivotal
     * cm      : coluna do maior elemento de uma linha pivotal
     *
     * *aux    : ponteiro para uma linha da matriz estentendida m
     *
     * mult    : multiplicador a ser usado nas linhas a serem pivotadas
     *
     */

    int i, j, k;



    double maior = m[0][0];
    int lm = 0;
    int cm = 0;
    int lt = 0;
    int p = 0;

    double *aux;

    double aux_troca;

    double mult;

    for (i = 0; i < n; i++)
    {
        maior = fabs(m[i][0]);
        lm = i;
        cm = 0;

        //Procura o maior elemento das linhas ainda
        //não pivotadas da matriz de coeficientes.

        for (j = i; j < n; j++)
        {
            for(k = 0; k < n; k++)
            {
                if (fabs(m[j][k]) > fabs(maior))
                {
                    maior = m[j][k];
                    lm = j;
                    cm = k;
                }
            }
        }

        //Troca a linha i com a linha do maior.

        aux   = m[i];
        m[i]  = m[lm];
        m[lm] = aux;

        lm = i;


        //Realiza a pivotação das linhas abaixo da linha i

        for(j = i+1; j < n; j++)
        {
            mult = -m[j][cm] / m[lm][cm];
            m[j][cm] = 0;

            for(k = 0; k <= n; k++)
            {
                if(k != cm)
                {
                    m[j][k] += m[lm][k] * mult;
                }
            }
        }

        //printf("pivot = %10.8lf\n", maior);
        //imprimeMatriz(m, n, n+1);
        printf("\n");

        if(i!=cm){
            for(lt=0;lt<n;lt++){
                aux_troca  = m[lt][cm];
                m[lt][cm] = m[lt][i];
                m[lt][i]  = aux_troca;
            }

            v[i] = cm;

        }
    }

    resolveMatrizTS(m,n,v);
}

int sretro(double **m,int n,double x[]){
    /*Resolve um SL TS com n variaveis cuja matriz aumentada e m.
    * Se o SL for determinado, x recebe a soluсao do SL e a funсao devolve 0;
    * Se for indeterminado x recebe uma das infinitas soluушes do SL e a funсao devolve 1;
    * Se for incompativel a funсao devolve 2;
    */

    int i,k, tipo=0;
    double soma;

    for(i=n-1;i>=0;i--){
        soma =0;
        for(k=i+1;k<n;k++){
            soma += m[i][k]*x[k];
        }

        if(fabs(m[i][i])<EPSILON){

          if(fabs(m[i][n]-soma)<EPSILON){
                /*x[i] e variрvel livre*/
                x[i]=0;
                tipo = 1;
            }else{
                return 2; /*SL incompatьvel*/
            }
        }else{
            x[i]=(m[i][n]-soma)/m[i][i];
        }
    }
    return tipo;
}/*Fim sretro*/

void resolveMatrizTS(double **m,int n, int v []){
    double *x;

    int tipo,i;

    x = malloc(sizeof(double)*n);
    if(x==NULL){
        printf("Erro ao alocar Matriz. Falta de memoria!\n");
        return 1;
    }

    tipo = sretro(m,n,x);

    printf("O SL e %s\n", tipo==0?"Determinado":tipo==1?"Indeterminado":"Incompativel");

    if(tipo!=2){
        for(i=0;i<n;i++){
            printf("x[%d] = %10.3lf ", v[i]+1,x[i]);
        }
        printf("\n\n");
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

int* alocaVetorPosicaoColunas(int n){
     int *v = malloc(sizeof(int) * n);
     return v;
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

