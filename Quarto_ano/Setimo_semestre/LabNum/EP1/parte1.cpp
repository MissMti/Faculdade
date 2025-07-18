#include <bits/stdc++.h>

using namespace std;

#define int long long

double tolerancia=1e-6;
double max_it=1000;



double g1(double x){
    return -sqrt(exp(x)/2.0);
}

double g2(double x){
    return sqrt(exp(x)/2.0);
}

double g3(double x){
    return log(2.0*x*x);
}



/*
Realiza as iterações e verifica se a diferença entre xk e xk+1 é menor ou igual a tolerancia
para cada uma das funções
*/
void metodo_do_ponto_fixo(double x0){
    cout<<"\nEncontrando as raizes: \n";
    double x1,diferanca;
    int cnt=0;

    cout << fixed << setprecision(10);

    cout<<"\nPara g1(x)=-sqrt(exp(x)/2.0), temos que: ";
    double tmp;
    x1=x0;
    for(int i=0;i<max_it;i++){
        cnt++;
        tmp=x1;
        x1=g1(tmp);
        diferanca=abs(tmp-x1);
        if(diferanca<=tolerancia){
            break;
        }
    }
    cout<<"\nPrimeira raiz: "<<x1;
    cout<<"\nTotal de iterações: "<<cnt;
    cout<<"\n";

    cout<<"\nPara g2(x)=sqrt(exp(x)/2.0), temos que: ";
    cnt=0;
    x1=x0;
    for(int i=0;i<max_it;i++){
        cnt++;
        tmp=x1;
        x1=g2(tmp);
        diferanca=abs(tmp-x1);
        if(diferanca<=tolerancia){
            break;
        }
    }
    cout<<"\nSegunda raiz: "<<x1;
    cout<<"\nTotal de iterações: "<<cnt;
    cout<<"\n";
    
    cout<<"\nPara g3(x)=log(2.0*x*x), temos que: ";
    cnt=0;
    x1=x0;
    for(int i=0;i<max_it;i++){
        cnt++;
        tmp=x1;
        x1=g3(tmp);
        diferanca=abs(tmp-x1);
        if(diferanca<=tolerancia){
            break;
        }
    }
    cout<<"\nTerceira raiz: "<<x1;
    cout<<"\nTotal de iterações: "<<cnt;
    cout<<"\n\n";

}



/*
Função: Realizar as leituras das variávies
*/
double leitura(){
    double x0;
    cout<<"\nOlá! Você está na parte 1 do EP1 de MAC0210!\n\n";
    cout<<"Por favor insira o valor de x0: ";
    cin>>x0;
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

    return x0;
}



signed main() {

    double x0=leitura();

    metodo_do_ponto_fixo(x0);
    
    return 0;
}
