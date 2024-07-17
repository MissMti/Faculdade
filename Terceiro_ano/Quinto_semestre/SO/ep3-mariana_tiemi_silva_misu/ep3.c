#define _GNU_SOURCE
#include <stdio.h> 
#include <time.h> 
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h> 

#include <pthread.h>

#include "ep3.h"

#define MLetra 1000 //Maximo de char na comando
#define MCom 100 //Maximo de parametro
#define MB  1048576 //Tamanho de um MB
#define Tb 4096 //tamanho de um blcoo
#define Tbr 3744 //tamanho de um bloco além das infos
#define int int32_t

FILE* arq;
char aux[MB];


//pegar horario
void Horario(int* data){
    time_t a=time(NULL);
    struct tm tm = *localtime(&a);

    data[0]=tm.tm_mday;
    data[1]=tm.tm_mon+1;
    data[2]=tm.tm_year+1900;
    data[3]=tm.tm_hour;
    data[4]=tm.tm_min;
    data[5]=tm.tm_sec;
}



//Pegar e colocar dados
void get_criou(int bloco,int* data){//qnd criou
    fseek(arq,Tb*bloco+8,SEEK_SET);
    fread(&data,sizeof(int),6,arq);
}

void get_modif(int bloco,int* data){//qnd modificou
    fseek(arq,Tb*bloco+32,SEEK_SET);
    fread(&data,sizeof(int),6,arq);
}

void get_acessou(int bloco,int* data){//qnd acessou
    fseek(arq,Tb*bloco+56,SEEK_SET);
    fread(&data,sizeof(int),6,arq);
}

void set_horarios(int bloco){//qnd criou atualiza todos os horarios
    int data[10];
    Horario(data);
    fseek(arq,Tb*bloco+8,SEEK_SET);
    fwrite(&data,sizeof(int),6,arq);
    fwrite(&data,sizeof(int),6,arq);
    fwrite(&data,sizeof(int),6,arq);
}

void set_modificacao(int bloco){//qnd modifica atualiza
    int data[10];
    Horario(data);
    fseek(arq,Tb*bloco+32,SEEK_SET);
    fwrite(&data,sizeof(int),6,arq);
    fwrite(&data,sizeof(int),6,arq);
}

void set_acesso(int bloco){//qnd acessa atualiza
    int data[10];
    Horario(data);
    fseek(arq,Tb*bloco+56,SEEK_SET);
    fwrite(&data,sizeof(int),6,arq);
}

int get_arq(int bloco){//arq=1 e dir =2
    int eh;
    fseek(arq,Tb*bloco,SEEK_SET);
    fread(&eh,sizeof(int),1,arq);
    set_acesso(bloco);
    return eh;
}

void set_arq(int bloco,int eh){//arq=1 e dir =2
    fseek(arq,Tb*bloco,SEEK_SET);
    fwrite(&eh,sizeof(int),1,arq);
    set_modificacao(bloco);
}

int get_desp(int bloco){//qtd de espaco desp no bloco
    int eh;
    fseek(arq,Tb*bloco+4,SEEK_SET);
    fread(&eh,sizeof(int),1,arq);
    set_acesso(bloco);
    return eh;
}

void set_desp(int bloco,int eh){//qtd de espaco desp no bloco
    fseek(arq,Tb*bloco+4,SEEK_SET);
    fwrite(&eh,sizeof(int),1,arq);
    set_modificacao(bloco);
}

int get_tam(int bloco){//tam do bloco
    int tam;
    fseek(arq,Tb*bloco+80,SEEK_SET);
    fread(&tam,sizeof(int),1,arq);
    set_acesso(bloco);
    return tam;
}

void set_tam(int bloco,int eh){//tam do bloco
    fseek(arq,Tb*bloco+80,SEEK_SET);
    fwrite(&eh,sizeof(int),1,arq);
    set_modificacao(bloco);
}

int get_dentro(int bloco){//qtd dentro do dir
    int qtd_dentro;
    fseek(arq,Tb*bloco+84,SEEK_SET);
    fread(&qtd_dentro,sizeof(int),1,arq);
    set_acesso(bloco);
    return qtd_dentro;
}

