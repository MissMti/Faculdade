# include <stdio.h>
#define MAX 256

int n,soma,anterior,novo,erro,d_principal,d_secundaria;
int i,j;
int A[MAX][MAX];


int main(){
 printf ("\n Insira o valor de n: ");
 scanf("%d",&n);

 for (i=0;i<n;i++){
  for (j=0;j<n;j++){
   printf("\n Insira o valor do elemento [%d]x[%d] da matriz A : ",i+1,j+1);
   scanf("%d",&A[i][j]);
  }

 }
 
 erro =0;
 soma = 0;
 
 for (i=0;i<n;i++){
  for (j=0;j<n;j++){
   soma = soma + A[i][j];
  }
  novo = soma;
  if ((i>0) && (novo != anterior)){
   erro = 1;

  }
  soma=0;
  anterior = novo;
 }
 
 for (i=0;i<n;i++){
  for (j=0;j<n;j++){
   soma = soma + A[j][i];
  }
  novo = soma;
  
  if (novo != anterior){
   erro = 1;
  }
  soma=0;
  anterior = novo;
 }
 
 d_principal = 0;
 d_secundaria = 0;
 
 for (i=0;i<n;i++){
  d_principal = d_principal + A[i][i];
  j=n-1-i;
  d_secundaria = d_secundaria + A[i][j];
 }
 
 if ((anterior != d_principal) || (anterior != d_secundaria))  {
  erro = 1;
 }
 
 if (erro==0){
  printf("\n Esse é um quadrado mágico!!! \n\n");
 }
 else{
  printf("\n Esse não é um quadrado mágico. \n\n");
 }


 return 0;
}


