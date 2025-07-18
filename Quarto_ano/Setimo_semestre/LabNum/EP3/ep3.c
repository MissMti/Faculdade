#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


// Calculando Trabalho_______________________________________

double x[10];
double F[10];
double theta[10];
double Fcos[10];

void coloca_valores_do_enunciado(){
    x[0]=0, F[0]=0.0, theta[0]=0.50;
    x[1]=5, F[1]=9.0, theta[1]=1.40;
    x[2]=10,F[2]=13.0,theta[2]=0.75;
    x[3]=15,F[3]=14.0,theta[3]=0.90;
    x[4]=20,F[4]=10.5,theta[4]=1.30;
    x[5]=25,F[5]=12.0,theta[5]=1.48;
    x[6]=30,F[6]=5.0, theta[6]=1.50;

    Fcos[0]=0.0000;
    Fcos[1]=1.5297;
    Fcos[2]=9.5120;
    Fcos[3]=8.7025;
    Fcos[4]=2.8087;
    Fcos[5]=1.0881;
    Fcos[6]=0.3537;
    return;
}

//  (i) Interpolar a função F(x)cos(θ(x)) usando Interpolação de Lagrange

double Lagrange(double tmp_x){
    double rsp=0;
    for(int i=0;i<7;i++){
        double t_atual=Fcos[i];
        for(int j=0;j<7;j++){
            if(i!=j){
                t_atual*=(tmp_x-x[j])/(x[i]-x[j]);
            }
        }
        rsp+=t_atual;
    }

    return rsp;
}


//  (iia.) Regra do Trapezio Composto

double Trapezio() {

    double rsp=Fcos[0];

    double tmp=0;
    for(int i=1;i<6;i++){
        tmp+=Fcos[i];
    }
    rsp+=2*tmp;

    rsp+=Fcos[6];

    rsp*=((x[1]-x[0])/2);
    
    return rsp;
}

//  (iib.) Regra de Simpson Composto

double Simpson() {

    double rsp=Fcos[0];

    //impares
    double tmp=0;
    for(int i=1;i<6;i=i+2){
        tmp+=Fcos[i];
    }
    rsp+=4*tmp;

    //pares
    tmp=0;
    for(int i=2;i<6;i=i+2){
        tmp+=Fcos[i];
    }
    rsp+=2*tmp;

    rsp+=Fcos[6];

    rsp*=((x[1]-x[0])/3);


    return rsp;
}



// Monte Carlo________________________________________________________

long long int n;

// Ponto aleatorio num intervalo [a,b] 

double sorteia(double a,double b){
    double valor=a;
    valor+=(b-a)*(double)rand()/(double)RAND_MAX;
    return valor;
}

// Funcoes

double f_sen(double x){
    return sin(x);
}

double f_cubico(double x){
    return x*x*x;
}

double f_euler(double x){
    return exp(-x);
}

double f_pi(double x, double y){
    if(x*x+y*y<=1.0){
        return 1.0;
    }
    return 0.0;
}


// Unidimensional

double unidimensional(double a, double b, double opcao) {

    double rsp=0.0;

    for(int i=0;i<n;i++){
        if(opcao==0){
            rsp+=f_sen(sorteia(a,b));
        }
        else{
            if(opcao==1){
                rsp+=f_cubico(sorteia(a,b));
            }
            else{
                rsp+=f_euler(sorteia(a,b));
                
            }
        }
    }
    rsp=rsp/n;

    rsp=rsp*(b-a);//multiplicamos pelo tamanho do dominio

    return rsp;
}

// Multidimensional

double multidimensional(double a1, double b1, double a2, double b2) {
// Como só tem um exercicio que usa essa funcao nao precisa de opcao e
//já sabemos que ela é bidimensional entao podemos só trabalhar com a1,b1,a2,b2 direto

    double rsp=0.0;

    for(int i=0;i<n;i++){
        rsp+=f_pi(sorteia(a1,b1),sorteia(a2,b2));
    }
    rsp=rsp/n;

    rsp=rsp*(b2-a2)*(b1-a1);//multiplicamos pelo tamanho do dominio

    return rsp;
}


int main() {

    printf("\n__________ Parte 1: Computando Trabalho __________\n\n");

    coloca_valores_do_enunciado();

    printf("\nResultados da interpolação: \n");
    printf("p(0)= %.4f \n",Lagrange(0));
    printf("p(5)= %.4f \n",Lagrange(5));
    printf("p(10)= %.4f \n",Lagrange(10));
    printf("p(15)= %.4f \n",Lagrange(15));
    printf("p(20)= %.4f \n",Lagrange(20));
    printf("p(25)= %.4f \n",Lagrange(25));
    printf("p(30)= %.4f \n\n",Lagrange(30));

    printf("Pela Regra do Trapezio Composta: %.6f J\n",Trapezio());
    printf("Pela Regra de Simpson Composta: %.6f J\n",Simpson());

    printf("\n\n__________ Parte 2: Integração por Monte Carlo __________\n\n");

    
    srand(time(NULL));
    //srand(12542842);

    printf("Insira o valor de n: ");
    scanf("%lld",&n);

    printf("\n1. sen(x): ");
    printf("%.6f \n\n", unidimensional(0, 1, 0));

    printf("2. x^3: ");
    printf("%.6f \n\n", unidimensional(3, 7, 1));

    printf("3. e^(-1): ");
    printf("%.6f \n\n", unidimensional(0, 10000000, 2));

    printf("4. pi: ");
    printf("%.6f \n", 4.0 *multidimensional(0, 1, 0, 1));


    printf("\n___________________________________________________________\n");

    return 0;
}