void set_dentro(int bloco,int eh){//qtd do dir
    fseek(arq,Tb*bloco+84,SEEK_SET);
    fwrite(&eh,sizeof(int),1,arq);
    set_modificacao(bloco);
}

int get_pai(int bloco){//qm eh o bloco pai
    int pai;
    fseek(arq,Tb*bloco+88,SEEK_SET);
    fread(&pai,sizeof(int),1,arq);
    set_acesso(bloco);
    return pai;
}

void set_pai(int bloco,int eh){//qm eh o bloco pai
    fseek(arq,Tb*bloco+88,SEEK_SET);
    fwrite(&eh,sizeof(int),1,arq);
    set_modificacao(bloco);
}

int get_cont(int bloco){//sim=2 nao=1
    int cont;
    fseek(arq,Tb*bloco+92,SEEK_SET);
    fread(&cont,sizeof(int),1,arq);
    set_acesso(bloco);
    return cont;
}

void set_cont(int bloco,int eh){//sim=2 nao=1
    fseek(arq,Tb*bloco+92,SEEK_SET);
    fwrite(&eh,sizeof(int),1,arq);
    set_modificacao(bloco);
}

char* get_nome(int bloco){//nome
    char* nome;
    fseek(arq,Tb*bloco+96,SEEK_SET);
    fread(&nome,sizeof(char),256,arq);

    set_acesso(bloco);
    return nome;
}

//atuliza bitmap livre=0 ocupado=1
void set_bitmap(int bloco,int valor){
    fseek(arq,Tb+4*bloco,SEEK_SET);
    fwrite(&valor,sizeof(int),1,arq);
}

//Devolve bitmap
int get_bitmap(int bloco){
    int valor;
    fseek(arq,Tb+4*bloco,SEEK_SET);
    fread(&valor,sizeof(int),1,arq);
    return valor;
}

//atualiza FAT
void set_fat(int bloco,int valor){
    fseek(arq,Tb*26+4*bloco,SEEK_SET);
    fwrite(&valor,sizeof(int),1,arq);
}

//atualiza FAT
int get_fat(int bloco){
    int valor;
    fseek(arq,Tb*26+4*bloco,SEEK_SET);
    fread(&valor,sizeof(int),1,arq);
    return valor;
}

//bloco 0 qtd dir
void set_qtd_dir(int valor){
    fseek(arq,0,SEEK_SET);
    fwrite(&valor,sizeof(int),1,arq);
}

int get_qtd_dir(){
    int valor;
    fseek(arq,0,SEEK_SET);
    fread(&valor,sizeof(int),1,arq);
    return valor;
}

//bloco 0 qtd arq
void set_qtd_arq(int valor){
    fseek(arq,4,SEEK_SET);
    fwrite(&valor,sizeof(int),1,arq);
}

int get_qtd_arq(){
    int valor;
    fseek(arq,4,SEEK_SET);
    fread(&valor,sizeof(int),1,arq);
    return valor;
}

//bloco 0 qtd esp livre
void set_esp_livre(int valor){
    fseek(arq,8,SEEK_SET);
    fwrite(&valor,sizeof(int),1,arq);
}

int get_esp_livre(){
    int valor;
    fseek(arq,8,SEEK_SET);
    fread(&valor,sizeof(int),1,arq);
    return valor;
}

//bloco 0 qtd esp desp
void set_esp_desp(int valor){
    fseek(arq,12,SEEK_SET);
    fwrite(&valor,sizeof(int),1,arq);
}

int get_esp_desp(){
    int valor;
    fseek(arq,12,SEEK_SET);
    fread(&valor,sizeof(int),1,arq);
    return valor;
}



//Impressoes
void printa_bloco(int bloco){//para debbugar
    int tmp;
    fseek(arq,bloco*Tb,SEEK_SET);

    for (int i=0;i<1024;i++){
        fread(&tmp,sizeof(int),1,arq);
        printf("%d ",tmp);
        if((i+1)%16==0){
            printf("\n");
        }
    }
}

