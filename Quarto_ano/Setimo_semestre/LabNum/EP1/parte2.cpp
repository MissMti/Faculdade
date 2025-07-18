#include <bits/stdc++.h>

using namespace std;

#define int long long

FILE *arq;
double l_,u_;
int p_;
double tolerancia=1e-6;
vector<complex<double>> cores;//guardas as raizes, a cor é o indice...
int cnt=0;
double max_it=1000;




/*
Entrada: x
Saída: f(x)

Função: Calcula o valor da função f aplicada em x.
*/
complex<double> evalf(complex<double> x){
    complex<double> tmp;
    tmp= 26.0*x*x*x*x*x*x*x-10.0*x*x*x*x*x*x+1965.0;
    return tmp;
}



/*
Entrada: x
Saída: f'(x)

Função: Calcula o valor da derivada da função f aplicada em x.
*/
complex<double> evalDf(complex<double> x){
    complex<double> tmp;
    tmp= 182.0*x*x*x*x*x*x-60.0*x*x*x*x*x;
    return tmp;
}



/*
Entrada: x_0
Saída: valor do resultado da aplicação do método de Newton partindo de x_0

Dado x_n e f, encontramos x_(n+1) por x_n-f(x_n)/f'(x_n), fazemos iterações suficientes
até encontrar um erro abs(x_n-x_(n+1))<=tolerancia (para ambas partes reais e imaginarias)

Função: Aplica o método de Newton para achar uma raiz da função f
(com primeira derivada f'), partindo do ponto x_0.
*/
complex<double> newton(complex<double> x0){
    complex<double> xn,xprox;

    xprox=x0;

    for(int i=0;i<max_it;i++){
        xn=xprox;
        xprox=xn-evalf(xn)/evalDf(xn);
        if(abs(xn.real()-xprox.real())<=tolerancia && abs(xn.imag()-xprox.imag())<=tolerancia){
            break;
        }
    }
    return xprox;
}



/*
Entrada: valor complexo x que é resultado da aplicação do método de Newton
Saída: indice da cor da raiz (caso não tenha encontrado, indique uma cor para ela também)

Aplicar um indice("cor") para cada uma das raízes, a função então confere se essa raiz encontrada
já foi "encontrada antes", caso sim, concede esse mesmo valor, caso não, gera um novo valor.
Considero também caso dê um valor que não converteu para nenhuma raiz..
*/
int qual_raiz(complex<double> x){
    //verifica se não converge
    if(abs(evalf(x).real())>tolerancia || abs(evalf(x).imag())>tolerancia){//f(x)!=0 (considerando a tolerancia)
        return 0;
    }

    //procura se essa raiz já foi encontrada
    for(int i=0;i<cores.size();i++){
        if(abs(x.real()-cores[i].real())<=tolerancia && abs(x.imag()-cores[i].imag())<=tolerancia){
            return i+1;
        }
    }

    //caso não, adiciona nova raiz
    cores.push_back(x);
    return cores.size();
}



/*
Entrada: l,u,p
Saída: 

Função: Acha as bacias de convergência da função f no domínio [l_1, u_1]×[l_2, u_2]
e gera um arquivo output.txt que contém os dados para a geração da imagem das 
bacias (pode usar gnuplot para gerar as imagens).
Os dados gerados preenchem uma imagem com p1×p2 pixels.
*/
void newton_basins(double l, double u, int p){
    complex<double> pixel,x;
    complex<double> I=1i;
    int cor;

    for(int i=1;i<=p;i++){
        for(int j=1;j<=p;j++){
            pixel=(l+(2*j-1)*(u-l)/(2*p))+(u-(2*i-1)*(u-l)/(2*p))*I;//esquerda para a direita de cima para baixo
            x=newton(pixel);
            cor=qual_raiz(x);
            
            fprintf(arq,"%lf %lf %lld \n",(l+(2*j-1)*(u-l)/(2*p)),(u-(2*i-1)*(u-l)/(2*p)),cor);
        }
    }
}



/*
Função: Realizar as leituras das variávies
*/
void leitura(){
    
    cout<<"\nOlá! Você está na parte 2 do EP1 de MAC0210!\n\n";

    cout<<"Sobre o intervalo [l,u]: \n";
    cout<<"Por favor insira o valor de l: ";
    cin>>l_;
    cout<<"\n";
    cout<<"Por favor insira o valor de u: ";
    cin>>u_;
    cout<<"\n";

    cout<<"Agora, insira o valor de p: ";
    cin>>p_;
    cout<<"\n";

    char perg;
    cout<<"Você deseja escolher a tolerancia do diferanca? (por padrão esse valor é 10⁻6) (s/n)";
    cin>>perg;
    cout<<"\n";
    if(perg=='s'){
        cout<<"Por favor insira o valor da tolerancia do diferanca: ";
        cin>>tolerancia;
        cout<<"\n";
    }

    cout<<"Você deseja escolher o maximo de iteraçoes? (por padrão esse valor é 1000) (s/n)";
    cin>>perg;
    cout<<"\n";
    if(perg=='s'){
        cout<<"Por favor insira o valor d o maximo de iteraçoes: ";
        cin>>max_it;
        cout<<"\n";
    }


}



signed main() {

    leitura();

    arq = fopen("output.txt", "w");

    newton_basins(l_,u_,p_);

    fclose(arq);

    return 0;
}
