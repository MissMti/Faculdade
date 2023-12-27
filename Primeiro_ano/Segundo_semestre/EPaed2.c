#include <stdio.h>
#include <stdlib.h>
#include <string.h>





typedef struct{
  char* word;
  int modo;
  int tamanho;
  int comeco;
} pilha; 





int encontra_zeros_h(int* tini,int m,int n,int aux, int* size){

	int qtd_zeros=0;

	if(tini[aux+1]!=-1 && (aux+1)%n!=0){
			 
		while((aux+1)%n!=0 && tini[aux]!=-1 && aux+1<m*n ){
			qtd_zeros++;
			//tini[aux]=1;
			aux++;
		}
		
		if(tini[aux]!=-1){
			//tini[aux]=1;
			qtd_zeros++;
		}
		
		size[0]=qtd_zeros;
		printf("\nEncontrou %d zero na horizontal\n", qtd_zeros);
		return 1;
	}
	
	return 0;
	
}




int encontra_zeros_v(int* tini,int m,int n,int aux, int* size){

	int qtd_zeros=0;
	
	if(aux-n>0){
		if(tini[aux-n]!=-1){
			return 0;
		}
	}
	

	printf("\nEncontrou no comeco em %d \n", aux);

	if(aux+n<m*n){
	
		if(tini[aux+n]!=-1){
			
			while(tini[aux]!=-1 && aux+n<m*n ){
				qtd_zeros++;
				//tini[aux]=1;
				aux=aux+n;
			}
			
			if(tini[aux]!=-1){
				//tini[aux]=1;
				qtd_zeros++;
			}
			
			size[0]=qtd_zeros;
			printf("\nEncontrou %d zero na vertical no comecoem \n", qtd_zeros);
			return 1;
		}
		
		return 0;
	}
	
	return 0;
	
}




int encontra_zero(int* modo, int* size, int* tini,int m,int n,int comeco,char* tfin){
	int i,aux=-1,veredito_h=0,veredito_v=0;
		
	for(i=comeco;i<m*n;i++){
		if(tini[i]!=-1){
			aux=i;
			break;
		}
	}
	
	if(aux==-1 || aux==(m*n)-1){
		return -1;
	}
	
	
	veredito_h = encontra_zeros_h(tini,m,n,aux,size);
	
	if(veredito_h==1){
		
		modo[0]=0;
		return aux;
	}
	
	
	veredito_v = encontra_zeros_v(tini,m,n,aux,size);
	
	if(veredito_v==1){
		modo[0]=1;
		return aux;
	}
	
	return -1;
	
}







/*int desempilha(){}

*/





void empilha(pilha* pilha,char* palavra, int mode,int tam,int start,int j){
	printf("\n c3:%d \n",start);
	printf("\n na pilha:%d \n",start);
	printf("\n na pilha:%d \n",mode);
	printf("\n na pilha:%d \n",tam);	
	pilha[j].word = palavra;
	pilha[j].modo = mode;
	pilha[j].tamanho = tam;
	pilha[j].comeco = start;
}





int compara(pilha* pilha,char* palavra,int tamanho){
	int i;
	
	for(i=0;i<tamanho;i++){
		if(pilha[i].word==palavra){
			return 0;
		}
	}
	
	return 1;
	
}


int letra_por_letra(char* palavra,char* tf,int posicao_t,int posicao_p){
	if(palavra[posicao_p]==tf[posicao_t]){
		return 1;
	}
	return 0;
}




int verificador(char* palavra, int tamanho, int modo, int* ti,char *tf,int n,int comeco){
	
	int i,confere;
	if(modo==0){
		for(i=comeco;i<tamanho;i++){
			if(ti[i]==1){
		 		printf("tini[%d]=%d",i,ti[i]);
				confere=letra_por_letra(palavra,tf,i,tamanho-comeco);
				printf("\n\nconfere%d,%d\n\n",confere,tamanho-comeco);
				if (confere==0){
					
					return 0;
				}
			}
		}
	}
	else{
		for(i=comeco;i<tamanho*n;i=i+n){
			if(ti[i]==1){
				printf("tini[%d]=%d",i,ti[i]);
				confere=letra_por_letra(palavra,tf,i,(i-1)/n);
				printf("\n\nconfere%d,%d\n\n",confere,(i-1)/n);
				if (confere==0){
					return 0;
				}
			}
		}
	}
	
	return 1;
}