void aux_imprime_arv(int bloco,int altura){//recursivo
    int eh=get_arq(bloco);
    char ntmp [256]={0};

    fseek(arq,Tb*bloco+96,SEEK_SET);
    fread(&ntmp,sizeof(char),256,arq);

    if(bloco==51){
        printf("\n/ \n");
    }
    else{
        for(int i=1;i<altura;i++){
            printf("|     ");
        }
        printf("└──────");
        printf("%s",ntmp);
        if(eh==2){
            printf("/");
        }
        printf("\n");
    }

    int tmp;
    altura++;

    for(int i=0;i<get_dentro(bloco);i++){
        fseek(arq,bloco*Tb+356+8*i,SEEK_SET);
        fread(&tmp,sizeof(int),1,arq);
        aux_imprime_arv(tmp,altura);
    }
    printf("\n");

    set_acesso(bloco);
}

void imprime_arv(){

    //bloco 51 é a raiz
    printf("\n");

    aux_imprime_arv(51,0);

}

void imprime_nome(int bloco){
    char nome_pai[256]={0};
    fseek(arq,Tb*bloco+96,SEEK_SET);
    fread(&nome_pai,sizeof(char),256,arq);
    printf("nome: %s\n",nome_pai); 
}



//Auxiliares
//Para a insercao dos dados pos-criacao de um bloco
void Insere_dados(int bloco, int eh_arq, int desp, int tam, int qtd_dentro, int pai, int cont){
    fseek(arq,bloco*Tb,SEEK_SET);

    fwrite(&eh_arq,sizeof(int),1,arq);//eh arq ou dir? arq=1 dir=2

    fwrite(&desp,sizeof(int),1,arq);//esp desperdicado

    int data[10];
    Horario(data);
    fwrite(data,sizeof(int),6,arq);//qnd criou? (6 int)
    fwrite(data,sizeof(int),6,arq);//qnd modificou? (6 int)
    fwrite(data,sizeof(int),6,arq);//qnd acessou? (6 int)

    fwrite(&tam,sizeof(int),1,arq);//tam? p arq 

    fwrite(&qtd_dentro,sizeof(int),1,arq);//qtd dentro? p dir  

    fwrite(&pai,sizeof(int),1,arq);//bloco que está arm (pai) (int) (no caso, como esse é o pai entao -1)

    fwrite(&cont,sizeof(int),1,arq);//tem continuacao?(int) sim(2) nao(1)
}

//vê no bitmap qual é o prox bloco livre e atualiza bitmap e fat
int prox_livre(){
    int tmp=0,eh_livre,resp;
    fseek(arq,Tb,SEEK_SET);
    while(1){
        fread(&eh_livre,sizeof(int),1,arq);

        if(eh_livre==0){
            resp=tmp;
            break;
        }
        tmp++;
    }

    //atualiza bitmap
    set_bitmap(resp,1);

    //atualiza fat
    set_fat(resp,-1);

    return resp;
}

//devolve o inteiro que esta o bloco interessado
int encontra_caminho(char* destino){
    //Separando todos os diretorios até chegar no arquivo
    
    
    char* caminho[MCom];
    int i=0;
	while(1) { 
		caminho[i]= strsep(&destino, "/"); 
		if (caminho[i]== NULL){
			break; 
		}
        i++;				
	} 

    
    caminho[0]="/";


    //Do Bloco 51 encontrar os outros
    int bloco=51,tmp;

    //ult acesso no bloco 51
    set_acesso(bloco);

    
    for(int j=1;j<i;j++){//comeca no 1, pois jah esta dentro do / (bloco 51)
        //encontrar o proximo a ir
        int qtd_coisa,encontrou=0;
        qtd_coisa=get_dentro(bloco);
        
        for(int k=0;k<qtd_coisa;k++){
            fseek(arq,bloco*Tb+356+8*k,SEEK_SET);
            fread(&tmp,sizeof(int),1,arq);

            char atual[256]={0};
            char nome[256]={0};
            
            strncpy(atual,caminho[j],strlen(caminho[j]));


            fseek(arq,tmp*Tb+96,SEEK_SET);
            fread(&nome,sizeof(char),256,arq);
            
            if(!strcmp(atual,nome)){
                encontrou=1;
                bloco=tmp;
            }
        }

        if(!encontrou){
            //se nao encontrou ver se tem proximo
            int prox;
            prox=get_cont(bloco);
            j--;
            if(prox==2){
                //ver no fat o proximo
                bloco=get_fat(bloco);
            }
            else{
                return bloco;
            }
        }

        //ult acessou
        set_acesso(bloco);

    }

    //achar o bloco mais fundo do dir
    while(get_cont(bloco)==2){
        bloco=get_fat(bloco);
    }

    return bloco;
}

