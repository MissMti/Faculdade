#define _GNU_SOURCE
#include <stdio.h> 
#include <time.h> 
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "ep2.h"

int volta_atual;//guardar a volta que o primeiro está
int volta_ult;//guardar a volta que o ultimo esta
int nova_volta;//alerta se comecou uma volta nova
int acabou_volta;//alerta se o ultimo lugar terminou uma volta
int ciclistas;//quantidade de ciclistas ainda na pista

//entradas do programa
int debug;
int d;
int k;

//Para impressoes finais
int rank[15000][5]; //id + quando terminou
int qtd_terminados;

int quebrados[15000][5]; //id + volta que terminou
int qtd_quebrados;



struct Participante{
    int id;
    int vel;
    double pos;
    int volta;
    int coluna;
    int linha;
    double pos_geral;
    int fantasma;
};

struct Participante data[2505];//info dos participantes

struct Participante pista [2505][15]; //pistas + 10 posicoes
int qtd_pista [2505];//quanitdade de ciclistas em cada coluna

struct Participante colocacao[2505];//colocacao dos cilistas ainda presentes

int ocupados [2505][15];//guarda se um lugar na pista esta ocupado

pthread_barrier_t barreira1;
pthread_barrier_t barreira2;
pthread_barrier_t barreira3;
pthread_barrier_t barreira4;

pthread_mutex_t lock;

//Funcao que verifica a qtd de algarismo de um numero em prol de melhorar esteticamente as impressoes
int tamanho(int a){
    int  b=0;
    while( a != 0 ) {
       a /= 10;
       b++;
    }
    return b;
}

//Funcao para os sorteios
int sorteia(){
    int r=rand()%100;
    return r;
}

//Imprime a pista
void printa_pista(){
    printf("\n");
    for(int j=9;j>=0;j--){
        for(int i=d-1;i>=0;i--){
            if(qtd_pista[i]>j){
                printf("%d ",pista[i][j].id);
            }
            else{
                printf(". ");                        
            }
        }
        printf("\n");
    }
    printf("\n");
}

//"Desce" os ciclistas de cada coluna na hora da saida, similar a ideia comentada pelo prof
void desce(){
    int m=0,l;
    for(int j=0;j<d;j++){
        m=0;
        l=0;
        while(m<qtd_pista[j]){
            if(ocupados[j][l]==1){
                
                pista[j][m]=pista[j][l];
                pista[j][m].linha=m;
                data[pista[j][l].id]=pista[j][m];
                ocupados[j][l]=0;
                ocupados[j][m]=1;
                m++;
            }

            l++;
                
        }    
    }
}

//para ordenar em ordem descendente
void ordena(int n){
    struct Participante tmp;
    for (int i=0; i<n; i++){
        for (int j=i+1; j<n; j++){
            if (colocacao[i].pos_geral < colocacao[j].pos_geral){
                tmp = colocacao[i];
                colocacao[i] = colocacao[j];
                colocacao[j] = tmp;
            }
        }
    }
}

//funcao para andar
void anda(struct Participante at){

    //atualiza posicao
    at.pos=(double)(at.pos+(double)at.vel/(double)60);
    at.pos_geral=(double)(at.pos_geral+(double)at.vel/(double)60.0);
    ocupados[at.coluna][at.linha]=0;
    qtd_pista[at.coluna]--;

    if((int)(at.pos*10)%10==0 || at.vel==60){  //nova posicao

        if(at.pos>=(double)d){//nova volta
            at.volta++;

            //quebrou ao atualizar nova posicao?
            if(at.volta%6==0){
                if(sorteia()<=15){
                    quebrados[qtd_quebrados][0]=at.id;
                    quebrados[qtd_quebrados][1]=at.volta;
                    qtd_quebrados++;
                    ciclistas--;
                    //matar
                    at.fantasma=1;
                    data[at.id]=at;
                    printf("\n Quebrou o id %d na volta %d\n",at.id,at.volta);
                    return ;
                }
            }

            //Nova volta?
            if(at.volta>volta_atual){

                volta_atual=at.volta;
                nova_volta=1;
            }     


            //Coluna da frente não tem como colocar, entao ele fica parado
            if(ocupados[0][9]==1){
                ocupados[at.coluna][at.linha]=1;
                qtd_pista[at.coluna]++;
                return;
            }

            //Sorteia velocidade nova
            if(ocupados[1][at.linha]==1){
                if(pista[1][at.linha].vel==30){
                    at.vel=30;
                }
            }
            else{
                if(at.vel==30){
                    if(sorteia()<=70){
                        at.vel=60;
                    }
                }
                else{
                    if(sorteia()<=50){
                        at.vel=30;
                    }
                }
            }

    
            at.pos=(double)(at.pos-(double)d);
            at.coluna=0;

            int a=0;

            for(int j=8;j>=0;j--){
                if(ocupados[0][j]==1){
                    at.linha=j+1;
                    a=1;
                    break;
                }
            }

            if(a==0){
                at.linha=0;
            }

                  
        }
        else{
            
            //Coluna da frente não tem como colocar, entao ele fica parado 
            if(ocupados[at.coluna+1][9]==1){
                ocupados[at.coluna][at.linha]=1;
                qtd_pista[at.coluna]++;
                return;
            }

            at.coluna=at.pos;

            int a=0;
            for(int j=8;j>=0;j--){
                if(ocupados[at.coluna][j]==1){
                    at.linha=j+1;
                    a=1;
                    break;
                }
            }

            if(a==0){
                at.linha=0;
            }
        }
    }
    

    //colocar no novo lugar
    qtd_pista[at.coluna]++;
    pista[at.coluna][at.linha].id=at.id;
    pista[at.coluna][at.linha].vel=at.vel;
    pista[at.coluna][at.linha].pos=at.pos;
    pista[at.coluna][at.linha].pos_geral=at.pos_geral;
    pista[at.coluna][at.linha].volta=at.volta;
    pista[at.coluna][at.linha].coluna=at.coluna;
    pista[at.coluna][at.linha].linha=at.linha;
    pista[at.coluna][at.linha].fantasma=at.fantasma;
    data[at.id]=pista[at.coluna][at.linha];
    ocupados[at.coluna][at.linha]=1;
    
    return;
}


