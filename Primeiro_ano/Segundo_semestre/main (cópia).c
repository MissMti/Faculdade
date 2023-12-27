#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX 200000


typedef struct cel {
  char palavra[100];
  long linhas[MAX]; /*quantas linhas no maximo tem o texto*/
  long freq[MAX];
  long qtd_linhas;
  int size;
  struct cel * esq;
  struct cel * dir; 
} no;

no * busca (no * raiz, char * x);
no * insercao (no * raiz, char * chave, long y, int tamanho);
void inordem (no *raiz);


int main(int argc, char **argv){

	no* p=NULL;
	/*no* aux=NULL;*/
	
	char tmp[100];
	char verifica;
	long i,j,k;
	long linha=1;
	FILE *file=fopen(argv[1],"r");
	j=fscanf(file,"%c",&verifica);
	
	while(j!=-1){
		i=1;
		
		if(j==1){
			if((verifica>='A' && verifica<='Z')||(verifica>='a' && verifica<='z')){/*verifica/encontra palavras no texto*/
				tmp[0]=verifica;
				fscanf(file,"%c",&verifica);
				while((verifica>='A' && verifica<='Z')||(verifica>='a' && verifica<='z')){
					tmp[i]=verifica;
					i++;/*conta o tamanho da palavra*/
					fscanf(file,"%c",&verifica);
				}
				
				
				/*A partir daqui temos uma nova palavra*/
				/*aux = busca(p, tmp);*/
				
				p = insercao(p, tmp, linha, i);
				
				
				
				for(k=0;k<i;k++){
					tmp[k]='\0';
				}
				
				
			}
		}
		
		if(verifica=='\n'){
			linha++;
		}
		
		j=fscanf(file,"%c",&verifica);
		
	}
	inordem(p);
	
	free(p);
	fclose(file);
	return 0;

}

no * busca (no * raiz, char * x){
  if (raiz == NULL || strcmp(raiz->palavra, x) == 0)
    return raiz;
  if (strcmp(x, raiz->palavra) < 0)
    return busca (raiz->esq, x);
  return busca (raiz->dir, x);
}


no * insercao (no * raiz, char * x, long y, int tamanho){
  /*long k;*/
  char tmp[100];
  if (raiz == NULL){
    raiz = malloc (sizeof(no));
    strcpy(raiz->palavra,"");
    strcpy(raiz->palavra,x);
    raiz->linhas[0] = y;
    raiz->freq[0] = 1;
    raiz-> qtd_linhas = 1;
    raiz->size=tamanho;
    raiz->dir = raiz->esq = NULL;
    return raiz;
  }
  
  
  strcpy(tmp,x);

  if (strcmp(tmp, raiz->palavra) == 0){
  	if(raiz->linhas[raiz->qtd_linhas-1]==y){/*msm linha*/
  		raiz->freq[raiz->qtd_linhas-1] = raiz->freq[raiz->qtd_linhas-1]+1;
  	}
  	else{/*linhas diferentes*/
  		raiz->linhas[raiz->qtd_linhas] = y;
  		raiz->freq[raiz->qtd_linhas] = 1;
    		raiz-> qtd_linhas = raiz-> qtd_linhas+1;
  	}
  
  }
  
  else if (strcmp(tmp, raiz->palavra) < 0)
    raiz->esq = insercao (raiz->esq, x, y,tamanho);
  else if (strcmp(tmp, raiz->palavra) > 0)
    raiz->dir = insercao (raiz->dir, x, y,tamanho);
 
  return raiz;
}

void inordem (no *raiz) {
  long i;
  if (raiz != NULL){
    inordem (raiz->esq);
    printf("%s ", raiz->palavra);
    for(i=0;i<raiz->qtd_linhas;i++){
    	
    	printf("%ld", raiz->linhas[i]);
    	if(raiz->freq[i] != 1){
    		printf("(%ld)", raiz->freq[i]);
    	}
    	printf("  ");
    }
    
    printf("\n");
    inordem (raiz->dir);
  }
}
