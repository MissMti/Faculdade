# include <stdio.h>





#define PI               3.14159
#define RAIO_AP          200
#define RAIO_ZA          2000      /* zona de alerta */
#define DELTA_ALARME     60
#define EPS_COS          0.000001  /* precisao para cálculo do cosseno */
#define EPS              0.01      /* precisao para valores envolvendo metros */




int iguais(double x, double y) {
 if(x-y < EPS && y-x < EPS){
  return 1;
 }
 else{
  return 0;
 }
}





double cosseno(double theta, double epsilon){
 double rad,sinal,ele;
 double termo =1;
 double fat = 2 ;
 double soma_fat= 2;
 double soma_cos=1;
 int k = 1;
 int i;
 
 rad = theta*(PI)/180; 
 while (termo>epsilon){
  if (k%2==0){
  sinal = 1;
  }
  else{
  sinal = -1;
  }
  
  ele = rad;
  for (i=1;i<2*k;i++){
   ele = ele*rad;
  }
  
  termo = ele/soma_fat;
  soma_cos = soma_cos + sinal*termo;
  
  soma_fat = soma_fat*(fat+2)*(fat+1);
  fat = fat +2;
  k = k+1;  
 }

 return soma_cos;
}






int localiza ( double xi,double yi, double div, double xj, double yj, double djv, double xk, double yk, double dkv, double *xv, double *yv){
 double pi,pj,qi,qj;
 if ((xi == xj) && (xk == xi)){
  return 0;
 }
 else{
  if ((xi != xj) && (xi != xk)){
   pi = ((xi*xi) - (xj*xj) + (yi*yi) - (yj*yj) - (div*div) + (djv*djv) )/(2*(xi - xj));
   pj = ((xi*xi) - (xk*xk) + (yi*yi) - (yk*yk) - (div*div) + (dkv*dkv) )/(2*(xi - xk));
   qi = (yj - yi)/(xi - xj);
   qj = (yk - yi)/(xi - xk);
   if ((qi == qj) && (xj == xk)){ 
    return 0;
   }
  }
  if ((xi != xj) && (xj != xk)) {
      pi = ((xi*xi) - (xj*xj) + (yi*yi) - (yj*yj) - (div*div) + (djv*djv) )/(2*(xi - xj));
      pj = ((xj*xj) - (xk*xk) + (yj*yj) - (yk*yk) - (djv*djv) + (dkv*dkv) )/(2*(xj - xk));
      qi = (yj - yi)/(xi - xj);
      qj = (yk - yj)/(xj - xk);
      if ((qi == qj) && (xi == xk)){
       return 0;
      }
  }
  if ((xi != xk) && (xj != xk)){
      pi = ((xk*xk) - (xj*xj) + (yk*yk) - (yj*yj) - (dkv*dkv) + (djv*djv) )/(2*(xk - xj));
      pj = ((xi*xi) - (xk*xk) + (yi*yi) - (yk*yk) - (div*div) + (dkv*dkv) )/(2*(xi - xk));
      qi = (yj - yk)/(xk - xj);
      qj = (yk - yi)/(xi - xk);
      if ((qi == qj) && (xi == xj)){
          return 0;
      }
  }
 *yv = (pj-pi)/(qi-qj);
 *xv = pi + qi*(*yv);

 return 1;
 }
}






double raiz(double x, double epsilon) {
 double radiz, termo,ant;
 double condicao = 1;
 if (x==0){
   radiz=0;
  }
  else{
   termo = (x +1)/2;
   if (termo > epsilon){
    while (condicao>epsilon){
    ant = termo;
    termo = (ant + (x / ant))/2;
    condicao = ant - termo;
    if (condicao<0){
     condicao = condicao * (-1);
    }
    }
   }
   radiz = termo;
  }
 return radiz;
}






