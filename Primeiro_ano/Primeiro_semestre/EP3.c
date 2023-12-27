# include <stdio.h>
# include <stdlib.h>


/*Lê imagem dentro de um arquivo e coloca em I*/
int *load_image_from_file(char *filename, int *w, int *h){
	FILE *arq;
	int i,tamanho;
	int *imagem_ent;
	char linha[100];
	char numero[100];
	
	arq = fopen(filename, "r");
	fscanf(arq,"%100[^\n]", linha);
	fscanf(arq,"%d %d \n", w, h);
	fscanf(arq,"%100[^\n]", numero);
	
	tamanho = (*w)*(*h);
	imagem_ent = malloc(sizeof(int)*(*w)*(*h));
	for(i=0;i<tamanho;i++){
		fscanf(arq,"%d ",&imagem_ent[i]);
	}
	return imagem_ent;

}




/*Usa o nome do arquivo dado pelo usuario e cria um arquivo do formato .pgm do tipo P2 com o nome e a imagem*/
void save_image_to_file(char *filename, int *I, int w, int h){
	FILE *arq;
	int i,j;
	arq = fopen(filename, "w");
	fprintf(arq,"P2 \n");
	fprintf(arq,"%d %d \n",w,h);
	fprintf(arq,"255\n");
	for(i=0;i<h;i++){
  		for(j=0;j<w;j++){
  		  fprintf(arq,"%d ",I[i*w + j]);
 		}
  		fprintf(arq,"\n");		
	}
	
	fclose(arq);
}


/*Apartir do t, verificar se cada valor é maior/igual ou menor que esse t */
int *binarize (int *I, int w, int h, int t){
	int *im_binarizada;
	int i,j;
	im_binarizada=malloc(sizeof(int)*w*h);
	
	for(i=0;i<h;i++){
  	  for(j=0;j<w;j++){
  		  if (I[i*w+j]>=t){
  		  	im_binarizada[i*w+j]=255;
  		  }
  		  else{
  		  	im_binarizada[i*w+j]=0;
  		  }
 	  }		
	}
	
	return im_binarizada;
}


/*Para facilitar o filtro, esse programa produz uma borda que reproduz os elementos internos*/
int *borda(int *I,int w, int h){
	int *ComBorda;
	int i,j,l,a;
	
	l=w+2;
	a=h+2;
	ComBorda = malloc(sizeof(int)*(l)*(a));
	
	/*Cantos*/
	ComBorda[0]= I[0];
	ComBorda[l-1]= I[w-1];
	ComBorda[(a-1)*(l)]= I[(h-1)*w];
	ComBorda[(a-1)*(l)+(l-1)]= I[(h-1)*w+(w-1)];
	
	/*Laterais cima e baixo*/
	for (i=1;i<(l-1);i++){
		ComBorda[i]= I[i-1];
	}
	for (i=1;i<(l-1);i++){
		ComBorda[(a-1)*(l)+i]=I[(h-1)*w+i-1];
	}
	
	/*Laterais direita e esquerda*/
	for (i=1;i<(l-1);i++){
		ComBorda[i*(l)]= I[(i-1)*w];
	}
	for (i=1;i<(l-1);i++){
		ComBorda[i*(l)+(l-1)] = I[(i-1)*w+(w-1)];
	}
	
	/*Interior*/
	for(i=0;i<h;i++){
 	 for(j=0;j<w;j++){
 	 	ComBorda[(i+1)*(l)+(j+1)]=I[i*w+j];
 	 }
 	}
	
	
	return ComBorda;
}

/*Ordena numa fila, para facilitar na formaçao da imagem filtrada independente da escolha */
int *ordena(int*B,int d,int k,int m,int *ordem,int nl){
	int num;
	int i,j,l;
	num =0;
	
	
	for(i=k;i<d+k;i++){
	 for(j=m;j<d+m;j++){
	 	ordem[num] =  B[i*nl+j];
	 	num = num + 1;
	 }
	}

	for(i=0;i<d*d;i++){
	 for(j=i+1;j<d*d;j++){
	 	if(ordem[i]>ordem[j]){
	 		l=ordem[i];
	 		ordem[i]=ordem[j];
	 		ordem[j]=l;
	 	}
	 }
	}
	

	return ordem;
}


