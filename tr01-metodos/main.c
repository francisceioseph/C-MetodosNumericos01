#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define EPSILON 0.00000001

//Metodos utilizados no Sistema Linear
double** alocaMatriz(int l, int c);
void imprimeMatriz(double **m, int l, int c);
void desalocaMatriz(double  **m, int l);

void pivotacao(double **m, int n,int v[]);
int procuraMaior(double *m, int tam);
void inicializaMatrizDoArquivo(char *filename, double **m);
int* alocaVetorPosicaoColunas(int n);
int sretro(double **m,int n,double x[]);
void initPivotacao();
void menu();
int fileExist(char *nomeArquivo);

//Metodo utilizado na Conversao
void initConversao();

int main()
{
    menu();

    return 0;
}

void menu(){

    char opcao;
    int continua = 1;

    while(continua){

        printf("\n\n\t### MENU PRINCIPAL ###\n\n");
        printf("\tC - Convers\706o\n");
        printf("\tS - Sistema Linear\n");
        printf("\tE - Equa\207\706o Alg\202brica\n");
        printf("\tF - Finalizar\n");

        printf("\n\tEscolha uma op\207\706o: ");
        scanf(" %c",&opcao);
        getchar();

        switch( opcao )
        {
            case 'c':
            case 'C':
                 initConversao();
                 break;
            case 's':
            case 'S':
                 initPivotacao();
                 break;

            case 'e':
            case 'E':
                 //adicionar Lagrange aqui
                 break;
            case 'f':
            case 'F':
                 continua = 0;
                 break;

            default:
                 printf("\n\n\tERRO! Voc\210 digitou uma opera\207\706o inv\240lida.\n");
                 break;
            }
    }

    printf("\n\tPrograma finalizado.\n");

}

typedef struct { //Definindo estrutura para Binário e Octal
   char *parteInteira;
   int parteFracionaria[20]; //Tamanho máximo da parte fracionária
   int preenchido; //Ultima posição utilizada do vetor parteFracionaria
} binario_Octal;

typedef struct { //Definindo estrutura para Hexa
    char *parteInteira;
    char parteFracionaria[20]; //Tamanho máximo da parte fracionária
    int preenchido; //Ultima posição utilizada do vetor parteFracionaria
} hexadecimal;

binario_Octal ConversaoBinaria (double decimal){ //Função de conversão de Decimal para Binário
    binario_Octal bin;
    bin.parteInteira = malloc(sizeof(double));
    int contador;
    long parteInteiraDecimal = (long)decimal; //Separando a parte inteira do número
    double parteFracionariaDecimal = decimal - parteInteiraDecimal; //Separando a parte fracionária do número
    itoa(parteInteiraDecimal, bin.parteInteira, 2); //Convertendo a parte inteira para a base numérica desejada
    contador = 0;
    while(contador != 20 && parteFracionariaDecimal != 0){ //Convertendo a parte fracionária
        parteFracionariaDecimal = 2 * parteFracionariaDecimal;
        bin.parteFracionaria[contador] = (int)parteFracionariaDecimal;
        parteFracionariaDecimal = parteFracionariaDecimal - (long)parteFracionariaDecimal;
        contador++;
    }
    bin.preenchido = contador; //Informando quantas posições do vetor parteFracionária foram utilizadas
    return bin;
}

binario_Octal ConversaoOctal(double decimal){ //Função de conversão de Decimal para Octal
    binario_Octal octal;
    octal.parteInteira = malloc(sizeof(double));
    int contador = 0;
    long parteInteiraDecimal = (long)decimal; //Separando a parte inteira do número
    double parteFracionariaDecimal = decimal - parteInteiraDecimal; //Separando a parte fracionária do número

    itoa(parteInteiraDecimal, octal.parteInteira, 8); //Convertendo a parte inteira para a base numérica desejada
    while(contador != 20 && parteFracionariaDecimal != 0){ //Convertendo a parte fracionária
        parteFracionariaDecimal = 8 * parteFracionariaDecimal;
        octal.parteFracionaria[contador] = (int)parteFracionariaDecimal;
        parteFracionariaDecimal = parteFracionariaDecimal - (long)parteFracionariaDecimal;
        contador++;
    }
    octal.preenchido = contador; //Informando quantas posições do vetor parteFracionária foram utilizadas
    return octal;
}