void copia_nome(int bloco,int prox){
    char nome_pai[256]={0};
    fseek(arq,Tb*bloco+96,SEEK_SET);
    fread(&nome_pai,sizeof(char),256,arq);
        
    fseek(arq,Tb*prox+96,SEEK_SET);//coloca nome no novo
    fwrite(&nome_pai,sizeof(char),256,arq);
}

//se inscreve no diretorio e atualiza a qtd no dir
void aux_inscreve_dir(int dir, int bloco, int eh){
    int tmp=get_dentro(dir);
    fseek(arq,Tb*dir+352+8*tmp,SEEK_SET);
    fwrite(&eh,sizeof(int),1,arq);
    fwrite(&bloco,sizeof(int),1,arq);

    tmp++;
    set_dentro(dir,tmp);

    set_modificacao(dir);
}

int inscreve_dir(int bloco, int prox, int eh){

    //ver se tem espaco no bloco pra colocar
    int tmp=get_dentro(bloco);

    if(tmp<468){//se tem,jah se inscreve
        aux_inscreve_dir(bloco,prox,eh);//int dir, int bloco, int arq
        
    }
    else{//se n, conecta com novo e se inscreve nele

        //tem continuacao no bloco
        set_cont(bloco,2);

        int prox_dir=prox_livre();
        int m=get_esp_livre();
        m--;
        set_esp_livre(m);

        //insere no prox livre infos
        Insere_dados(prox_dir,2,0,0,1,bloco,1);//(int bloco, int eh_arq, int desp, int tam, int qtd_dentro, int pai, int cont)
        
        //preciso fazer o nome...
        copia_nome(bloco,prox_dir);
        
        //atualiza fat
        set_fat(bloco,prox_dir);

        //insere
        aux_inscreve_dir(prox_dir,prox,eh);//dir,bloco,arq

        set_acesso(bloco);
    
        bloco=prox_dir;

    }

    //dire atual: data da ult modificacao e acesso
    set_modificacao(bloco);
    return bloco;
}



//Funcoes
void desmonta(){
    fseek(arq,0,SEEK_SET);
    fclose(arq);
}

void status(){
    fseek(arq,0,SEEK_SET);
    int tmp;
    fread(&tmp,sizeof(int),1,arq);
    printf("Quantidade de Diretórios: %d\n",tmp);
    fread(&tmp,sizeof(int),1,arq);
    printf("Quantidade de Arquivos: %d\n",tmp);
    fread(&tmp,sizeof(int),1,arq);
    printf("Espaço Livre (em bloco): %d\n",tmp);
    fread(&tmp,sizeof(int),1,arq);
    printf("Espaço Desperdiçado (em bytes): %d\n",tmp);
}

