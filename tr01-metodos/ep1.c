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

void resolveMatrizTS(double **m,int n, int v []);

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

//Lagrange e Newton
double *alocaCoeficientes(int n);
double calculaLimite(double *coeficientes, int grau);
int newton(double x0, double *vetorCoeficientesP, int grau);
double calculaExpressao(double x, double *coeficientes, int grau);
void initLagrange();

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
        printf("\tC - Conversao\n");
        printf("\tS - Sistema Linear\n");
        printf("\tE - Equacao Algebrica\n");
        printf("\tF - Finalizar\n");

        printf("\n\tEscolha uma opcao: ");
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
                 initLagrange();
                 break;
            case 'f':
            case 'F':
                 continua = 0;
                 break;

            default:
                 printf("\n\n\tERRO! Voce digitou uma operacao invalida.\n");
                 break;
            }
    }

    printf("\n\tPrograma finalizado.\n");

}

typedef struct { //Definindo estrutura para Binário e Octal
   int *parteInteira;
   int tamanhoInteira; //Quantos digitos o número terá na base Octal/Decimal
   int parteFracionaria[20]; //Tamanho máximo da parte fracionária
   int preenchido; //Ultima posição utilizada do vetor parteFracionaria
} binario_Octal;

typedef struct { //Definindo estrutura para Hexa
    unsigned char *parteInteira;
    int tamanhoInteira; //Quantos digitos o numero terá na base Hexa
    char parteFracionaria[20]; //Tamanho máximo da parte fracionária
    int preenchido; //Ultima posição utilizada do vetor parteFracionaria
} hexadecimal;

binario_Octal ConversaoBinaria (double decimal){ //Função de conversão de Decimal para Binário
    binario_Octal bin;
    long parteInteiraDecimal = (long)decimal; //Separando a parte inteira do número
    int digitos;
    if ( parteInteiraDecimal == 0 ){
       digitos = 1;
    } else {
        digitos = (log10(parteInteiraDecimal) / log10(2)) + 1; //Definindo quantos dígitos o número terá na base desejada
    }
    bin.parteInteira = malloc(digitos * sizeof(int)); //Alocando memória para o vetor de dígitos
    double parteFracionariaDecimal = decimal - parteInteiraDecimal; //Separando a parte fracionária do número
    int contador; //Criando contador para for
    bin.tamanhoInteira = digitos; //atribindo a quantidade de dígitos a variável da struct
    for(contador = 0; contador < digitos; contador++){ //Convertendo a parte inteira para a base numérica desejada
        bin.parteInteira[contador] = (parteInteiraDecimal%2);
        parteInteiraDecimal = parteInteiraDecimal/2;
    }
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
    long parteInteiraDecimal = (long)decimal; //Separando a parte inteira do número
    int digitos;
    if ( parteInteiraDecimal == 0 ){
       digitos = 1;
    } else {
      digitos = (log10(parteInteiraDecimal) / log10(8)) + 1; //Definindo quantos dígitos o número terá na base desejada
    }
    octal.parteInteira = malloc(digitos * sizeof(int)); //Alocando memória para o vetor de dígitos
    double parteFracionariaDecimal = decimal - parteInteiraDecimal; //Separando a parte fracionária do número
    int contador;//Criando contador para for
    octal.tamanhoInteira = digitos; //atribindo a quantidade de dígitos a variável da struct
    for(contador = 0; contador < digitos; contador++){ //Convertendo a parte inteira para a base numérica desejada
        octal.parteInteira[contador] = (parteInteiraDecimal%8);
        parteInteiraDecimal = parteInteiraDecimal/8;
    }
    contador = 0;
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
    int contador;
    long parteInteiraDecimal = (long)decimal; //Separando a parte inteira do número
    int digitos;
    if ( parteInteiraDecimal == 0 ){
       digitos = 1;
    } else {
       digitos = (log10(parteInteiraDecimal) / log10(16)) + 1; //Definindo quantos dígitos o número terá na base desejada
    }
    hexa.parteInteira = malloc(digitos * sizeof(char)); //Alocando memória para o vetor de dígitos
    double parteFracionariaDecimal = decimal - parteInteiraDecimal; //Separando a parte fracionária do número
    hexa.tamanhoInteira = digitos; //Criando contador para for
    for(contador = 0; contador < digitos; contador++){ //Convertendo a parte inteira para a base numérica desejada
        hexa.parteInteira[contador] = mapaHexa[parteInteiraDecimal%16];
        parteInteiraDecimal = parteInteiraDecimal/16;
    }
    contador = 0;
    while(contador != 20 && parteFracionariaDecimal != 0){ //Convertendo a parte fracionária
        parteFracionariaDecimal = 16 * parteFracionariaDecimal;
        hexa.parteFracionaria[contador] = mapaHexa[(int)parteFracionariaDecimal]; //Usando o mapa para atribuir a string os valores corretos
        parteFracionariaDecimal = parteFracionariaDecimal - (long)parteFracionariaDecimal;
        contador++;
    }
    hexa.preenchido = contador; //Informando quantas posições do vetor parteFracionária foram utilizadas
    return hexa;
}