double velocidade(double x0, double y0, double x1, double y1, double deltaT){
  
  double cord,deltaS,veloz;
  
  cord = (x0-x1)*(x0-x1)+(y0-y1)*(y0-y1);
  deltaS = raiz(cord,EPS);
  veloz = deltaS/deltaT;
  
  return veloz;
  
}






int intercepta(double x0, double y0, double x1, double y1, double *x, double
*y) {

 double m,n,A,B,C,Delta,futy,xi,yi,xj,yj,disti,distj,distif,distjf;
 
  if (x0*x0 + y0*y0 <= 200){/* Pois se a posiao previa estava na AP, e a atual na ZA, é pq esta se afastando da AP*/
  return 0;
 }
 
 if (x0 == x1){
    if (x0 <= RAIO_AP && x0 >= RAIO_AP*(-1)){
    futy= (RAIO_AP*RAIO_AP) - (x0*x0);
    *y = raiz(futy,EPS);
    *x = x0;
    if (y0<0){
     *x = (-1)*(*x);
     *y = (-1)*(*y);
    }
    return 1;
   }
   else{
    return 0;
   }
 }
 else { /*Considerando o sitema dado no pdf, temos: y = mx +n e x2 + y2 = r2, entao x2 + (mx + n)2 = r2 */
  m = (y1 - y0)/(x1 - x0);
  n = y0 - m*x0;
  A = 1 + m*m;
  B = 2*m*n;
  C = n*n - RAIO_AP*RAIO_AP;
  Delta = B*B - 4*A*C;
  if (Delta<0){
   return 0;
  }
  else {
   if (Delta ==0){
     *x = ((-1)*B )/ (2*A);
     *y = m*(*x) + n;
     return 1;
   }
   else{
     xi = ((-1)*B - raiz(Delta,EPS))/ (2*A);
     yi = m*(xi) + n;
     xj = ((-1)*B + raiz(Delta,EPS))/ (2*A);
     yj = m*(xj) + n;
     disti = (x0 - xi)*(x0 - xi) + (y0 - yi)*(y0 - yi);
     distj = (x0 - xj)*(x0 - xj) + (y0 - yj)*(y0 - yj);
     distif = (x1 - xi)*(x1 - xi) + (y1 - yi)*(y1 - yi);
     distjf = (x1 - xj)*(x1 - xj) + (y1 - yj)*(y1 - yj);
     
     if (disti <= distj){
       if (distif > distjf){ /*Atravesa AP, logo esta se afastando */
        return 0;
       }
       else {
        *x = xi;
        *y = yi;
        return 1;
       }
     }
     else {
      if (distjf > distif){/*Atravesa AP, logo esta se afastando */
       return 0;
      }
      else{
       *x = xj;
       *y = yj;
       return 1;
      } 
     }
   }
  } 
 }
}






