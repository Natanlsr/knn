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
Amostra* distribuirDados(Amostra *amostras,int qtd_amostras,int qtd_atributos){
    int i=0,c1=0,c2=0,c3=0;
    //Para um problema com 3 classes e amostras com classes proporcionais(mesma quantidade de cada classe)
    int qtd_cada_classe = qtd_amostras / 3;

    Amostra *classe1 = malloc(qtd_cada_classe * sizeof(Amostra));
    Amostra *classe2 = malloc(qtd_cada_classe * sizeof(Amostra));
    Amostra *classe3 = malloc(qtd_cada_classe * sizeof(Amostra));

    for(i=0;i<qtd_amostras;i++){

            if(amostras[i].atributos[qtd_atributos] == 1.0 )
                classe1[c1++].atributos = amostras[i].atributos;

            if(amostras[i].atributos[qtd_atributos] == 2.0 )
                classe2[c2++].atributos = amostras[i].atributos;

            if(amostras[i].atributos[qtd_atributos] == 3.0 )
                classe3[c3++].atributos = amostras[i].atributos;

    }
    //Distribuindo amostras
    int j = 0;
    for(i=0;i<qtd_amostras;i+=3){
        amostras[i].atributos = classe1[j].atributos;
        amostras[i+1].atributos = classe2[j].atributos;
        amostras[i+2].atributos = classe3[j++].atributos;
    }
    return amostras;

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
    amostra = distribuirDados(amostra,qtd_amostras,qtd_atributos);

    return amostra;

}

double calcularClasse(Metrica *distancias,int n_vizinhos,int qtd_dis){

    int classe_1 = 0;
    int classe_2 = 0;
    int classe_3 = 0;
    for(int i =0;i<n_vizinhos;i++){
        if(distancias[i].classe == 1.0){
            classe_1++;
        }

        if(distancias[i].classe == 2.0){
            classe_2++;
        }
        if(distancias[i].classe == 3.0){
            classe_3++;
        }
    }

        if(classe_1 > classe_2 && classe_1 > classe_3){

            return 1.0;
        }
        if(classe_2 > classe_1 && classe_2 > classe_3){
            return 2.0;
        }
        if(classe_3 > classe_1 && classe_3 > classe_2){
            return 3.0;
        }

        return 0;

}

Distancia* calcularDistancia(Amostra *amostra,int inicio,int fim,int qtd_am,int qtd_atr){

    printf("%d,%d\n",inicio,fim);
    Distancia *distancias; //vetor com as distancias das amostras selecionadas para serem classificadas
    int total = (fim - inicio+1); //Quantidade de amostras
    int pos_dis,pos_calc,i,j;
    int qtd_distancias = qtd_am - total; //Quantidade de distacinas de cada amostra
    distancias = malloc(total * sizeof(Distancia));
    pos_calc = 0;
    for(int in = inicio;in<=fim;in++){
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

int classificar(Amostra *amostra,int k_fold, int n_vizinhos,int q_am,int q_at){

    int qtd_k = ceilf(q_am/k_fold);
    int it = 0,inicio,fim,qtd_distancias,qtd_calc; //indica qual iteracao do k-fold esta,posicao inicial do grupo de teste,posicao final
    int qtd_acertada = 0;
    double classe;
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
        for(int i=0;i<qtd_calc;i++){

                //Diminuindo o vizinho caso seje necessario
                for(int j=n_vizinhos;j>0;j--){
                    classe = calcularClasse(distancias[i].dados,j,qtd_distancias);
                    if(classe != 0.0)
                        break;
                }


        /*
            printf("Classe calculada da amostra %d: %f\n",i+inicio,classe);
            printf("Classe correta da amostra:%f\n",amostra[i+inicio].atributos[q_at]);
          */

            if(classe == amostra[i+inicio].atributos[q_at]){
                qtd_acertada++;
            }


        }

        it++;
    }

    return qtd_acertada;
}

int main(){

    int qtd_amostra,qtd_atributos,acertos;
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

    for(int i=0;i<qtd_amostra;i++){
        printf("AMOSTRA: %d\n",i);
        for(int j=0;j<=qtd_atributos;j++){
            printf("ATRIBUTO %d: %f\n",j,amostras[i].atributos[j]);
        }
        printf("-------------------------------------------------------\n");
    }


    if(amostras == NULL){
        printf("Erro ao processar o arquivo das amostras");
        return -1;
    }



    acertos = classificar(amostras,k_fold,n_vizinhos,qtd_amostra,qtd_atributos);
    double porcentagem = ((double)(acertos)/qtd_amostra)*100;
    printf("Porcentagem de acertos: %.4lf%%",porcentagem);











    return 0;
}