void imprimirValoresBinario (binario_Octal binario){ //Função impressão do valor binário no console
    printf("\tBinario: ");
    int i;
    for(i = (binario.tamanhoInteira - 1); i >=0; i--){
        printf("%d", binario.parteInteira[i]);
    }
    if(binario.preenchido != 0){
        printf(".");
        for(i = 0; i < binario.preenchido; i++){
            printf("%d", binario.parteFracionaria[i]);
        }
    }
    printf("\n");
}

void imprimirValoresOctal (binario_Octal octal){ //Função impressão do valor octal no console
    printf("\tOctal: ");
    int i;
    for(i = octal.tamanhoInteira - 1 ; i >= 0; i--){
        printf("%d", octal.parteInteira[i]);
    }
    if(octal.preenchido != 0){
        printf(".");
        for(i = 0; i < octal.preenchido; i++){
            printf("%d", octal.parteFracionaria[i]);
    }
        }
    printf("\n");
}

void imprimirValoresHexa (hexadecimal hexa){ //Função impressão do valor hexadecimal no console
    printf("\tHexadecimal: ");
    int i;
    for (i = hexa.tamanhoInteira - 1; i >= 0; i--){
        printf("%c", hexa.parteInteira[i]); //Transforma qualquer char minúsculo contido no vetor em char maiúsculo
    }
    if(hexa.preenchido != 0){
        printf(".");
        for (i = 0; i<hexa.preenchido; i++){
            printf("%c", hexa.parteFracionaria[i]);
        }
    }
    printf("\n");
}

void initConversao(){
    double decimal;
    printf("\n\n\tDigite um numero decimal :");
    scanf("%lf",&decimal);
    printf("\n");
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
        printf("\n\tERRO! Arquivo nao existe.\n");
    }
}

