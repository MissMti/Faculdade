%{Função Decompress: tem como objetivo descomprimir uma imagem;}%

function decompress(compressedImg, method, k, h)

  %Lê a imagem comprimida
  imagem_antiga=imread(compressedImg);


  %Calcula n e p
  n=size(imagem_antiga,1);
  p=n+(n-1)*k;


  %Começa a gerar a nova imagem colocando as informações da imagem comprimida
  
  if size(imagem_antiga, 3) == 1 % Para imagem preto e branca
    cores=1;
    nova_imagem=zeros(p,p,1,'uint8');
    nova_imagem(1:(k+1):p, 1:(k+1):p, :) = imagem_antiga;
  else
    cores=3;
    nova_imagem=zeros(p,p,3,'uint8');
    nova_imagem(1:(k+1):p, 1:(k+1):p, :) = imagem_antiga;
  end

  %Preenche a nova imagem uma cor por vez, considerando o método escolhido
  for RGB = 1:cores

    %Metodo Bilinear por Partes
    if(method==1)
           
      B = [ 1 0 0 0; 
            1 0 h 0; 
            1 h 0 0; 
            1 h h h*h];

      
      for i = 1:n-1
        for j = 1:n-1

          %Como no enunciado, F=Ba

          F=[imagem_antiga(i, j, RGB); 
             imagem_antiga(i, j+1, RGB); 
             imagem_antiga(i+1, j, RGB); 
             imagem_antiga(i+1, j+1, RGB)];

          a=B\double(F);
          a1=a(1);
          a2=a(2);
          a3=a(3);
          a4=a(4);
          
          ii=(i-1)*(k+1)+1;
          jj=(j-1)*(k+1)+1;

          %Preenche a nova imagem

          for linha = ii:ii+k
            for coluna = jj: jj+k
              valor= a1 + a2*(linha - ii)*h/(k+1) +a3*(coluna - jj)*h/(k+1) + a4*(linha - ii)*h/(k+1)*(coluna - jj)*h/(k+1);
              nova_imagem(linha, coluna, RGB) = max(0,min(valor,255));%Arrendonda
            end
          end
        end
      end


    %Metodo Bicubico
    else
      B= [1 0 0 0;
           1 h h*h h*h*h;
           0 1 0 0;
           0 1 2*h 3*h*h];

      

      %Pre-calcula as derivadas
      dfdx = zeros(p,p);
      dfdy = zeros(p,p);
      dfdxy = zeros(p,p);

      for i = 1:k+1:p
        for j = 1:k+1:p
            % Derivada x
            if i == 1
                dfdx(i,j) = (nova_imagem(i+(k+1),j,RGB) - nova_imagem(i,j,RGB))/h;
            elseif i == p
                dfdx(i,j) = (nova_imagem(i,j,RGB) - nova_imagem(i-(k+1),j,RGB))/h;
            else
                dfdx(i,j) = (nova_imagem(i+(k+1),j,RGB) - nova_imagem(i-(k+1),j,RGB))/(2*h);
            end
            
            % Derivada y
            if j == 1
                dfdy(i,j) = (nova_imagem(i,j+(k+1),RGB) - nova_imagem(i,j,RGB))/h;
            elseif j == p
                dfdy(i,j) = (nova_imagem(i,j,RGB) - nova_imagem(i,j-(k+1),RGB))/h;
            else
                dfdy(i,j) = (nova_imagem(i,j+(k+1),RGB) - nova_imagem(i,j-(k+1),RGB))/(2*h);
            end
            
            % Derivada mista 
            if i == 1 || i == p || j == 1 || j == p
                if i < p && j < p
                    dfdxy(i,j) = (nova_imagem(i+(k+1),j+(k+1),RGB) - nova_imagem(i+(k+1),j,RGB) - ...
                                 nova_imagem(i,j+(k+1),RGB) + nova_imagem(i,j,RGB))/h^2;
                else
                    dfdxy(i,j) = 0;
                end
            else
              dfdxy(i,j) = (nova_imagem(i+(k+1),j+(k+1),RGB) - nova_imagem(i+(k+1),j-(k+1),RGB) - ...
                             nova_imagem(i-(k+1),j+(k+1),RGB) + nova_imagem(i-(k+1),j-(k+1),RGB))/(4*h^2);
            end
        end
      end


      for i = 1:k+1:p-1
        for j = 1:k+1:p-1

          %Fazendo matriz, conforme o enunciado MAT=BAB^T (B^T é a trnasposta de B)

          MAT = double([nova_imagem(i,j,RGB)     nova_imagem(i,j+ (k+1),RGB)     dfdy(i,j)     dfdy(i,j+ (k+1));
                 nova_imagem(i+ (k+1),j,RGB)   nova_imagem(i+ (k+1),j+ (k+1),RGB)   dfdy(i+ (k+1),j)   dfdy(i+ (k+1),j+ (k+1));
                 dfdx(i,j)          dfdx(i,j+ (k+1))          dfdxy(i,j)    dfdxy(i,j+ (k+1));
                 dfdx(i+ (k+1),j)        dfdx(i+ (k+1),j+ (k+1))        dfdxy(i+ (k+1),j)  dfdxy(i+ (k+1),j+ (k+1))]);

          
          A = inv(B)*MAT*inv(B');
          
          %Preenche a nova imagem
          for linha = i: i+k
            for coluna = j:j+k


              x = [ 1  (linha - i)*h/(k+1)  ((linha - i)*h/(k+1))*((linha - i)*h/(k+1))  ((linha - i)*h/(k+1))*((linha -i)*h/(k+1))*((linha -i)*h/(k+1))];
              y = [ 1; 
                    (coluna  - j)*h/(k+1);
                    (coluna  - j)*h/(k+1)*(coluna  - j)*h/(k+1);
                    (coluna  - j)*h/(k+1)*(coluna  - j)*h/(k+1)*(coluna  - j)*h/(k+1)];
                

              nova_imagem(linha, coluna, RGB) =  max(0,min(x*A*y,255)); %Arrendonda para evitar valores irregulares
            end
          end
        end
      end
    end
  end

  
  %Gera a imagem descomprimida
  imwrite(uint8(nova_imagem),'decompressed.png','Compression', 'lzw');

endfunction