int *filter_image (int *I,int w, int h, int d, int tipo){
	int nl,na;
	int *B;
	int *filtrada;
	int i,j;
	int hh,ww;
	int *Ordem;
	hh=h;
	ww=w;
	nl=w+(d-1);
	na=h+(d-1);
	B=malloc(sizeof(int)*nl*na);
	filtrada=malloc(sizeof(int)*w*h);
	Ordem = malloc(sizeof(int)*d*d);
	B=I;
	
	
	
	
	for (i=0;i<((d-1)/2);i++){
		B = borda(B,ww,hh);
		hh=hh+2;
		ww=ww+2;
	}
	
	
	for(i=0;i<h;i++){
  	   for(j=0;j<w;j++){
  		
  		Ordem = ordena(B,d,i,j,Ordem,nl);
  		
  		
  		if (tipo==1){
			filtrada[i*w+j]=Ordem[0];
		}
		else{
			if(tipo==2){
				filtrada[i*w+j]=Ordem[(d*d)/2];
			}
			else{
				filtrada[i*w+j]=Ordem[(d*d)-1];
			}
		}
  			
 	   }		
 	}

	free(Ordem);
	Ordem=NULL;
	free(B);
	B=NULL;
	
	return filtrada;
}


int *contorno (int *I, int w, int h){
	int *im_contornada;
	int *base_filtrada;
	int i,j;
	
	im_contornada=malloc(sizeof(int)*w*h);
	base_filtrada=malloc(sizeof(int)*w*h);
	
	base_filtrada=filter_image(I,w,h,3,1);
	for(i=0;i<h;i++){
  	  for(j=0;j<w;j++){
  		  im_contornada[i*w+j] = I[i*w+j] - base_filtrada[i*w+j];
 	  }		
	}
	
	return im_contornada;
}

/*Inverte imagem*/
int *invert_image(int *I, int w, int h){
	int *im_invertida;
	int i,j;
	
	im_invertida=malloc(sizeof(int)*w*h);
	
	for(i=0;i<h;i++){
  	  for(j=0;j<w;j++){
  		  im_invertida[i*w+j] = 255 - I[i*w+j];
 	  }		
	}
	
	return im_invertida;
}