hexadecimal ConversaoHexa(double decimal){ //Função de conversão de Decimal para Hexadecimal
    char mapaHexa[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; //Criando um mapa para atribuir valores hexa correto a parte fracionária
    hexadecimal hexa;
    hexa.parteInteira = malloc(sizeof(double));
    int contador = 0;
    long parteInteiraDecimal = (long)decimal; //Separando a parte inteira do número
    double parteFracionariaDecimal = decimal - parteInteiraDecimal; //Separando a parte fracionária do número
    itoa(parteInteiraDecimal, hexa.parteInteira, 16); //Convertendo a parte inteira para a base numérica desejada
    while(contador != 20 && parteFracionariaDecimal != 0){ //Convertendo a parte fracionária
        parteFracionariaDecimal = 16 * parteFracionariaDecimal;
        hexa.parteFracionaria[contador] = mapaHexa[(int)parteFracionariaDecimal]; //Usando o mapa para atribuir a string os valores corretos
        parteFracionariaDecimal = parteFracionariaDecimal - (long)parteFracionariaDecimal;
        contador++;
    }
    hexa.preenchido = contador; //Informando quantas posições do vetor parteFracionária foram utilizadas
    return hexa;
}

void imprimirValoresBinario (binario_Octal x){ //Função impressão do valor binário no console
    printf("\n\tBinario: ");
    printf("%s", x.parteInteira);
    printf(".");
    int i;
    for(i = 0; i < x.preenchido; i++){
        printf("%d", x.parteFracionaria[i]);
    }
    printf("\n");
}

void imprimirValoresOctal (binario_Octal x){ //Função impressão do valor octal no console
    printf("\tOctal: ");
    printf("%s", x.parteInteira);
    printf(".");
    int i;
    for(i = 0; i < x.preenchido; i++){
        printf("%d", x.parteFracionaria[i]);
    }
    printf("\n");
}

void imprimirValoresHexa (hexadecimal hexa){ //Função impressão do valor hexadecimal no console
    printf("\tHexadecimal: ");
    int i;
    for (i = 0; i<strlen(hexa.parteInteira); i++){
        printf("%c", toupper(hexa.parteInteira[i])); //Transforma qualquer char minúsculo contido no vetor em char maiúsculo
    }
    printf(".");
    for (i = 0; i<hexa.preenchido; i++){
        printf("%c", hexa.parteFracionaria[i]);
    }
    printf("\n");
}

void initConversao(){
    double decimal;
    printf("\n\tDigite um numero decimal :");
    scanf("%lf",&decimal);

    binario_Octal bin;
    bin = ConversaoBinaria(decimal);
    imprimirValoresBinario(bin);

    free(bin.parteInteira);

    bin = ConversaoOctal(decimal);
    imprimirValoresOctal(bin);

    free(bin.parteInteira);

    hexadecimal hexa;
    hexa = ConversaoHexa(decimal);
    imprimirValoresHexa(hexa);

    free(hexa.parteInteira);
}

void initPivotacao(){
    char namefile[50];

    printf("\n\tNome do arquivo : ");
    scanf("%s",&namefile);
    strcat(namefile,".txt");

    if(fileExist(namefile)==1){
        int tam = obtemTamanhoMatriz(namefile);
        double **m = alocaMatriz(tam, tam + 1);
        int *v = alocaVetorPosicaoColunas(tam);

        inicializaMatrizDoArquivo(namefile, m);
        //imprimeMatriz(m, tam, tam + 1);
        pivotacao(m, tam,v);
        resolveMatrizTS(m,tam,v);
        //imprimeMatriz(m, tam, tam + 1);
        desalocaMatriz(m,tam);
    }else{
        printf("\n\tERRO! Arquivo n\706o existe.\n");
    }
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
     * v       : vetor que armazena as posicoes finais das colunas
     */

    int i, j, k;

    double maior = m[0][0];
    int lm = 0;
    int cm = 0;
    int lt = 0;

    double *aux;

    double aux_troca;
    int aux_pos;

    double mult;

    for(i = 0; i < n; i++)
    {
        v[i]= i; //armazena as posicoes finais das colunas
    }

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

        //Efetua a troca das colunas
        if(i!=cm){
            for(lt=0;lt<n;lt++){
                aux_troca  = m[lt][cm];
                m[lt][cm] = m[lt][i];
                m[lt][i]  = aux_troca;
            }

            aux_pos = v[i];
            v[i] = v[cm];
            v[cm] = aux_pos;
        }
    }

    //resolveMatrizTS(m,n,v);
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

void resolveMatrizTS(double **m,int n, int v [])
{
    double *x;

    int tipo,i;

    x = malloc(sizeof(double)*n);
    if(x==NULL){
        printf("Erro ao alocar Matriz. Falta de memoria!\n");
        return 1;
    }

    tipo = sretro(m,n,x);

    printf("\tO SL \202 %s\n\n", tipo==0?"Determinado":tipo==1?"Indeterminado":"Incompativel");

    if(tipo!=2){
        for(i=0;i<n;i++){
            printf("x[%d] = %10.3lf ", v[i]+1,x[i]);

        }
        printf("\n\n");
        imprimeMatriz(m, n, n + 1);
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
    }else{
        printf("Erro! Arquivo invalido.\n");
        system ("pause");
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

int fileExist(char *nomeArquivo)
{
    int exist = 1;

    FILE *file = fopen(nomeArquivo, "r");

    if (file == NULL)
    {
        exist = 0;
    }

    return exist;
}

