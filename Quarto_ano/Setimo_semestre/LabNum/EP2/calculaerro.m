%{Função Calcula Erro: tem como objetivo calcular o erro entre a imagem A e a descompressa;}%

function calculateError(originalImg, decompressedImg)

  %Lê as imagens
  A = double(imread(originalImg));
  B = double(imread(decompressedImg));

  if size(A, 3) == 1 %Em caso de imagem preto e branca, repete a "cor" para outras tres colunas
    A = cat(3,A,A,A);
    B = cat(3,B,B,B);
  end

  erroR=0;
  erroG=0;
  erroB=0;

  p=size(B)(1);
  

  %Faz o somatório
  for i=1:p
    for j=1:p
      erroR=erroR+(A(i,j,1) - B(i,j,1))*(A(i,j,1) - B(i,j,1));
      erroG=erroG+(A(i,j,2) - B(i,j,2))*(A(i,j,2) - B(i,j,2));
      erroB=erroB+(A(i,j,3) - B(i,j,3))*(A(i,j,3) - B(i,j,3));
    end
  end

  %Divide por p^2
  erroR=erroR/(p*p);
  erroG=erroG/(p*p);
  erroB=erroB/(p*p);

  %Aplica a raiz quadrada
  erroR=sqrt(erroR);
  erroG=sqrt(erroG);
  erroB=sqrt(erroB);

  %Imprime a resposta
  rsp=(erroR+erroG+erroB)/3;
  disp(rsp); 

endfunction