void completa_tabela(char* palavra,char* tf,int modo,int tamanho,int comeco,int m, int n){
	int i;
	
	if(modo==0){
		for(i=0;i<tamanho;i++){
			tf[comeco+i]=palavra[i];
			tf[comeco+i]=1;
		}
	}
	
	if(modo==1){
		for(i=0;i<tamanho;i++){
			tf[comeco]=palavra[i];
			tf[comeco]=1;
			comeco=comeco+n;
		}
	}
}




int encontra_palavra(int num, char*** p, int* qtd_p,int max,int* ti,char* tf, int m, int n,int modo, int* size,int comeco, pilha* pilha,int* tam_pilha){
	int i,j,tamanho,novo,verificado=0;
	
	
	
	tamanho=size[0];
	
	if(qtd_p[tamanho]!=0){
		for(i=0;i<qtd_p[tamanho];i++){
			novo=compara(pilha,p[tamanho][i],tam_pilha[0]);
			verificado=verificador(p[tamanho][i],tamanho,modo,ti,tf,n,comeco);
			printf("\n verificado %d\n",verificado);
			
			if(novo==1 && verificado==1){
				completa_tabela(p[tamanho][i],tf,modo,tamanho,comeco,m,n);
				
				empilha(pilha,p[tamanho][i],modo,tamanho,comeco,tam_pilha[0]);
				
				tam_pilha[0]++;
				printf("\n tp: %d \n",tam_pilha[0]);
				break;
			}		
		}
		
		for(j=0;j<tam_pilha[0];j++){
			printf("\n %s, %d, %d, %d",pilha[j].word, pilha[j].modo, pilha[j].tamanho, pilha[j].comeco);
		}
		return 1;
	}
	for(i=0;i<tam_pilha[0];i++){
		printf("%s, %d, %d, %d",pilha[i].word,pilha[i].modo,pilha[i].tamanho, pilha[i].comeco);
	}
	return -1;
	

}	








int backtracking(int num, char*** p, int* qtd_p,int max,int* ti,char* tf, int m, int n,int* modo, int* size,int comeco, pilha* pilha, int* tam_pilha){
	
	int existe_p,tem,i,intervalo;
	
	
	existe_p=encontra_palavra(num, p,qtd_p, max, ti, tf, m, n, modo[0], size,comeco,pilha, tam_pilha);
	
	intervalo=size[0];
	
	if(existe_p==1 && modo[0]==0){
		
		for(i=comeco;i<comeco+intervalo;i++){
			printf("\n %d \n",i);
			tem=encontra_zeros_v(ti, m, n, i, size);
			printf("\n %d \n",i);
			
			if(tem==1){
				existe_p=encontra_palavra(num, p,qtd_p, max, ti, tf, m, n, 1, size,i,pilha,tam_pilha);
			}
		}
	}
	
	if(existe_p==1 && modo[0]==1){
		modo[0]=0;
		printf("entrou");
		for(i=comeco;i<comeco+size[0]*n;i=i+n){
			tem=encontra_zeros_h(ti,m,n,i,size);
			
			if(i!=0){
				if(ti[i-1]!=-1 && i%n!=0){
					tem=0;
					
				}	
			}
			
			if(tem==1){
				
				existe_p=encontra_palavra(num, p,qtd_p, max, ti, tf, m, n, 0, size,i,pilha,tam_pilha);
			}
		}
		
	}
	
	return existe_p;
	
	
}







int ultima_checagem(char* tfinal,int m,int n){
	int i;
	for(i=0;i<m*n;i++){
		if(tfinal[i]=='+'){
			return -1;
		}
	}
	return 1;
	
} 