void lista(char** parametro){
    printf("\n");

    int bloco=encontra_caminho(parametro[1]);

    for(int i=0;i<get_dentro(bloco);i++){
        fseek(arq,Tb*bloco+356+8*i,SEEK_SET);

        int tmp;
        fread(&tmp,sizeof(int),1,arq);
        char nome[256]={0};
        fseek(arq,Tb*tmp+96,SEEK_SET);
        fread(&nome,sizeof(char),256,arq);
        printf("%s",nome);
        if(get_arq(tmp)==2){
            printf("/\n");
        }
        else{
            printf("\nTamanho em bytes: %d\n",get_tam(tmp));
        }

        int data[20];
        fseek(arq,Tb*tmp+8,SEEK_SET);
        fread(&data,sizeof(int),18,arq);

        printf("Data de criação: %d/%d/%d   %d:%d:%d\n",data[0],data[1],data[2],data[3],data[4],data[5]);
        printf("Data de última modificação: %d/%d/%d   %d:%d:%d\n",data[6],data[7],data[8],data[9],data[10],data[11]);
        printf("Data do último acesso: %d/%d/%d   %d:%d:%d\n",data[12],data[13],data[14],data[15],data[16],data[17]);
        printf("\n");

    }

}

void apaga(char** parametro){

    char atual[256]={0};
    int at=encontra_caminho(parametro[1]);


    //pegar nome do arquivo
    fseek(arq,Tb*at+96,SEEK_SET);
    fread(&atual,sizeof(char),255,arq);

    int tmp,k;
    int bloco=get_pai(at);
    //apaga no pai
    for(int i=0;i<get_dentro(bloco);i++){
        set_acesso(bloco);
        fseek(arq,bloco*Tb+356+8*i,SEEK_SET);
        fread(&tmp,sizeof(int),1,arq);
        
        char nome[256]={0};


        fseek(arq,tmp*Tb+96,SEEK_SET);
        fread(&nome,sizeof(char),256,arq);
            
        if(!strcmp(atual,nome)){
            fseek(arq,bloco*Tb+352+8*i,SEEK_SET);
            int zero[10]={0};
            fwrite(&zero,sizeof(int),2,arq);
            
            break;
        }

        if(i==468){
            bloco=get_fat(bloco);
            i=-1;
        }


    }

    //apaga conteudo
    int ale[2000]={0};
    while(1){
        fseek(arq,at*Tb,SEEK_SET);
        int aux=get_cont(at);
        fwrite(&ale,sizeof(int),1024,arq);
        if(aux==1){
            break;
        }
        at=get_fat(at);
    }

}

void toca(char** parametro){

    int bloco=encontra_caminho(parametro[1]);

    if(get_arq(bloco)==1){//eh arq
        set_acesso(bloco);
    }
    else{//eh diretorio
        int prox=prox_livre();
        int m=get_esp_livre();
        m--;
        set_esp_livre(m);
        bloco=inscreve_dir(bloco,prox,1);
        Insere_dados(prox,1,Tbr,0,0,bloco,1);
    }
}

void mostra(char** parametro){

    int bloco=encontra_caminho(parametro[1]);

    char c;


    while(1){
        //le conteudo desse bloco
        int tam=get_tam(bloco);

        fseek(arq,Tb*bloco+352,SEEK_SET);
        for(int i=0;i<tam;i++){
            fread(&c,sizeof(char),1,arq);
            printf("%c",c);
        }

        //atualiza o acesso
        set_acesso(bloco);

        if(get_cont(bloco)==1){
            break;
        }

        //pegar o proximo
        bloco=get_fat(bloco);
    }

}

