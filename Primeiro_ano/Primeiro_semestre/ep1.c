#include <stdio.h>
#include <stdlib.h>

int main() {

int controle,depuracao,resposta,jogjogados,jogvencidos,cont;
int num, A, B,C,tentativa,adivinha,A_certo,B_certo,C_certo;

tentativa = 6;
controle = 0;
jogjogados = 0;
jogvencidos = 0;
depuracao = 0;
A=0;
B=0;
C=0;
cont=0;

printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
printf("\n Bem-vindo ao JOGO da FORCA !! \n");
printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");


/*solicitar a escolha do usuário*/
while (controle == 0){
   if (depuracao == 0){
      printf ("\n [ depuracão = DESLIGADO / jogos jogados = %d ]\n", jogjogados);
   }
   else {
      printf ("\n [ depuracão = LIGADO / jogos jogados = %d ]\n", jogjogados);
   }
   printf ("\n 1. Continuar a jogar");
   printf ("\n 2. Alternar modo depuração e continuar a jogar");
   printf ("\n 3. Parar\n");
   printf ("\n Escolha uma opção: ");
   scanf ("%d", &resposta);

   if (resposta == 1){
      jogjogados = jogjogados + 1;
   }

   if (resposta == 2){
      if (depuracao == 0){
         depuracao = 1;
      }
      else{
         depuracao = 0;
      }
      jogjogados = jogjogados + 1;
   }     

   if (resposta == 3){
      printf("\nVocê jogou %d jogos e venceu %d vezes!\n\n", jogjogados,jogvencidos);
      controle = 1;
   }

   /*sortear e separar o número aleatório*/      
   num = rand() % 1000;
   C = num % 10;
   B = ((num - C) %100)/10;
   A = ((num - C - B*10) %1000)/100;
   
   if (depuracao == 1 && controle == 0){
       printf("\nO numero a ser adivinhado é %d\n",num);
   }
   
   /*Impressoes baseadas na resposta do usuário*/
   while (controle == 0 && cont == 0 && tentativa > 0){
      printf ("\nRestam %d tentativas",tentativa);
      printf ("\nSua próxima tentativa (dígito de 0 a 9):");
      scanf ("%d", &adivinha);
      if (adivinha == A){
         A_certo=1;
      }
      if (adivinha == B){
         B_certo=1;
      }
      if (adivinha == C){
         C_certo=1;
      }

      if (A_certo == 1){
         if (B_certo == 1){
            if (C_certo ==1){
               jogvencidos= jogvencidos + 1;
               printf("\nVocê ja acertou os seguintes digitos: %d %d %d ", A,B,C);
            }
            else{
               printf("\nVocê ja acertou os seguintes digitos: %d %d _ ", A,B);
            }
         }
         else{
            if (C_certo ==1){
               printf("\nVocê ja acertou os seguintes digitos: %d _ %d ", A,C);
            }
            else{
               printf("\nVocê ja acertou os seguintes digitos: %d _ _", A);
            }
         }
      }
      else{
         if (B_certo == 1){ 
            if (C_certo ==1){
               printf("\nVocê ja acertou os seguintes digitos: _ %d %d ", B,C);
            }
            else{
               printf("\nVocê ja acertou os seguintes digitos: _ %d _ ", B);  
            } 
         }
         else{
            if (C_certo ==1){
               printf("\nVocê ja acertou os seguintes digitos: _ _ %d ",C);
            }
            else{
               printf("\nVocê ja acertou os seguintes digitos: _ _ _ ");
            }
         }
      }
      if (A_certo == 1 && B_certo ==1 && C_certo==1){
         printf("\n\nVocê VENCEU!\n\n\n");
         cont = 1;
      }
      tentativa = tentativa - 1;
      if (tentativa==0 && (A_certo != 1 || B_certo !=1 || C_certo!=1)){
         printf("\n\nVocê PERDEU!\n\n\n");
      }

   }
   tentativa = 6;
   cont =0;
   A_certo= 0;
   B_certo= 0;
   C_certo=0;
}

return 0;
}