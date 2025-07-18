%{Função Compress: tem como objetivo comprimir uma imagem;}%

function compress(originalImg, k)

  %Lê a imagem original
  imagem=imread(originalImg);
  
  %Calcula para encontrar a nova imagem
  p=size(imagem,1);
  ind=1:k+1:p;
  nova_imagem=imagem(ind,ind,:);

  %Gera a imagem comprimida
  imwrite(uint8(nova_imagem), "compressed.png",  "Quality", 100);

endfunction
