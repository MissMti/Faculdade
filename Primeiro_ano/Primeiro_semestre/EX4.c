#include <stdio.h>

int main () {

/* Criar e definir as variaveis*/
int n;
int digito,elevado,tamanho,acabar,erro,Dig_A,Dig_B,A,B,elv_maior,i;
elevado=10;
tamanho=1;
elv_maior=1;
i=1;

/* Solicitar ao usuario o numero */
printf("Entre com um número n:");
scanf("%d", &n);

A=n;
B=n;

/* Descobrir quantos algarismos tem o numero*/
while (acabar != 1) {
    digito = n / elevado;
    if (digito == 0){
      acabar = 1;     
    }
    else{
       tamanho = tamanho + 1;
       elevado = elevado*10;
    }
      
}



/* Encontrar a variável que será utilizada como referencia para verificar os digitos do numero*/
for (i==1; i<tamanho; i++){
  elv_maior = elv_maior * 10;
}

/* Reiniciar contador*/
i=0;


/* Comparar os números de forma a verificar se o número é, ou não, um palindromo*/
while (i<=tamanho) {
    Dig_A = A%10;
    Dig_B = (B - (B%elv_maior))/elv_maior;

    if (Dig_A == Dig_B) {
        A = (A - Dig_A)/10;
        B = B - Dig_B*elv_maior; 

        
 
        elv_maior=elv_maior/10;
        
    }    
    else{
        erro = 1;
        i = tamanho +1;
        
    }

    i=i+2;
    
}  


/* Imprimir se o número é, ou não, palindromo */
if (erro == 1){
    printf ("\n Esse número não é palíndromo! \n\n");
}
else{
    printf ("\n Esse número é palíndromo! \n\n");
}

}