//funcao das threads ciclistas
void* corrida(void *arg){

    struct Participante *b;
    b = (struct Participante *)arg;


    //enquanto acontecer (ha no maximo 2 ciclistas na pista)
    while(ciclistas>=2){
        pthread_barrier_wait(&barreira2);

        //todos os ciclistas andam 1 passos de forma concorrente
        if(data[b->id].fantasma==0 && ciclistas>=2){
            pthread_mutex_lock(&lock);
            anda(data[b->id]);
            pthread_mutex_unlock(&lock);
        }

        
        pthread_barrier_wait(&barreira4);
        if(ciclistas<2){
            break;
        }
        pthread_barrier_wait(&barreira1);
        pthread_barrier_wait(&barreira3);
        if(ciclistas<2){
            break;
        }

    }
    

}


int main(int numargs, char *arg[]) { 

    //recebe dados de entrada
    char* a=arg[1];
    d=atoi(a);
    char* b=arg[2];
    k=atoi(b);

    debug=0;
    if(numargs==4 && strcmp(arg[3], "-debug") == 0){
        debug=1;
    }


    //inicializa random
    srand(time(NULL));    

    //comeca variaveis
    int clock=0;//em ms  

    ciclistas=k;
    nova_volta=0;
    acabou_volta=0;
    qtd_quebrados=0;
    volta_atual=1;
    volta_ult=1;

    for(int i=0;i<2502;i++){
        for(int j=0;j<12;j++){
            ocupados[i][j]=0;
        }
        qtd_pista[i]=0;
    }

    //criar as threads
    pthread_barrier_init(&barreira1, NULL, k+1);
    pthread_barrier_init(&barreira2, NULL, k+1);
    pthread_barrier_init(&barreira3, NULL, k+1);
    pthread_barrier_init(&barreira4, NULL, k+1);
    pthread_mutex_init(&lock, NULL);
    
    pthread_t t[k];
    for(int i=1; i<=k; i++){
        data[i].id = i;
        data[i].vel = 30;
        data[i].pos = ((k-i)/5);
        data[i].pos_geral = ((k-i)/5);
        data[i].volta = 1;
        data[i].coluna = ((k-i)/5);
        data[i].linha = (i-1)%5;
        data[i].fantasma = 0;

        qtd_pista[((k-i)/5)]++;
        pista[data[i].coluna][data[i].linha]=data[i];
        ocupados[data[i].coluna][data[i].linha]=1;
        
        pthread_create(&t[i],NULL,corrida,(void *)&data[i]);
    }

    if(ciclistas>=2){
        pthread_barrier_wait(&barreira2);
        pthread_barrier_wait(&barreira4);
    }

   
    //main
    while(ciclistas>=2){
        pthread_barrier_wait(&barreira1);

        pthread_mutex_lock(&lock);

        //avanca o tempo
        clock=clock+60;

        //desce todo mundo
        desce();

        //seta a funcao que controla as colocacoes
        int m=0;
        for(int i=d-1;i>=0;i--){
            for(int j=0;j<qtd_pista[i];j++){
                colocacao[m]=pista[i][j];
                m++;
            }            
        }


        //ordena as colocacoes
        ordena(ciclistas);

        //Ultimo lugar acabou a sua volta?
        if(colocacao[ciclistas-1].pos_geral>=volta_ult*d){
            acabou_volta=1;
            volta_ult++;
        }

        //nova volta
        if(nova_volta==1){
            if(volta_atual%2==1){
                double final=colocacao[0].pos_geral;
                int qtds=1;
                while(colocacao[qtds].pos_geral==final){
                    qtds++;
                }

                //Decide ganhador
                double parcela=100/qtds;
                int tmp=sorteia()/parcela;
                rank[qtd_terminados][0]=colocacao[tmp].id;
                if(colocacao[tmp].pos==0.5){
                    rank[qtd_terminados][1]=clock-30;
                }
                else{
                    rank[qtd_terminados][1]=clock;
                }

                //mata
                ciclistas--;
                qtd_terminados++;
                data[colocacao[tmp].id].fantasma=1;
                ocupados[data[colocacao[tmp].id].coluna][data[colocacao[tmp].id].linha]=0;

                for(int i=data[colocacao[tmp].id].linha; i<qtd_pista[data[colocacao[tmp].id].coluna]-1;i++){
                    pista[data[colocacao[tmp].id].coluna][i]=pista[data[colocacao[tmp].id].coluna][i+1];
                    ocupados[data[colocacao[tmp].id].coluna][i]=ocupados[data[colocacao[tmp].id].coluna][i+1];
                    pista[data[colocacao[tmp].id].coluna][i].linha--;
                    data[pista[data[colocacao[tmp].id].coluna][i+1].id]=pista[data[colocacao[tmp].id].coluna][i];

                }

                ocupados[data[colocacao[tmp].id].coluna][qtd_pista[data[colocacao[tmp].id].coluna]-1]=0;
                qtd_pista[data[colocacao[tmp].id].coluna]--;
        

                printf("Ganhou: %d\n",colocacao[tmp].id);

                for(int i=tmp;i<ciclistas;i++){
                    colocacao[i]=colocacao[i+1];
                }

                
            }

            nova_volta=0;
        }
        


        //imprime
        if(ciclistas>1){
            if(debug==0){//modo padrao
            
                //se acabou uma volta
                if(acabou_volta==1){
                    printf("\n** Volta %d Completada **\n",volta_ult-1);
                    printf("\nColocacao:   Id:    Localizacao:\n");
                    for(int i=0;i<ciclistas;i++){
                        if(colocacao[i].volta<volta_atual){
                            printf(" %d            %d      %.1f (RET)\n",i+1,colocacao[i].id,colocacao[i].pos);
                        }
                        else{
                            printf(" %d            %d      %.1f\n",i+1,colocacao[i].id,colocacao[i].pos);
                        }
                    }
                    acabou_volta=0;
                    printf("\n");
                }
            }
            else{//modo debug
                printa_pista();
            }
        }

        pthread_barrier_wait(&barreira3);
        if(ciclistas<2){
            break;
        }

        pthread_mutex_unlock(&lock);     
        pthread_barrier_wait(&barreira2);

        pthread_barrier_wait(&barreira4);
        if(ciclistas<2){
            break;
        }

    }


    //pthread_barrier_wait(&barreira3);


    //join threads
    for(int i=1; i<=k;i++){
        pthread_join(t[i],NULL);
    }

    //Controle do ultimo lugar
    for(int i=1;i<=k;i++){
        if(data[i].fantasma==0){
            rank[qtd_terminados][0]=i;
            if(data[i].pos_geral<(volta_atual-1)*d){
                clock=clock+(((volta_atual-1)*d-data[i].pos_geral)*2)*60;
                rank[qtd_terminados][1]=clock;
            }
            else{
                if(data[i].pos==0.5){
                    rank[qtd_terminados][1]=clock-30;
                }
                else{
                    rank[qtd_terminados][1]=clock;
                }
            }
            qtd_terminados++;
        }
    }
    



    
    
    pthread_mutex_destroy(&lock);
    pthread_barrier_destroy(&barreira1);
    pthread_barrier_destroy(&barreira2);
    pthread_barrier_destroy(&barreira3);
    pthread_barrier_destroy(&barreira4);


    //impressoes finais
    printf("\n\n*** RELATORIO FINAL ***\n\n");
    printf("Rank:    id:     Quando cruzou a linha:\n");
    for(int i=0;i<k-qtd_quebrados;i++){
        printf(" %d ",i+1);
        int tam=tamanho(i+1);
        for(int j=0;j<7-tam;j++){
            printf(" ");
        }
        printf(" %d ",rank[i][0]);
        tam=tamanho(rank[i][0]);
        for(int j=0;j<7-tam;j++){
            printf(" ");
        }
        printf(" %d \n",rank[i][1]);

    }

    if(qtd_quebrados>0){
        printf("\n\n* Quebrados *\n");
        printf("id:      Qual volta quebrou:\n");
        for(int i=0;i<qtd_quebrados;i++){
            printf(" %d",quebrados[i][0]);
            int tam=tamanho(quebrados[i][0]);
            for(int j=0;j<8-tam;j++){
                printf(" ");
            }
            printf(" %d \n",quebrados[i][1]);
        }
    }
    

    printf("\n\n");

    return 0;
}