int p_cruzadas(int num, char*** p, int* qtd_p,int tamanho_max,int* tinicial,char* tfinal, int linhas, int colunas){
	int comeco=0,veredito=0,check,i,prox=0,aux;
	int* modo;
	int* size;
	int* tam_pilha;
	pilha* pilha;
	
	modo=malloc(sizeof(int));
	size=malloc(sizeof(int));
	size[0]=0;
	tam_pilha=malloc(sizeof(int));
	pilha=malloc(sizeof(pilha)*num);
	
	tam_pilha[0]=0;
	
	for(i=0;i<num;i++){
		pilha[i].word=malloc(sizeof(char)*tamanho_max);
		pilha[i].word=NULL;
		pilha[i].modo= 0;
		pilha[i].tamanho= 0;
		pilha[i].comeco= 0;
	}
	
	while(veredito!=-1 && prox<linhas*colunas){
		aux=comeco;
		comeco=encontra_zero(modo,size,tinicial,linhas,colunas,prox,tfinal);
		if(comeco!=-1){
			veredito=backtracking(num, p, qtd_p, tamanho_max, tinicial, tfinal, linhas, colunas,modo,size,comeco,pilha, tam_pilha);
			if(modo[0]==0){
				prox=prox+size[0];
				printf("\no prox sera %d\n",prox);
			}
			else{
				prox=prox+1;
				printf("\no prox sera %d\n",prox);
			}
		}
		else{	
			prox=prox+1;
			printf("\no prox sera %d\n",prox);
		}
	}
	
	check=ultima_checagem(tfinal,linhas,colunas);
	
	free(modo);
	free(size);
	
	if(veredito==-1 || check ==-1){
		free(pilha);
		free(tam_pilha);
		return 0;
	}
	
	free(pilha);
	free(tam_pilha);
	return 1;
	
}





void leitura_tab(int linhas,int colunas,int* tinicial, char* tfinal){
int i;

for(i=0;i<linhas*colunas;i++){
	scanf("%d ",&tinicial[i]);
	
}

for(i=0;i<linhas*colunas;i++){
	if (tinicial[i]==0){
		tfinal[i]='+';
		
	}
	else {
		tfinal[i]='*';
		
	}
}

}



void leitura_palavras(int num_palavras,char*** palavras,int* qtd_palavras, int max){
int i,j,len;
char* palavra_tmp;
palavra_tmp=malloc(sizeof(char)*max);

for(i=0;i<num_palavras;i++){
	scanf(" %s",palavra_tmp);
	len = strlen(palavra_tmp);
	if (len<=max && len>=2){
		palavras[len][qtd_palavras[len]] = malloc(len*sizeof(char));
		for (j = 0; j < len; j++){
			palavras[len][qtd_palavras[len]][j] = palavra_tmp[j];
		}
		qtd_palavras[len]++;
	}	

}

free(palavra_tmp);

}





int main(){

int m,n,i,j,existe;
int instancias=1;
int condicao=0;
int num_palavras,tamanho_max;
int* tab_inicial;
char* tab_final;
int* qtd_palavras;
char*** palavras;




while(condicao==0){
	
	scanf("%d %d/n",&m,&n);
	
	
	if(m==0 && n==0){
		condicao=1;
	}
	
	else{
		existe=1;
		
		
		
		tab_inicial = malloc(sizeof(int)*m*n);
		tab_final=malloc(sizeof(char)*m*n);
		leitura_tab(m,n,tab_inicial,tab_final);
		
		
		
		if (m>=n){
			tamanho_max=m;
		}
		else{
			tamanho_max=n;
		}
		
		palavras = malloc(sizeof(char**)*(tamanho_max+1));
		qtd_palavras=malloc(sizeof(int)*(tamanho_max+1));
		
		
		
		scanf(" %d",&num_palavras);
		
		if(num_palavras ==0){
			for(i=0;i<m*n;i++){
				if(tab_inicial[i]==0){
					existe=0;
				}
			}
		
		}
		else{
			for (i=2;i<tamanho_max+1;i++){
				palavras[i] = malloc(sizeof(char*)*num_palavras);
			}
			
			for (i=2;i<tamanho_max+1;i++){
				qtd_palavras[i]=0;
			}
			
			leitura_palavras(num_palavras,palavras,qtd_palavras,tamanho_max);
			existe=p_cruzadas(num_palavras, palavras, qtd_palavras, tamanho_max, tab_inicial, tab_final, m, n);
		
		}
		
		
		
		printf("\n Instancia %d \n",instancias);
		/*for(i=0;i<m*n;i++){
			printf("%d ",tab_inicial[i]);
		}*/
		
		
		if (existe==0){
			printf("\n Não haverá solucão para esse caso\n");
		}
		else {
				
			for(i=0;i<m;i++){
				for(j=0;j<n;j++){
					printf("%c ",tab_final[i*n+j]);
				}
				printf("\n");
			}
		
			for(i=2;i<=tamanho_max;i++){
				for(j=0;j<qtd_palavras[i];j++){
					printf("%s \n",palavras[i][j]);
				}
			}
		}
		
		printf("\n\n\n Nova instancia \n\n\n");
		instancias++;
		
		free(tab_inicial);
		free(tab_final);
		free(qtd_palavras);
		free(palavras);
	}

}


return 0;
}