int main() {

 FILE *arq;
 char filename[256] ;
 int cont, n,condicao;
 int idv,ida,idb,idc,idaf,idbf,idcf;
 double posi_ax, posi_ay, Ha, thetaa;
 double posi_bx, posi_by, Hb, thetab;
 double posi_cx, posi_cy, Hc, thetac;
 double posi_axf, posi_ayf, Haf, thetaaf;
 double posi_bxf, posi_byf, Hbf, thetabf;
 double posi_cxf, posi_cyf, Hcf, thetacf;
 double tempo;
 
 double dav,dbv,dcv;
 double davf,dbvf,dcvf;              
 double xv,yv,xvf,yvf;
  
 int local,localf;
 double coord,coordf;
 double distperc,veloc,distorigem;
 
 int atinge;
 double futurox,futuroy;
 double distAPi,distAPf,p_tempo;
 
 int xigual,yigual;
 
 
 
 printf("\n\n Sistema de Monitoramento\n");
 printf("\n Digite o nome do arquivo com os dados a serem processados: ");
 scanf("%s", filename);
 arq = fopen(filename, "r");

 if (arq == NULL) {
  printf("\n Nao consegui abrir o arquivo %s.\n\n\n", filename);
  return 0;
 }

 fscanf(arq, "%d", &n);
 cont = 0 ;
 printf ("\n O número de casos a serem analisados é %d\n\n\n",n);




 while (cont < n) {
 
  condicao = 0;
  /*leitura de cada um dos n casos analisados*/
  fscanf(arq, "%d ", &idv);
  printf ("\n IDENTIFICAÇÃO DO VEICULO: %d \n",idv);
  
  fscanf (arq,"%d %lf %lf %lf %lf ",&ida,&posi_ax,&posi_ay,&Ha,&thetaa);
  fscanf (arq,"%d %lf %lf %lf %lf ",&idb,&posi_bx,&posi_by,&Hb,&thetab);
  fscanf (arq,"%d %lf %lf %lf %lf ",&idc,&posi_cx,&posi_cy,&Hc,&thetac);
  fscanf (arq,"%lf",&tempo);
  fscanf (arq,"%d %lf %lf %lf %lf ",&idaf,&posi_axf,&posi_ayf,&Haf,&thetaaf);
  fscanf (arq,"%d %lf %lf %lf %lf ",&idbf,&posi_bxf,&posi_byf,&Hbf,&thetabf);
  fscanf (arq,"%d %lf %lf %lf %lf ",&idcf,&posi_cxf,&posi_cyf,&Hcf,&thetacf);
  

  
  dav = Ha*cosseno(thetaa,EPS_COS);
  dbv = Hb*cosseno(thetab,EPS_COS);
  dcv = Hc*cosseno(thetac,EPS_COS);
  

  davf = Haf*cosseno(thetaaf,EPS_COS);
  dbvf = Hbf*cosseno(thetabf,EPS_COS);
  dcvf = Hcf*cosseno(thetacf,EPS_COS);
  
  printf ("\n Antenas na posicao previa\n");
  printf(" id |      posicao      |  H(m)  |theta(graus)| distancia (m) \n");
  printf(" %2d |(%8.2f,%8.2f)|%8.2f|  %8.2f  |%8.2f\n",ida,posi_ax,posi_ay,Ha,thetaa,dav);
  printf(" %2d |(%8.2f,%8.2f)|%8.2f|  %8.2f  |%8.2f\n",idb,posi_bx,posi_by,Hb,thetab,dbv);
  printf(" %2d |(%8.2f,%8.2f)|%8.2f|  %8.2f  |%8.2f\n",idc,posi_cx,posi_cy,Hc,thetac,dcv);
                

  local= localiza(posi_ax,posi_ay,dav,posi_bx,posi_by,dbv,posi_cx,posi_cy,dcv,&xv,&yv);
  localf= localiza(posi_axf,posi_ayf,davf,posi_bxf,posi_byf,dbvf,posi_cxf,posi_cyf,dcvf,&xvf,&yvf);
  
  
  coord = (xv-xvf)*(xv-xvf) + (yv-yvf)*(yv-yvf);
  distperc = raiz(coord,EPS);
  veloc = velocidade(xv,yv,xvf,yvf,tempo);
  coordf = xvf*xvf + yvf*yvf;
  distorigem = raiz(coordf,EPS);
  
  xigual = iguais(xv,xvf);
  yigual = iguais(yv,yvf);
  if (xigual == 1 && yigual == 1){
  veloc = 0.0;
  distperc = 0.0;
  }
  


  if (local == 0){/*caso nao seja possivel calcular a localizacao previa*/
      printf ("\n Não foi possivel fazer o calculo para determinar as coordenadas absolutas previa do veiculo");
      printf ("\n Logo, tambem nao foi possivel determinar a situacao do veiculo \n\n\n\n");
      condicao = 1;
  }
  
  else {
   printf("\n Localização previa: ( %.2f , %.2f )\n",xv,yv);
   printf("\n Intervalo de tempo:  %.2f segundos \n",tempo);
   printf("\n Antenas na posicao atual \n");
   printf(" id |      posicao      |  H(m)  |theta(graus)| distancia (m) \n");
   printf(" %2d |(%8.2f,%8.2f)|%8.2f|  %8.2f  |%8.2f\n",idaf,posi_axf,posi_ayf,Haf,thetaaf,davf);
   printf(" %2d |(%8.2f,%8.2f)|%8.2f|  %8.2f  |%8.2f\n",idbf,posi_bxf,posi_byf,Hbf,thetabf,dbvf);
   printf(" %2d |(%8.2f,%8.2f)|%8.2f|  %8.2f  |%8.2f\n",idcf,posi_cxf,posi_cyf,Hcf,thetacf,dcvf);
   
   if (localf == 0){ /*caso nao seja possivel calcular a localizacao atual*/
      printf ("\n Não foi possivel fazer o calculo para determinar as coordenadas absolutas atual do veiculo \n");
      printf ("\n Logo, tambem nao foi possivel determinar a situacao do veiculo \n\n\n\n ");
      condicao = 1;
   }
   
   else {
      printf("\n Localização atual: ( %.2f , %.2f )\n",xvf,yvf);
      printf("\n Distancia percorrida:  %.2f m \n",distperc);
      printf(" Velocidade:  %.2f m/s\n",veloc);
      printf(" Distancia da origem:  %.2f m \n",distorigem);
      
   }
   
  }
  



  
  if (condicao == 0){
     
     if (xvf*xvf + yvf*yvf > RAIO_ZA*RAIO_ZA){
       if (veloc == 0){
         printf("\n Veiculo estacionado FORA da Zona de Alerta\n");
       } 
       else {
         printf("\n Veiculo em movimento FORA da Zona de Alerta\n");
       }
     }
     else {
       if (xvf*xvf + yvf*yvf <= RAIO_AP*RAIO_AP){
            if (veloc == 0){
      		printf("\n Veiculo estacionado na AP\n\n");
            }
            else {
                printf("\n Veiculo em movimento na AP\n\n");
            }
            printf(" *************************************\n");
            printf("  ALARME ALARME ALARME ALARME ALARME  \n");
            printf("         Veiculo esta na AP           \n");
            printf(" *************************************\n");
            
       }
       else {
           if (veloc == 0){
      		 printf("\n Veiculo estacionado na ZONA DE ALERTA\n");
            }
            else {
                 printf("\n Veiculo em movimento na ZONA DE ALERTA\n");
                 
                
                 atinge = intercepta(xv,yv,xvf,yvf,&futurox,&futuroy);
                  if (atinge == 0){
                    printf ("\n Trajetoia NAO interceptara AP\n");
                  }
                 else {
                    
                    distAPi = (xv-futurox)*(xv-futurox) + (yv-futuroy)*(yv-futuroy);
                    distAPi = raiz(distAPi,EPS);
                    distAPf = (xvf-futurox)*(xvf-futurox) + (yvf-futuroy)*(yvf-futuroy);
                    distAPf = raiz(distAPf,EPS);
                    if (distAPi <= distAPf){
                     printf ("\n Trajetoia NAO interceptara AP\n");
                    }
                    else{
                     printf ("\n Trajetoia INTERCEPTARA AP\n");
                     printf("\n Distancia atual de AP: %.2f metros\n",distAPf);
                     p_tempo = distAPf/veloc;
                     printf(" Intersecção ocorerra em %.2f segundos, na coordenada (%.2f,%.2f)\n",p_tempo,futurox,futuroy);
                     if (p_tempo < DELTA_ALARME){
                       printf("\n *************************************\n");
            		printf("  ALARME ALARME ALARME ALARME ALARME  \n");
            		printf("         INVASAO IMINENTE!!!          \n");
            		printf(" *************************************\n");
                     }
                    }
                 }
            }
       }
     }
  
  
  printf ("\n\n\n\n");
  
  }
  
  
  cont++ ;
 }
 
 fclose(arq);
 return 0;
}
