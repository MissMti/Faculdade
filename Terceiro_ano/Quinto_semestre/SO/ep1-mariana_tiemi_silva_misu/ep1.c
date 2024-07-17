#define _GNU_SOURCE
#include <stdio.h> 
#include <string.h> 
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>

#include "ep1.h"

#define Quantum 3

int tp=0;

void* Gasta_Tempo(){
    int n=tp;

    //gerar afinidade
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpuset);

    struct timeval ta,tb;
    gettimeofday(&ta, NULL);
    int a,b,c=0;
    a=ta.tv_sec;
    b=a;
    while(b-a<n){
        c=c+1;
        gettimeofday(&tb, NULL);
        b=tb.tv_sec;
    }
    return 0;
}

int min(int a, int b){
    if(a<b){
        return a;
    }
    return b;
}

int max(int a, int b){
    if(a>b){
        return a;
    }
    return b;
}

void troca(int* A, int* C){
    int B[5];
    B[0]=A[0];
    B[1]=A[1];
    B[2]=A[2];
    B[3]=A[3];
    A[0]=C[0];
    A[1]=C[1];
    A[2]=C[2];
    A[3]=C[3];
    C[0]=B[0];
    C[1]=B[1];
    C[2]=B[2];
    C[3]=B[3];
}

int quant(int dl, int t){
    return max(1,5-(dl-t)/5);
}

void EcP(FILE* a, FILE* b, int n){
    int i,x,y,z,p=0;
    char nome[20];

    //leitura
    int A[n+5][5];
    char B[n+5][20];
    for (i = 1; i <= n; i++) {
        fscanf(a,"%s %d %d %d", nome, &z, &x, &y);
        A[i][3] = z;//t0
        A[i][2] = x;//t0
        A[i][1] = y;//dt
        A[i][0] = i;//Para salvar o processo com o nome;
        strcpy(B[i], nome);
    }

    //seto as threads
    pthread_t tds ;

    int C[n+5][5];//"Fila" dos processos
    int m=0,t=0,j=1,tam=0;
    while(m<n){
        //repassa a fila atual
        for(i=1;i<=tam;i++){
            C[i][0]=C[i+1][0];
            C[i][1]=C[i+1][1];
            C[i][2]=C[i+1][2];
            C[i][3]=C[i+1][3];
        }

        //Quando a fila está vazia
        if(tam==0){
            tp=A[j][2]-t;
            Gasta_Tempo();
            t=t+tp;
            i=1;
            while(A[j][2]==t && j<=n){
                C[i][0]=A[j][0];
                C[i][1]=A[j][1];
                C[i][2]=A[j][2];
                C[i][3]=A[j][3];
                j++;
                tam++;
                i++;
            }
        }
        

        //Fazer o processo
        int q=quant(C[1][3],t);
        tp=min(C[1][1],q);
        t=t+tp;//novo t
        C[1][1]=C[1][1]-tp;

        //thread
        pthread_create(&tds,NULL,Gasta_Tempo,NULL);
        pthread_join(tds,NULL);

        for(i=j;i<=n;i++ ,j++){
            if(A[i][2]>t){
                break;
            }        
            tam++; 
            C[tam][0]=A[i][0];
            C[tam][1]=A[i][1];
            C[tam][2]=A[i][2];
            C[tam][3]=A[i][3];
        }

        if(C[1][1]==0){//esse processo acabou
            fprintf(b, "%s %d %d\n",B[C[1][0]],t-C[1][2],t);
            tam--;
            m++;
        }
        else{//esse processo ainda tah rolando
            troca(C[1],C[tam+1]);
            p++;
        }

    }

    //Linha extra
    fprintf(b,"%d\n",p);

}