int *label_components(int *I, int w, int h, int *ncc){
  int* ja_passou;
  int* im_borda_zero;
  int* imI_borda_zero;
  int* im_rotulada;
  int i,j,m,n,a,z;
    
  im_borda_zero = malloc(sizeof(int)*(w+2)*(h+2));
  imI_borda_zero = malloc(sizeof(int)*(w+2)*(h+2));
  ja_passou = malloc(sizeof(int)*w*h);
  im_rotulada = malloc(sizeof(int)*w*h);

  *ncc = 0;
  
  /*zera todos os valores do arquivo*/
  for (i = 0; i < h+2; i++){
    for (j = 0; j < w+2; j++){
      im_borda_zero[i*(w+2)+j] = 0; 
    }
  }
  
  /*faz uma imagem I com borda, para facilitar na hora de verificar os 8 vizinhos*/
  for (i = 0; i < h+2; i++){
      imI_borda_zero[i*(w+2)] = 0; 
      imI_borda_zero[i*(w+2)+(w+1)] = 0; 
      imI_borda_zero[i] = 0; 
      imI_borda_zero[(h+1)*(w+2)+i] = 0; 
  }
  
  /*reproduz I dentro*/
  for (i = 1; i < h+1; i++){
    for (j = 1; j < w+1; j++){
      imI_borda_zero[i*(w+2)+j] = I[(i-1)*w+j-1]; 
    }
  }
  
  /*Nessa parte criei um loop que verifica cada elemento interno da imagem com borda e verifico se ele é diferente de zero e se seu representante na imagem zerada tbm vale zero, o que demonstra ser um novo componente conexo*/
  for (i = 1; i < h+1; i++){
    for (j = 1; j < w+1; j++){
	
      a = 0;
      z = 0;

      if (imI_borda_zero[i*(w+2) + j] != 0 && im_borda_zero[i*(w+2) + j] == 0){
        *ncc = *ncc + 1; 
        im_borda_zero[i*(w+2) + j] = *ncc;
        ja_passou[a] = i*(w+2) + j;

	/*verifco se os vizinhos sao elementos novos, diferentes de zero e ainda n registrados, para entao adicionar a uma fila e posteriormente anasalisar seus vizinhos tbm, até acabar com todos os elementos da fila*/
        while (a <= z ){          

          for (m = -1 ; m <= 1; m++){
             
            for (n = -1; n <= 1; n++){
                  
                if ((imI_borda_zero[ja_passou[a] + m*(w+2) + n] != 0) && (im_borda_zero[ja_passou[a]+ m*(w+2) + n] == 0)){

                  z++;
                  ja_passou[z]= ja_passou[a] + m*(w+2) + n;   
                  im_borda_zero[ja_passou[z]] = *ncc;                 

                }       
  
              }
              
            }
                    
          a++;
    
        }
      }

    }
  }
  
  
  
  
  for (i = 0; i < h; i++){
    for (j = 0; j < w; j++){
      im_rotulada[i*w+j] = im_borda_zero[(i+1)*(w+2)+(j+1)]; 
    }
  }
  
  
  
  if((*ncc<=127)&&(*ncc>0)){
	for(i=0;i<h;i++){
  	  for(j=0;j<w;j++){
  		  im_rotulada[i*w+j] = im_rotulada[i*w+j]*(255/(*ncc));
 	  }		
	}	
  }
  
  free(im_borda_zero);
  im_borda_zero=NULL;
  free(imI_borda_zero);
  imI_borda_zero=NULL;
  free(ja_passou);
  ja_passou=NULL;
  
  return im_rotulada;

}





