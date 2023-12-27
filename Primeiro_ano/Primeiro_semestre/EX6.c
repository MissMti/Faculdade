#include <stdio.h>

int n,d,num,contador,digito,a,b,quantosa,quantosb;

int erro = 0;
int i =1;

int contadigitos(int n, int d) {
 contador = 0;
 num = n ;
 while (num != 0){
  digito = num % 10;

  if (digito == d){
   contador = contador + 1;
  }
  num = num / 10;

 }

 return contador;
}


int main (){
 printf("\n Insira o numero a:");
 scanf("%d",&a);

 printf("\n Insira o numero b:");
 scanf("%d",&b);

 for ( i=1 ; i<=9 ; i++ ){
  quantosa = contadigitos (a,i);
  quantosb = contadigitos (b,i);

  if (quantosa != quantosb ){
   erro = erro + 1;
  }
  
 }
 
 if (erro == 0){
  printf("\n\n Os numeros formam uma permutacao !!! \n\n");
 }
 else {
  printf("\n\n Os numeros nao formam uma permutacao :/ \n\n");
 }
 return 0;
} 