void pivotacao(double **m, int n,int v [])
{

    /*
     * Esta funÃ§Ã£o implementa o metodo da pivotaÃ§Ã£o completa
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
        //nÃ£o pivotadas da matriz de coeficientes.

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


        //Realiza a pivotaÃ§Ã£o das linhas abaixo da linha i

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
    * Se o SL for determinado, x recebe a soluÑao do SL e a funÑao devolve 0;
    * Se for indeterminado x recebe uma das infinitas soluÑƒÑˆes do SL e a funÑao devolve 1;
    * Se for incompativel a funÑao devolve 2;
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
                /*x[i] e variÑ€vel livre*/
                x[i]=0;
                tipo = 1;
            }else{
                return 2; /*SL incompatÑŒvel*/
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
        return;
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

    /* Dado um vetor, esta funÃ§Ã£o procura o maior elemento
     * os indices 0 e tam - 1.
     * A funÃ§Ã£o retorna o indice o maior elemento.
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

// Funcoes genericos de manipulaÃ§Ã£o de matrizes

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
     * base da matriz. Caso contrÃ¡rio, ele devolverÃ¡ um
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
     * com a funÃ§Ã£o aloca_matriz.
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

//Lagrange e Newton

//metodo que inicia o metodo lagrange
void initLagrange(){
     int retorno;
     retorno = lagrange();
     
     if ( retorno == -3){
        printf("\n\n\tFalta de memoria!!!\n\n");
     } else if ( retorno == -2){
        printf("\n\n\tERRO: a0 nao pode ser igual a zero!!!\n\n");
     } else if ( retorno == -1){
        printf("\n\n\tERRO: an nao pode ser menor ou igual a zero!!!\n\n");
     }
}

/*
*Metodo que solicita a insercao de coeficientes de uma expressao algebrica,
*para que calcule os limites superiores e inferiores das raizes positivas e
*negativas, e no fim faz a chamada do metodo newton que calcula uma raiz
*aproximada do limite superior positivo
*se return = 0, entao ok
*se return = -1, entao an<=0
*se return = -2, entao a0==0
*se return = -3, falta de espaço para alocar os vetores de coeficientes
*/
int lagrange(){
    int grau;
    int i;
    int j;
    int retornoMetodoNewton;
    double *vetorCoeficientesP;
    double *vetorCoeficientesP1;
    double *vetorCoeficientesP2;
    double *vetorCoeficientesP3;
    double coeficiente;
    double L;
    double L1;
    double L2;
    double L3;
    
    printf("\n\n\n\tGrau da equacao: ");
    scanf(" %d", &grau);
       
    vetorCoeficientesP = alocaCoeficientes(grau + 1);
    if (vetorCoeficientesP == NULL){
       return -3;
    }
  
    vetorCoeficientesP1 = alocaCoeficientes(grau + 1);
    if (vetorCoeficientesP1 == NULL){
       free(vetorCoeficientesP);
       return -3;
    }
    
    vetorCoeficientesP2 = alocaCoeficientes(grau + 1);
    if (vetorCoeficientesP2 == NULL){
       free(vetorCoeficientesP);
       free(vetorCoeficientesP1);
       return -3;
    }

    vetorCoeficientesP3 = alocaCoeficientes(grau + 1);
    if (vetorCoeficientesP3 == NULL){
       free(vetorCoeficientesP);
       free(vetorCoeficientesP1);
       free(vetorCoeficientesP2);
       return -3;
    }
    
    
    /*
    *Bloco que captura os valores dos coeficientes com a
    *funcao scanf, testa se tem os valores aceitaveis e
    *os atribuem ao vetorCoeficientesP.
    */
    for ( i=grau; i>=0; i--){
        printf("\n\ta[%d] = ",i);
        scanf(" %lf", &coeficiente);
        if( i == grau){
            if ( coeficiente <= 0){
               return -1;
            }    
        } else if ( i == 0){
            if ( coeficiente == 0) {
               return -2;
            }
        }
        
        vetorCoeficientesP[i] = coeficiente;
    }
        
    //Calculo do limite superior positivo
    L = calculaLimite(vetorCoeficientesP, grau);
    
    /*
    *Bloco que cria P1 (vetorCoeficientesP1) a partir de P (vetorCoeficientesP),
    *para depois ser passado como parâmetro na funcao: calculaLimite
    */
    for(i = 0, j = grau; i<=grau; i++, j--){
          vetorCoeficientesP1[i] = vetorCoeficientesP[j];
    }
    
    //alterando os sinais dos coeficientes de P1 se an < 0
    if ( vetorCoeficientesP1[grau] < 0 ) {
         for (i = 0; i <=grau; i++)
         {
             vetorCoeficientesP1[i] = vetorCoeficientesP1[i] * -1.0;
         }    
    }
    //Calculo do primeiro limite, limite inferior positivo
    L1 = calculaLimite(vetorCoeficientesP1, grau);
    L1 = 1.0/L1;
    
    /*
    *Bloco que cria P2 (vetorCoeficientesP2) a partir de P (vetorCoeficientesP),
    *para depois ser passado como parâmetro na funcao: calculaLimite
    */
    for(i = 0; i<=grau; i++){
          if ( i%2 == 1){
             vetorCoeficientesP2[i] = vetorCoeficientesP[i] * -1.0;
          } else {
            vetorCoeficientesP2[i] = vetorCoeficientesP[i];
          }
    }
    
    //alterando os sinais dos coeficientes de P2 se an < 0
    if ( vetorCoeficientesP2[grau] < 0 ) {
         for (i = 0; i <=grau; i++)
         {
             vetorCoeficientesP2[i] = vetorCoeficientesP2[i] * -1.0;
         }    
    }
    //Calculo do limite inferior negativo
    L2 = calculaLimite(vetorCoeficientesP2, grau);
    L2 = L2 * -1.0;
    
    /*
    *Bloco que cria P3 (vetorCoeficientesP3) a partir de P1 (vetorCoeficientesP1),
    *para depois ser passado como parâmetro na funcao: calculaLimite
    */
    for(i = 0; i<=grau; i++){
          if ( i%2 == 1){
             vetorCoeficientesP3[i] = vetorCoeficientesP1[i] * -1.0;
          } else {
            vetorCoeficientesP3[i] = vetorCoeficientesP1[i];
          }
    }
    
    //alterando os sinais dos coeficientes de P3 se an < 0
    if ( vetorCoeficientesP3[grau] < 0 ) {
         for (i = 0; i <=grau; i++)
         {
             vetorCoeficientesP3[i] = vetorCoeficientesP3[i] * -1.0;
         }    
    }
    //Calculo do limite superior negativo
    L3 = calculaLimite(vetorCoeficientesP3, grau);
    L3 = -1.0/L3;
     
    printf("\n\n\t%lf <= E+ <= %lf", L1, L );
    printf("\n\n\t%lf <= E- <= %lf\n\n", L2, L3);

    /*
    *Invocação no método newton, que aplica o método de newton, usando
    *como valor inicial o limite superior positivo
    */ 
    retornoMetodoNewton = newton(L, vetorCoeficientesP, grau);
    
    free(vetorCoeficientesP);
    free(vetorCoeficientesP1);
    free(vetorCoeficientesP2);
    free(vetorCoeficientesP3);
    
    if ( retornoMetodoNewton == -1 ){
       return -3;
    }
    return 0;
}

/*
*Método que executa o método de Newton para o cálculo de uma raiz proxima.
*
*params: x0 - valor inicial para a execucao do calculo da raiz
*        vetorCoeficientesP - vetor com os coeficientes da expressao algebrica
*        grau - valor inteiro com o grau da expressao algebrica
*
*se return = 0, entao ok
*se return = -1,falta de espaco para o vetor dos coeficintes da derivada
*se return = -2
*
*/
int newton(double x0, double *vetorCoeficientesP, int grau){
     
     double *expressaoDerivada = alocaCoeficientes(grau);
     int i = grau;
     int qtdIteracoes = 1000;
     double fLinhaDeX=0;
     double fDeX=0;
     double x = x0;

     if (expressaoDerivada == NULL){
         return -1;
     }
     
     //Bloco que gera os coeficientes da expressao da derivada
     for(i; i>0; i--)
     {
         expressaoDerivada[i-1] = vetorCoeficientesP[i] * i;      
     }
     
     /*
     *Bloco que executa o calculo de uma raiz da expressao vetorCoeficientesP
     */
     for ( i = 0; i<1000; i++) {
         double valorAnterior = x;
         double moduloDaDiferenca = 0;
         
         fDeX = calculaExpressao(x, vetorCoeficientesP, grau);
         fLinhaDeX = calculaExpressao(x, expressaoDerivada, grau-1);
         x = x - (fDeX/fLinhaDeX);
         
         moduloDaDiferenca = valorAnterior - x;
         if ( moduloDaDiferenca < 0){
              moduloDaDiferenca = moduloDaDiferenca * -1.0;
         }
         
         if ( moduloDaDiferenca < EPSILON){
            qtdIteracoes = ++i;
            i = 1000;
         } 
         
     }
     
     printf("\tRaiz aproximada : %6.8lf  Qtd de Iteracoes: %i", x,qtdIteracoes);
     
     free(expressaoDerivada);
     return 0;
}

/*
*Metodo que calcula o resultado de uma expressao algebrica
*a partir do vetor de coeficientes, do grau da expressao e 
*do valor de x para o qual eu quero calcular a expressao
*
*retorna o valor da expressao 
*/
double calculaExpressao(double x, double *coeficientes, int grau)
{
    int i;
    double resposta = 0;
    for( i = grau; i>=0; i--){
         resposta += pow(x, i) * coeficientes[i]; 
    }
    return resposta;
}

/*
*Metodo que caucula o Limite de uma expressao polinomial
*/
double calculaLimite(double *coeficientes, int grau)
{
    int i ;
    double B = 0.0;
    int n = grau;
    int k = 0;
    double modulo;
    double L;
    for (i = grau; i >=0; i--){
        
         if ( coeficientes[i] < 0 ){
            modulo = coeficientes[i] * -1.0;
            if ( modulo > B) {
               B = modulo;
            }
            if ( i > k){         
               k = i;
            }
         }      
    }
    
    //calcula o limite
    L = 1 + pow(B/coeficientes[grau], 1.0/(n-k)); 

    return L;
}

//metodo que aloca n posicoes double para um vetor 
double *alocaCoeficientes(int n){

	int i,j;
	double *m;
	
	m = malloc(sizeof(double)*n);
	
	if(m==NULL){
		/*memoria insuficiente*/
		return NULL;
	}
	
	return m;	
}