void RR(FILE* a, FILE* b, int n){
    int i,x,y,z,p=0;
    char nome[20];

    //leitura
    int A[n+5][5];
    char B[n+5][20];
    for (i = 1; i <= n; i++) {
        fscanf(a,"%s %*d %d %d", nome, &x, &y);
        A[i][2] = x;//t0
        A[i][1] = y;//dt
        A[i][0] = i;//Para salvar o processo com o nome;
        strcpy(B[i], nome);
    }


    //seto as threads
    pthread_t tds;

    int C[n+5][5];//"Fila" dos processos
    int m=0,t=0,j=1,tam=0;
    while(m<n){
        //repassa a fila atual
        for(i=1;i<=tam;i++){
            C[i][0]=C[i+1][0];
            C[i][1]=C[i+1][1];
            C[i][2]=C[i+1][2];
        }

        //Quando a fila está vazia
        if(tam==0){
            tp=A[j][2]-t;
            Gasta_Tempo();
            t=t+tp;
            i=1;
            while(A[j][2]==t && j<=n){
                C[i][0]=A[j][0];
                C[i][1]=A[j][1];
                C[i][2]=A[j][2];
                j++;
                tam++;
                i++;
            }
        }


        //Fazer o processo
        tp=min(C[1][1],Quantum);
        t=t+tp;//novo t
        C[1][1]=C[1][1]-tp;

        //thread
        pthread_create(&tds,NULL,Gasta_Tempo,NULL);
        pthread_join(tds,NULL);

        for(i=j;i<=n;i++ ,j++){
            if(A[i][2]>t){
                break;
            }        
            tam++; 
            C[tam][0]=A[i][0];
            C[tam][1]=A[i][1];
            C[tam][2]=A[i][2];
        }

        if(C[1][1]==0){//esse processo acabou
            fprintf(b, "%s %d %d\n",B[C[1][0]],t-C[1][2],t);
            tam--;
            m++;
        }
        else{//esse processo ainda tah rolando
            troca(C[1],C[tam+1]);
            p++;
        }

    }

    //Linha extra
    fprintf(b,"%d\n",p);
}

void SJF(FILE* a, FILE* b, int n){
    int i,x,y,z;
    char nome[20];

    //leitura
    int A[n+5][5];
    char B[n+5][20];
    for (i = 1; i <= n; i++) {
        fscanf(a,"%s %*d %d %d", nome, &x, &y);
        A[i][2] = x;//t0
        A[i][1] = y;//dt
        A[i][0] = i;//Para salvar o processo com o nome;
        strcpy(B[i], nome);
    }

    //seto as threads
    pthread_t tds [n];

    int C[n+5][5];//"Fila" dos processos
    int m=0,t=0,j=1;//quantos processo jah foram,tempo passado e controle para indice
    int tam=0;//controle de quantos jah foram incluidos
    while(m<n){
        for(i=j;i<=n;i++ ,j++){
            if(A[i][2]>t){
                break;
            }            
            C[tam][0]=A[i][0];
            C[tam][1]=A[i][1];
            C[tam][2]=A[i][2];
            if(C[m][1]>C[tam][1]){//realiza a troca
                troca(C[m],C[tam]);
            }
            tam++;
        }

        //quando a fica esta vazia
        if(tam==m){
            tp=A[j][2]-t;
            Gasta_Tempo();
            t=t+tp;
            i=m;
            while(A[j][2]==t && j<=n){
                C[i][0]=A[j][0];
                C[i][1]=A[j][1];
                C[i][2]=A[j][2];
                if(C[m][1]>C[i][1]){
                    troca(C[m],C[i]);
                }
                j++;
                tam++;
                i++;
                
            }
        }

        //realizo o processo atual
        t=t+C[m][1];
        tp=C[m][1];

        //thread
        pthread_create(&tds[m],NULL,Gasta_Tempo,NULL);
        pthread_join(tds[m],NULL);

        fprintf(b, "%s %d %d\n",B[C[m][0]],t-C[m][2],t);
        
        //novo menor
        m++;       
        for(i=m;i<tam;i++){
            if(C[m][1]>C[i][1]){//realiza a troca
                troca(C[m],C[i]);
            }
        }
        
    }

    //Linha extra
    fprintf(b,"%d\n",0);
    
}


int main(int numargs, char *arg[]) { 
    //ver a quantidadede processos
    FILE *arqv;
    arqv = fopen (arg[2], "rt");
 
    if (arqv == NULL) {
       return 0;
    }  
    
    int qtd_de_processos=0;
    char str[50];
    
    while (fgets(str, 50, arqv) != NULL) {
        qtd_de_processos++;
    }

    fclose (arqv);

    //gera arq pra leitura e escrita
    FILE *arq;
    arq = fopen (arg[2], "rt");

    if (arq == NULL) {
       return 0;
    }  

    FILE *novo;
    novo = fopen(arg[3], "wt");

    if (novo == NULL) {
       return 0;
    }  

    //decide qual escalonador
    if(strcmp(arg[1], "1") == 0){ //Shortest Job First
        SJF(arq,novo,qtd_de_processos);
    }
    if(strcmp(arg[1], "2") == 0){ //Round-Robin
        RR(arq,novo,qtd_de_processos);
    }
    if(strcmp(arg[1], "3") == 0){ //Escalonamento com prioridade
        EcP(arq,novo,qtd_de_processos);
    }

    fclose (arq);
    fclose (novo);

    return 0;
}