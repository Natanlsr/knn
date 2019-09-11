#include<stdio.h>
#include<math.h>
#include<stdlib.h>

struct amostra{
    double *atributos;
};

typedef struct amostra Amostra;

struct metrica{
    double distancia;
    double classe;
};

typedef struct metrica Metrica;

struct distancia{
    Metrica *dados;
};

typedef struct distancia Distancia;

int ordena(const void *a,const void *b){
    double d1 = ((Metrica*)a)->distancia;
    double d2 = ((Metrica*)b)->distancia;
    if(d1 == d2)
        return 0;
    else
        if(d1<d2)
            return -1;
        else
            return 1;

}

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

Distancia* calcularDistancia(Amostra *amostra,int inicio,int fim,int qtd_am,int qtd_atr){

    printf("%d,%d\n",inicio,fim);

    Distancia *distancias; //vetor com as distancias das amostras selecionadas para serem classificadas
    int total = (fim - inicio+1); //Quantidade de amostras
    int pos_dis,pos_calc,i,j;
    int qtd_distancias = qtd_am - total; //Quantidade de distacinas de cada amostra
    distancias = malloc(total * sizeof(Distancia));
    printf("Total: %d\n",total);
    pos_calc = 0;
    for(int in = inicio;in<=fim;in++){
        printf("Amostra: %d\n",in);
        distancias[pos_calc].dados = malloc(qtd_distancias*sizeof(Metrica));
        pos_dis = 0;
        for(i =0;i<qtd_am;i++){
            if(i == inicio)
                if(fim == qtd_am-1)
                    break;
                else
                    i = fim+1;
            //printf("Distancia em relacao a: %d\n",i);
            //Calculando a distancia
            //Percorrendo os atributos para calcular
            double soma = 0;
            for(j=0;j<qtd_atr;j++){
                //printf("%d,%d,%d,%d\n",pos_calc,in,i,j);
                soma+= pow(amostra[in].atributos[j] - amostra[i].atributos[j],2);
            }
            distancias[pos_calc].dados[pos_dis].distancia = sqrt(soma);
            distancias[pos_calc].dados[pos_dis].classe = amostra[i].atributos[j];//para nao perder classe da amostra

            pos_dis++;
        }
        qsort(distancias[pos_calc].dados,qtd_distancias,sizeof(Metrica),ordena);
        pos_calc++;

    }

    return distancias;




}

double classificar(Amostra *amostra,int k_fold, int n_vizinhos,int q_am,int q_at){

    int qtd_k = ceilf(q_am/k_fold);
    int it = 0,inicio,fim,qtd_distancias,qtd_calc; //indica qual iteracao do k-fold esta,posicao inicial do grupo de teste,posicao final
    printf("%d\n",qtd_k);
    Distancia *distancias;

    while(it<k_fold){
        inicio = it * qtd_k; //Primeira posicao da amostra de teste
        fim = inicio + qtd_k - 1; //ultima posicao
        if(it+1 == k_fold){
            fim = q_am-1;
        }
        qtd_calc = (fim - inicio+1);
        int qtd_distancias = q_am - qtd_calc;
        distancias = calcularDistancia(amostra,inicio,fim,q_am,q_at);
        for(int i=0;i<qtd_calc ;i++){
            printf("\n----------DISTANCIA DA AMOSTRA %d, em relacao as outras:----------\n",i+inicio);
                for(int j =0;j<qtd_distancias;j++){

                    printf("%f\n",distancias[i].dados[j]);
                }
        }
        it++;
    }

    return 0.0;
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
    /*
    for(int i=0;i<qtd_amostra;i++){
        printf("AMOSTRA: %d\n",i);
        for(int j=0;j<qtd_atributos;j++){
            printf("ATRIBUTO %d: %f\n",j,amostras[i].atributos[j]);
        }
        printf("-------------------------------------------------------\n");
    }
    */

    if(amostras == NULL){
        printf("Erro ao processar o arquivo das amostras");
        return -1;
    }



    classificar(amostras,k_fold,n_vizinhos,qtd_amostra,qtd_atributos);












    return 0;
}
