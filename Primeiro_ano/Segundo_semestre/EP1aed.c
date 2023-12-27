#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void conjecturacollatz (unsigned long int inicio,unsigned long int fim,int* npassos){

unsigned long int i,atual;
int numpatual;

/*Fazer um loop para descobrir o numero de passos de todo o intervalo*/
for (i=inicio;i<=fim;i++){
	
	atual=i;
	numpatual=0;
	
	while(atual != 1){
	  
	  /*Verificar se o valor do "atual" ja é um valor com o numero de passos encontrados previamente*/
	  
 	  if(atual<i && atual>=inicio){
 	 	numpatual=numpatual+npassos[atual-inicio];
	 	atual=1;
	 	
	  }
	  
	  /*Caso nao, continuar a funcao da conjectura de forma normal*/
	  else{
	    if(atual%2==0){
	   	 atual = atual/2;
  		 numpatual++;
  		 
	    }
	    else{
		
		 atual = (3*atual) + 1;
		 numpatual++;	
		 	
	    }
	  }
	}
	
	/*Coloca no vetor o numero de passos para i*/
	npassos[i-inicio]=numpatual;
	
}

}







int main(){
unsigned long int i,f;
unsigned long int j;
double tempo;
FILE* arq;
int* npassos;
arq= fopen("quantidadedepassos.txt","w");

/*Solicitando informacoes para o usuario*/
printf("\n	Por favor insira o valor de i:");
scanf(" %lu",&i);
printf("	Agora, por favor insira o valor de f:");
scanf(" %lu",&f);

/*Alocando memoria para guardar o numero de passos de todos os numeros no intervalo*/
npassos = malloc(sizeof(int)*(f-i+1));


/*Comeca a contagem de tempo*/
tempo=clock();

/*Chamando a funcao*/
conjecturacollatz(i,f,npassos);

/*Termina a contagem de tempo e imprime o tempo gasto na funcao em segundos*/
tempo=(clock()-tempo)/CLOCKS_PER_SEC;
printf("\n	O tempo para realizar a funcao foi de %f s \n\n",tempo);

/*Imprime os resultados em um arquivo separado*/
for (j=i;j<=f;j++){
	fprintf(arq,"O numero de passos necessarios para chegar em %lu foram: %d\n",j,npassos[j-i]);
}
fclose(arq);


/*Liberta a memoria alocada*/
free(npassos);
npassos=NULL;

return 0;

}
