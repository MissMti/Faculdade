# include <stdio.h>

#define MAX 100

int m,n,i,j,a,b;
int M[MAX];
int N[MAX];

int main(){
 printf ("\n Insira o valor de m: ");
 scanf("%d",&m);
 printf ("\n Insira o valor de n: ");
 scanf("%d",&n);

 printf("\n Insira a primeira sequencia: ");
 for (i=0;i<m;i++){
   scanf("%d",&M[i]);
 }

 printf("\n Insira a segunda sequencia: ");
 for (i=0;i<n;i++){
   scanf("%d",&N[i]);
 }
 
 printf("\n Sequencia ordenada com número de ambas, sem repetição: ");
 a=0;
 b=0;
 for (i=0;i<(m+n) && a<m && b<n;i++){
   
   if (M[a] < N[b]){
    printf("%d ",M[a]);
    a = a+1;
   }
   if (M[a] > N[b]){
    printf("%d ",N[b]);
    b = b+1;
   }
   if (M[a] == N[b]){
    printf("%d ",M[a]);
    a = a+1;
    b = b+1;
    i = i+1;
   }
 }
 
 for (i=a;i<m;i++){
   printf("%d ",M[i]);
 }
 for (i=b;i<n;i++){
   printf("%d ",N[i]);
 }
 
 printf("\n\n");
 
 return 0; 
}
 
