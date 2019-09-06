#include<stdio.h>
#include<math.h>
#include<stdlib.h>

struct amostra{
    double *atributos;
};

typedef struct amostra Amostra;

Amostra* lerArquivo(char *nm_arquivo,int *q_a, int *q_at){

    FILE *arq;
    Amostra *amostra;
    int qtd_amostras,qtd_atributos,i=-1;
    char *atributos;
    char *leitura;
    int total,j,a=-1;

    arq = fopen(nm_arquivo,"rt");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        return NULL;
    }

    fscanf(arq,"%d%d",&qtd_amostras,&qtd_atributos);
    //printf("%d,%d\n",qtd_amostras,qtd_atributos);
    atributos = malloc(qtd_atributos+1 * sizeof(double));
    amostra = malloc(qtd_amostras * sizeof(Amostra));
    total = qtd_atributos+1;
    *q_a = qtd_amostras;
    *q_at = qtd_atributos;

    while(!feof(arq)){
        if(i==0 || i%total==0){
            a++;
            //printf("AMOSTRA: %d\n",a);
            amostra[a].atributos =  malloc(total * sizeof(double));
            j = 0;//indice do atributo
        }
        if(fgets(atributos,total,arq))
            if(i != -1){
                amostra[a].atributos[j]=atof(atributos);
                //printf("%f\n",amostra[a].atributos[j]);
                j++;
            }
        i++;
    }

    return amostra;

}

int main(){

    int qtd_amostra,qtd_atributos;
    int n_vizinhos,k_fold,z_score;
    char nm_arquivo[150];
    Amostra *amostras;

    printf("Digite o nome do aquivo: ");
    scanf("%s",nm_arquivo);
    printf("Digite o numero de vizinhos do k-nn: ");
    scanf("%d",&n_vizinhos);
    printf("Digite o numero de particoes dos k-fold: ");
    scanf("%d",&k_fold);
    printf("Digite 1 caso queira normalizar ou 0 caso contrario: ");
    scanf("%d",&z_score);

    amostras = lerArquivo(nm_arquivo,&qtd_amostra,&qtd_atributos);

    if(amostras == NULL){
        printf("Erro ao processar o arquivo das amostras");
        return -1;
    }
















    return 0;
}