void criadir(char** parametro){
    char* caminho[MCom];
    int i=0;
	while(1) { 
		caminho[i]= strsep(&parametro[1], "/"); 
		if (caminho[i]== NULL){
			break; 
		}
        i++;				
	} 

    
    caminho[0]="/";
    int qtd=i;


    //Do Bloco 51 encontrar os outros
    int bloco=51,tmp;

    //ult acesso no bloco 51
    set_acesso(bloco);

    
    for(int j=1;j<i;j++){//comeca no 1, pois jah esta dentro do / (bloco 51)
        //encontrar o proximo a ir
        int qtd_coisa,encontrou=0;
        qtd_coisa=get_dentro(bloco);
        
        for(int k=0;k<qtd_coisa;k++){
            fseek(arq,bloco*Tb+356+8*k,SEEK_SET);
            fread(&tmp,sizeof(int),1,arq);


            char atual[256]={0};
            char nome[256]={0};
            
            strncpy(atual,caminho[j],strlen(caminho[j]));


            fseek(arq,tmp*Tb+96,SEEK_SET);
            fread(&nome,sizeof(char),256,arq);
            
            if(!strcmp(atual,nome)){
                encontrou=1;
                bloco=tmp;
            }
        }

        if(!encontrou){
            //se nao encontrou ver se tem proximo
            int prox;
            prox=get_cont(bloco);
            j--;
            if(prox==2){
                //ver no fat o proximo
                bloco=get_fat(bloco);
            }
            else{
                break;
            }
        }

        //ult acessou
        set_acesso(bloco);

    }

    //achar o bloco mais fundo do dir
    while(get_cont(bloco)==2){
        bloco=get_fat(bloco);
    }

    //bloco 0
    fseek(arq,0,SEEK_SET);
    tmp=get_qtd_dir();
    tmp++;
    set_qtd_dir(tmp);

    fseek(arq,8,SEEK_SET);
    tmp=get_esp_livre();
    tmp--;
    set_esp_livre(tmp);

    int prox=prox_livre();
    int m=get_esp_livre();
    m--;
    set_esp_livre(m);

    bloco=inscreve_dir(bloco,prox,2);

    //colocar as info no prox
    Insere_dados(prox,2,0,0,0,bloco,1);

    //insere nome
    char nome[256]={0};
    strncpy(nome,caminho[qtd-1],strlen(caminho[qtd-1]));

    fseek(arq,Tb*prox+96,SEEK_SET);
    fwrite(&nome,sizeof(char),256,arq);

    return;
}

void copia (char** parametro){

    //abre arquivo de origem
    FILE* origem;
    origem=fopen(parametro[1],"r");
    if (origem== NULL){
        printf("Não existe ou deu erro para abrir o arquivo\n");
        return;
    }

    //tamanho do arquivo de origem (pra saber quantos blocos precisaremos..)
    fseek(origem,0L,SEEK_END);
    long long tam_origem= ftell(origem);

    //encontra destino
    int bloco_destino=encontra_caminho(parametro[2]);

    //temos espaco livre o suficiente?
    int espaco_livre,blocos_precisos,resto;
    espaco_livre=get_esp_livre();

    if(espaco_livre<tam_origem/Tbr){
        printf("n há espaco o suficiente\n");
        return;
    }
    else{//se sim, quantos blocos vamos precisar?
        blocos_precisos=tam_origem/Tbr;
        resto=tam_origem%Tbr;
        if(resto>0){
            blocos_precisos++;
        }
    }

    //atualiza espaco livre, qtd de arq no bloco 0 e qtd de esp desp
    int tmp=get_qtd_arq();
    tmp++;
    set_qtd_arq(tmp);

    tmp=get_esp_livre()-blocos_precisos;
    set_esp_livre(tmp);

    tmp=get_esp_desp()+Tbr-resto;
    set_esp_desp(tmp);
    

    int prox=prox_livre();
    bloco_destino=inscreve_dir(bloco_destino,prox, 1);

    //metadados + conteudo do arq
    int desp,eh_cont,tamanho;
    char c;
    fseek(origem,0,SEEK_SET);

    for(int j=0;j<blocos_precisos;j++){
        desp=0;
        eh_cont=2;
        tamanho=Tbr;
        if(j==blocos_precisos-1){
            desp=Tbr-resto;
            eh_cont=1;
            tamanho=resto;
        }
        Insere_dados(prox,1,desp,tamanho,0,bloco_destino,eh_cont);

        char n[256]={0};
        strncpy(n,parametro[1],strlen(parametro[1]));

        fseek(arq,Tb*prox+96,SEEK_SET);//coloca nome no novo
        fwrite(&n,sizeof(char),256,arq);

        //insere conteudo
        fseek(arq,Tb*prox+352,SEEK_SET);
        for(int i=0;i<tamanho;i++){
            fread(&c,sizeof(char),1,origem);
            fwrite(&c,sizeof(char),1,arq);
        }

        set_modificacao(prox);

        if(j==blocos_precisos-1){
            break;
        }

        //atualiza prox
        bloco_destino=prox;
        prox=prox_livre();

        set_fat(bloco_destino,prox);
    }

   fclose(origem);

}

