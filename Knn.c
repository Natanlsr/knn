#include<stdio.h>
#include<math.h>
#include<stdlib.h>

struct amostra{
    double *atributos;
    int classe;
};

typedef struct amostra Amostra;

struct metrica{
    double distancia;
    int classe;
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


Amostra* lerArquivo(char *nm_arquivo,int *q_a, int *q_at, int *q_c){

    FILE *arq;
    Amostra *amostra;
    int qtd_amostras,qtd_atributos,i=-1;
    char *atributos;
    char *leitura;
    int total,j,a=0;
    double atributo;

    arq = fopen(nm_arquivo,"r");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        return NULL;
    }

    fscanf(arq,"%d%d",&qtd_amostras,&qtd_atributos);
    printf("%d,%d\n",qtd_amostras,qtd_atributos);
    atributos = malloc(qtd_atributos+1 * sizeof(double));
    amostra = malloc(qtd_amostras * sizeof(Amostra));
    total = qtd_atributos+1;
    *q_a = qtd_amostras;
    *q_at = qtd_atributos;

    while(!feof(arq)){


        amostra[a].atributos =  malloc(total * sizeof(double));
        for(j=0;j<qtd_atributos;j++){
            fscanf(arq,"%lf",&amostra[a].atributos[j]);
            //printf("%lf\n",amostra[a].atributos[j]);
        }
        fscanf(arq,"%d",&amostra[a].classe);
        /*
        if(fgets(atributos,total,arq))
            if(i != -1){
                amostra[a].atributos[j]=atof(atributos);
                //printf("%f\n",amostra[a].atributos[j]);
                j++;
            }
            */
        a++;
    }
    fclose(arq);
    *q_c = amostra[a-1].classe;
    //amostra = distribuirDados(amostra,qtd_amostras,qtd_atributos);

    return amostra;

}

int calcularClasse(Metrica *distancias,int n_vizinhos,int qtd_dis,int qtd_classes){

        int *classes = calloc(qtd_classes,sizeof(int));
        int pos,i;

        for(i =0;i<n_vizinhos;i++){
            pos = distancias[i].classe - 1;
            printf("Classe: %d\n",distancias[i].classe);
            classes[pos]++;
        }

        int maior = pos,empate = 0;

        for(i=1;i<qtd_classes;i++){
            if(classes[i] != 0){
                if(classes[i] > classes[maior])
                {
                    maior = i;
                }else{
                    if(classes[i] == classes[maior])
                        empate++;
                }
            }
        }

        if(empate == n_vizinhos)
            i = -1;

        return maior+1;

}

Distancia* calcularDistancia(Amostra *amostra,int inicio,int fim,int qtd_am,int qtd_atr){

    //printf("%d,%d\n",inicio,fim);
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
            distancias[pos_calc].dados[pos_dis].classe = amostra[i].classe;//para nao perder classe da amostra
            pos_dis++;
        }
        qsort(distancias[pos_calc].dados,qtd_distancias,sizeof(Metrica),ordena);
        pos_calc++;

    }

    return distancias;

}

double* media(Amostra *amostras, int qtd_atr,int qtd_am){

    double *media = calloc(qtd_atr,sizeof(double));
    int i,j;


    //Soma dos atributos
    for(i = 0; i < qtd_am; i++){
        for(j = 0;j < qtd_atr;j++){
            media[j] += amostras[i].atributos[j];
        }
    }
    //se faz a média
    for(i = 0;i < qtd_atr;i++){
        media[i] /= qtd_am;
    }

    return media;
}

double* desvio_padrao(Amostra *amostras, int qtd_atr, int qtd_am){

    double *desvio = calloc(qtd_atr,sizeof(double));
    int i,j;
    //ja possuindo a media

    double *m;
    m = media(amostras,qtd_atr,qtd_am);

    for(i = 0;i < qtd_am;i++){
        for(j = 0;j < qtd_atr; j++){
            desvio[j] += pow(amostras[i].atributos[j] - m[j], 2);
        }
    }

    for(i = 0;i < qtd_atr; i++){
        desvio[i] = sqrt(desvio[i]/qtd_am);
    }

    return desvio;
}

Amostra* z_score(Amostra *amostras,int qtd_atr, int qtd_am){

    int i,j;
    double *m = media(amostras,qtd_atr,qtd_am);
    double *d = desvio_padrao(amostras, qtd_atr, qtd_am);

    for(i =0; i< qtd_am;i++){
        for(j = 0; j< qtd_atr;j++){
            amostras[i].atributos[j] = (amostras[i].atributos[j] - m[j])/d[j];
        }
    }

    return amostras;
}

int classificar(Amostra *amostra,int k_fold, int n_vizinhos,int q_am,int q_at, int qtd_cl){

    int qtd_k = ceilf(q_am/k_fold);
    int it = 0,inicio,fim,qtd_distancias,qtd_calc; //indica qual iteracao do k-fold esta,posicao inicial do grupo de teste,posicao final
    int qtd_acertada = 0;
    int classe;
    //printf("%d\n",qtd_k);
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
                    classe = calcularClasse(distancias[i].dados,j,qtd_distancias,qtd_cl);
                    if(classe != 0)
                        break;
                }



            printf("Classe calculada da amostra %d: %d\n",i+inicio,classe);
            printf("Classe correta da amostra:%d\n",amostra[i+inicio].classe);


            if(classe == amostra[i+inicio].classe){
                qtd_acertada++;
            }


        }

        it++;
    }

    return qtd_acertada;
}

int main(){

    int qtd_amostra,qtd_atributos,acertos,qtd_classes;
    int n_vizinhos,k_fold,z;
    char nm_arquivo[150];
    Amostra *amostras;

    printf("Digite o nome do aquivo: ");
    scanf("%s",nm_arquivo);
    printf("Digite o numero de vizinhos do k-nn: ");
    scanf("%d",&n_vizinhos);
    printf("Digite o numero de particoes dos k-fold: ");
    scanf("%d",&k_fold);
    printf("Digite 1 caso queira normalizar ou 0 caso contrario: ");
    scanf("%d",&z);

    amostras = lerArquivo(nm_arquivo,&qtd_amostra,&qtd_atributos,&qtd_classes);
    printf("QUantidade de classes: %d\n",qtd_classes);
    if(amostras == NULL){
        printf("Erro ao processar o arquivo das amostras");
        return -1;
    }

    if(z == 1)
        amostras = z_score(amostras,qtd_atributos,qtd_amostra);

    int j;

    /*
    for(int i=0;i<qtd_amostra;i++){
        printf("AMOSTRA: %d\n",i);
        for(j=0;j<qtd_atributos;j++){
            printf("ATRIBUTO %d: %f\n",j,amostras[i].atributos[j]);
        }
        printf("Classe: %d\n",amostras[i].classe);
        printf("-------------------------------------------------------\n");
    }
    */







    acertos = classificar(amostras,k_fold,n_vizinhos,qtd_amostra,qtd_atributos,qtd_classes);
    double porcentagem = ((double)(acertos)/qtd_amostra)*100;
    printf("Porcentagem de acertos: %.4lf%%",porcentagem);




    return 0;
}