int main () {

char escolha_inicial,escolha;
int Quit;
int *I;
int *R;
char filename[256] ;
char nome_p_salvar[256];
int largura,altura;
int tipo, d;
int valor_t;
int ncc;
R=NULL;
Quit=1;

printf("\n	Seja Bem-Vindo ao Programa de Processamento de Imagens !\n\n\n");


/*Primeira Escolha, quando não há nenhuma imagem*/
printf("\n	Por favor, escolha uma das seguintes opções:\n");
printf("\n	Q -> Termina o programa\n");
printf("\n	L -> Lê um arquivo de imagens\n");
printf("\n	Sua escolha: ");
scanf(" %c",&escolha_inicial);

if ((escolha_inicial == 'Q')||(escolha_inicial =='q')){
	printf("\n	Você encerrou o programa! TCHAU\n\n");
	return 0;
}
else{
	printf("\n	Você escolheu ler uma imagem!\n\n");
	printf("	Por favor, insira o nome do arquivo a ser aberto: ");
	scanf("%s", filename);
	I=load_image_from_file(filename,&largura,&altura);
	Quit=0;
	
}

while (Quit == 0){
	printf("\n\n\n	*************************************************\n\n");
	printf("\n	Por favor, escolha uma das seguintes opções: \n\n");
	printf("\n	 Q -> Termina o programa\n");
	printf("	 L -> Lê um arquivo de imagens\n");
	printf("	 S -> Salva a imagem em arquivo\n");
	printf("	 M -> Mantem a imagem-entrada anterior\n");
	printf("	 B -> Binariza a imagem-entrada\n");
	printf("	 C -> Calcula Contorno da imagem-entrada\n");
	printf("	 F -> Filtra a imagem-entrada\n");
	printf("	 I -> Inverte a imagem-entrada\n");
	printf(" 	 R -> Rotula a imagem-entrada\n");
	printf("\n	Sua escolha: ");
	scanf(" %c",&escolha);
	
	/*Por um questao de organizacao do codigo, optei por usar varios if inves de uma sequencia com if e else */
	
	if ((escolha == 'Q')||(escolha=='q')){
		printf("\n	Você Encerrou o Programa! TCHAU!!!\n\n");
		Quit=1;	
	}
	
	if ((escolha == 'L')||(escolha=='l')){
		free(R);
		R=NULL;
		free(I);
		I=NULL;
		printf("\n	Você escolheu Ler uma imagem\n");
		printf("	Por favor, insira o nome do arquivo a ser aberto: ");
		scanf("%s", filename);
		I=load_image_from_file(filename,&largura,&altura);
		printf("\n	A imagem foi lida!!\n\n");
	}
	
	if ((escolha == 'S')||(escolha=='s')){
		if(R==NULL){
			printf("\n	Você escolheu Salvar a imagem-entrada atual em um arquivo\n");
 			printf("\n	Por favor insira o nome desse novo arquivo: ");
 			scanf("%s", nome_p_salvar);
 			save_image_to_file(nome_p_salvar,I,largura,altura);
			printf("\n	Sua imagem foi salva!!\n\n");
		}
		else{
			printf("\n	Você escolheu Salvar a imagem-transformada em um arquivo\n");
 			printf("\n	Por favor insira o nome desse novo arquivo: ");
 			scanf("%s", nome_p_salvar);
 			save_image_to_file(nome_p_salvar,R,largura,altura);
			printf("\n	Sua imagem foi salva!!\n\n");
		}
	}
	
	if ((escolha == 'M')||(escolha=='m')){
		printf("\n	Você escolheu Manter a imagem atual\n");
		free(R);
		R=NULL;
	}
	
	if ((escolha == 'B')||(escolha=='b')){
		if (R !=NULL){
			I=R;
		}
		printf("\n	Você escolheu Binarizar a imagem atual\n");
		printf("\n	Agora para isso, por favor escolha um t, que será usado como referência para a lamiarização: ");
		scanf(" %d",&valor_t);
		R=binarize(I,largura,altura,valor_t);
		printf("\n	Pronto, agora a sua imagem já esta binarizada!\n\n");		
		
	}
	if ((escolha == 'C')||(escolha=='c')){
		if (R !=NULL){
			I=R;
		}
		printf("\n	Você escolheu Calcular o contorno a imagem atual\n");
		R=contorno(I,largura,altura);
		printf("\n	A imagem teve seu contorno determinado!!\n\n");
		
	}
	if ((escolha == 'F')||(escolha=='f')){
		if (R !=NULL){
			I=R;
		}
		printf("\n	Você escolheu Filtrar a imagem atual\n");
		printf("\n	Por Favor defina o tamanho dxd que a filtragem irá analisar: ");
		scanf(" %d",&d);
	
		printf("\n	Entendido, agora escolha qual será o tipo de filtragem realizada: \n");
		printf("\n	 1 - filtro de minimo\n");
		printf("	 2 - filtro de mediana\n");
		printf("	 3 - filtro de maxima\n");
		printf("\n	A sua escolha será: ");
		scanf(" %d",&tipo);
		R = filter_image(I,largura,altura,d,tipo);
		printf("\n	A imagem foi filtrada!!\n\n");
		
		
	}
	if ((escolha == 'I')||(escolha=='i')){
		if (R !=NULL){
			I=R;
		}
		printf("\n	Você escolheu Inverter a imagem atual\n");
		R=invert_image(I,largura,altura);
		printf("\n	A imagem teve seu inverso determinado!!\n\n");
		
	}
	if ((escolha == 'R')||(escolha=='r')){
		if (R !=NULL){
			I=R;
		}
		printf("\n	Você escolheu Rotular a imagem atual \n");
		R=label_components(I,largura,altura,&ncc);
		printf("\n	A imagem foi rotulada!! \n\n");
		printf("\n	Na imagem há %d componentes conexos!",ncc);
		
		
	}
}

free(R);
R=NULL;
free(I);
I=NULL;

return 0;
}