void monta(char** parametro){

    arq=fopen(parametro[1],"rb+");

    if(arq==NULL){
        arq=fopen(parametro[1],"wb+");
        
        for(int i=0;i<100;i++){
            fwrite(&aux,sizeof(char),MB,arq);
        }

        //Bloco 0 para metadados gerais do status
        fseek(arq,0,SEEK_SET);
        int tmp=1;
        fwrite(&tmp,sizeof(int),1,arq);//qtd de dir
        tmp=0;
        fwrite(&tmp,sizeof(int),1,arq);//qtd de arq
        tmp=25648;
        fwrite(&tmp,sizeof(int),1,arq);//qtd de esp livre (25600-52)
        tmp=0;
        fwrite(&tmp,sizeof(int),1,arq);//qtd de esp desperdicado
       

        //Bloco 1 a 25 para Bitmap
        fseek(arq,Tb,SEEK_SET);
        tmp=1;
        for(int i=0;i<52;i++){
            fwrite(&tmp,sizeof(int),1,arq);//qtd de dir
        }

        //Bloco 26 a 50 para FAT
        fseek(arq,26*Tb,SEEK_SET);
        tmp=-1;
        for(int i=0;i<52;i++){
            fwrite(&tmp,sizeof(int),1,arq);//qtd de dir
        }

        //Bloco 51 para primeiro dir "/"
        char nome_tmp[256]="/";

        Insere_dados(51,2,0,0,0,-1,1);

        fwrite(&nome_tmp,sizeof(char),256,arq);//nome(256 chars)
        
    }
    else{
        imprime_arv();
    }
}



// Processa comandos
int Comandos(char* comando, char** parametro){
    //le linha
    char* a = readline(""); 
	if(strlen(a)==0){
		return 0;
	}

	strcpy(comando, a); 

    // para separar os parametros do comando
	for (int i = 0; i < MCom; i++) { 
		parametro[i]= strsep(&comando, " "); 
		if (parametro[i]== NULL){
			break; 
		}				
	} 

    char* Com[20];
    Com[0] = "monta";
	Com[1] = "copia"; 
    Com[2] = "criadir";
    Com[3] = "apagadir";
	Com[4] = "mostra"; 
    Com[5] = "toca"; 
    Com[6] = "apaga";
	Com[7] = "lista"; 
    Com[8] = "atualizadb"; 
    Com[9] = "busca";
	Com[10] = "status"; 
    Com[11] = "desmonta"; 
    Com[12] = "sai"; 

	int tmp;
	for (int i = 0; i < 13; i++) { 
		if (!strcmp(parametro[0], Com[i])) { 
            tmp=i;
		} 
	} 

    if(tmp==0){
        monta(parametro);
    }
    if(tmp==1){
        copia(parametro);
    }
    if(tmp==2){
        criadir(parametro);
    }
    if(tmp==3){
        //apagadir(parametro);
    }
    if(tmp==4){
        mostra(parametro);
    }
    if(tmp==5){
        toca(parametro);
    }
    if(tmp==6){
        apaga(parametro);
    }
    if(tmp==7){
        lista(parametro);
    }
    if(tmp==8){
        //atualizadb();
    }
    if(tmp==9){
        //busca();
    }
    if(tmp==10){
        status();
    }
    if(tmp==11){
        desmonta();
    }
    if(tmp==12){
        return 1;
    }

	return 0;
}


int main() { 

	char comando[MLetra];
    char* parametros [MCom];
    int ref;

	while (1) { 
		// Imprime prompt
		printf("{ep3}: ");

        //Lê e processa comando
		ref=Comandos(comando,parametros);

        if(ref==1){ //sai do shell, com sai
            break;
        }
	} 

	return 0; 

} 