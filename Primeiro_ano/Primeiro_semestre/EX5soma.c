#include <stdio.h>

int main () {

double x,eps,fatorial,num,soma,elevado,termo;
int controle;
controle=0;
elevado=1.0;
termo = 0.0;
fatorial=1.0;
soma=0.0;

printf("\n Entre com um valor x:  ");
scanf("%lf", &x);
printf("\n Entre com um valor para eps:  ");
scanf("%lf", &eps);
if (eps>1){
 controle=1;
 }

while (controle==0){
      num = elevado/fatorial;
      soma= soma+num;
      if (num<0){
          num=num*(-1);
      }
      if (num<eps){
          
          controle=1;
      }
      
      termo = termo+1;
      elevado = elevado * x;
      fatorial = fatorial * termo;
}
printf("\n A aproximação é de: %f \n\n",soma );

return 0;
}
