from sage.all import *
import random
import time

# Criptografa X e Calcula Y, segundo a forma do Algoritmo Menezes-Vanstone sobre a curva eliptica dada pelo enunciado
def criptografa_MV(X,k,P,Q,p):
    y0=k*P
    C=k*Q
    c1=C[0]
    c2=C[1]
    r1=X[0]
    r2=X[1]
    y1=(c1*r1)%p
    y2=(c2*r2)%p
    Y=(y0,y1,y2)
    return Y

# Descriptografa Y, segundo a forma do Algoritmo Menezes-Vanstone sobre a curva eliptica dada pelo enunciado
def descriptografa_MV(Y,s,p):
    C=s*Y[0]
    c1=C[0]
    c2=C[1]
    c1_inv=pow(c1, p-2, p)
    c2_inv=pow(c2, p-2, p)
    x1=(Y[1]*c1_inv)%p
    x2=(Y[2]*c2_inv)%p
    X=(x1,x2)
    return X

# Gerador de valores hexadecimais pseudo-aleatorios
def gerador_hex_aleatorio():
    random.seed(time.time())
    hex = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f']
    return random.choice(hex)

# Lê os primeiros 120 digitos de um arquivo
def le_prim_dig(nome_doc):
    saida=""
    with open(nome_doc, "r") as arquivo:
        conteudo=arquivo.read()
        saida+=conteudo[0:120]
    return saida
    
# Calcula a Diferença de Hamming entre os 10 primeiros blocos de dois arquivos
def dif_hamming(nome_doc1,nome_doc2,Tam_b):
    tmp=0
    with open(nome_doc1, "r", encoding="utf-8") as arquivo1:
        with open(nome_doc2, "r", encoding="utf-8") as arquivo2:
            conteudo1=bytes.fromhex(arquivo1.read())
            conteudo2=bytes.fromhex(arquivo2.read())

            block1=[conteudo1[i:i+Tam_b] for i in range(0, len(conteudo1),Tam_b)]
            block2=[conteudo2[i:i+Tam_b] for i in range(0, len(conteudo2),Tam_b)]
            for i in range(10):
                block_int1 = int.from_bytes(block1[i], 'big')
                block_int2 = int.from_bytes(block2[i], 'big')
                
                if(block_int1!=block_int2):
                    tmp+=1
    return tmp

# Criptografa documento em modo CBC
def criptografa_CBC(conteudo,k,P,Q,p,tam_b,vi):

    #Conteudo em hexadecimal para bits
    texto=bytes.fromhex(conteudo)

    #Separa em blocos
    blocks=[texto[i:i+tam_b] for i in range(0, len(texto),tam_b)]

    resultado=""
    residuos=""
    ant=vi
    ant_y1=int.from_bytes(ant[:1], 'big')
    ant_y2=int.from_bytes(ant[1:], 'big')

    C = k * Q  

    c1=C[0]
    c2=C[1] 
    

    y0=k*P
    

    for block in blocks:
        
        # Calcula y1 e y2
        x1=int.from_bytes(block[:1], 'big')
        x2=int.from_bytes(block[1:], 'big')

        y1=int((c1*x1)%p)
        y2=int((c2*x2)%p)

        #Faz XOR com o anterior
        tmp1=ant_y1^y1
        tmp2=ant_y2^y2

        #Atualiza o residuos
        if(tmp1>255):
            residuos+='01'
        else:
            residuos+='00'

        if(tmp2>255):
            residuos+='01'
        else:
            residuos+='00'

        resultado+=(hex(tmp1)[2:].zfill(2))[-2:]
        resultado+=(hex(tmp2)[2:].zfill(2))[-2:]
        
        # Atualiza valores anteriores para o próximo bloco
        ant_y1=tmp1
        ant_y2=tmp2
        

    return (y0,resultado,residuos)
    
    
# Descriptografa documento em modo CBC
def descriptografa_CBC(cont,k,Q,P,p,tam_b,vi,y0,s,residuos): 

    #Concatenar com o residuos
    conteudo= ''.join([residuos[i:i+2] +cont[i:i+2] for i in range(0, len(cont), 2)])

    #Conteudo em hexadecimal para bits
    texto=bytes.fromhex(conteudo)

    #Separa em blocos
    blocks=[texto[i:i+tam_b+2] for i in range(0, len(texto),tam_b+2)]


    resultado=""
    vi=bytes.fromhex("00000000")
    ant=vi
    ant_y1=int.from_bytes(ant[:2], 'big')
    ant_y2=int.from_bytes(ant[2:], 'big')

    C = s*y0
    c1=C[0]
    c2=C[1] 
    c1_inv=pow(c1, p-2, p)
    c2_inv=pow(c2, p-2, p)

    for block in blocks:

        
        
        # XOR com o bloco anterior (primeiro com y1, depois com y2)
        y1=int.from_bytes(block[:2], 'big')
        y2=int.from_bytes(block[2:], 'big')


        y1_pos=ant_y1^y1
        y2_pos=ant_y2^y2

        # Calcula x1 e x2
        x1=int((c1_inv*y1_pos)%p)
        x2=int((c2_inv*y2_pos)%p)

        resultado+=(hex(x1)[2:].zfill(2))[-2:]
        resultado+=(hex(x2)[2:].zfill(2))[-2:]
        
        # Atualiza valores anteriores para o próximo bloco
        ant_y1=y1
        ant_y2=y2
        
    return resultado




def ex1(p):# Gera curva Eliptica
    print("\n 1 - Crio uma curva eliptica E dada por y^2=x^3+2x+3 sobre o corpo finito Z*_263")
    F=GF(p)
    E=EllipticCurve(F,[2,3])
    print("\n")
    return E

def ex2(E,x,y): #Verifica se o ponto P pertence a curva
    print("\n 2 - Verifico se o Ponto P=(200,39) pertence a curva: ")
    try:
        P = E(x,y)  
        print("\nSim, o ponto P=(200,39) pertence a essa curva :) ")
    except:
        print("\nNão, o ponto P=(200,39) não pertence a curva :( ") 
    print("\n")
    return P

def ex3(E): #Verifica quantos pontos pertencem a essa curva
    pontos=E.points()
    print("\n 3 - Essa curva contem ",len(pontos)," pontos")
    print("\n")
    return pontos

def ex4(pontos): #Imprime os 10 primeiros pontos
    print("\n 4 - Os primeiros 10 pontos nessa curva são: ")
    for i in range(10):
        print(" P[",i+1,"]:  (",pontos[i][0],",",pontos[i][1],")")
    print("\n")
    
def ex5(E,x,y):#Verifica se R pertence a curva
    print("\n 5 - Agora, verifico se o Ponto R=(175,83) pertence a curva: ")
    try:
        R = E(x,y)  
        print("\n Sim, o ponto R=(175,83) pertence a essa curva :) ")
    except:
        print("\n Não, o ponto R=(175,83) não pertence a curva :( ") 
    print("\n")
    return R

def ex6(P,R):#Faz a soma
    soma=P+R
    print("\n 6 - A soma de P + R é : (",soma[0],",",soma[1],")")
    print("\n")
    return soma

def ex7(p,Nusp):#Gera s
    s=Nusp%p
    print("\n 7 - O valor de s é: ",s)
    print("\n")
    return s

def ex8(s,P):#Gera Q
    Q=s*P
    print("\n 8 - O valor de Q é: (",Q[0],",",Q[1],")")
    print(" E a Chave Pública da Alice é dada por: (Q,P)")
    print("\n")
    return Q

def ex9(s,Nusp,p):#Gera k
    k=(s+Nusp)%p
    print("\n 9 - O valor de k é: ",k)
    print("\n")
    return k

def ex10(R,k,P,Q,p):#Criptografa R
    Y=(criptografa_MV(R,k,P,Q,p))
    print("\n 10 - Agora criptografamos R=(175,83) e encontramos que Y=((",Y[0][0],",",Y[0][1],"), ",Y[1],", ",Y[2],")")
    print("\n")
    return Y

def ex11(Y,s,p):#Descriptografa R
    resultado=(descriptografa_MV(Y,s,p))
    print("\n 11 - Agora descriptografamos Y e encontramos o resultado = ", resultado)
    print("\n")
    return


def ex12(nome_doc,NUSP):#Cria arquivo
    print("\n 12 - Criamos o arquivo ",nome_doc," cujo os primeiros dígitos são: ")
    with open(nome_doc, "w", encoding="utf-8") as arquivo:

        arquivo.write(str(NUSP))
        for i in range(120000):
            tmp=gerador_hex_aleatorio()
            arquivo.write(tmp)
            
    print(le_prim_dig(nome_doc))
    print("\n")
    return

def ex13(k,P,Q,p,nome_doc,Tam_b,VI,doc_res):#Criptogrfa o arquivo
    
    with open(nome_doc, "r", encoding="utf-8") as arquivo:
        conteudo=arquivo.read()

    (y0,resultado,residuos)=criptografa_CBC(conteudo,k,P,Q,p,Tam_b,VI)

    with open(doc_res, "w", encoding="utf-8") as arquivo:
        arquivo.write(str(resultado))

    print("\n 13 - Criptografamos ",nome_doc," em modo CBC, resultando no arquivo doc1-cript.txt, cujo os primeiros digitos são:")
    print(le_prim_dig(doc_res))
    print("\n")
    return (y0,residuos)

def ex14(k,Q,P,p,tam_b,vi,nome_doc,doc_res,y0,s,residuos):#Descriptografa o arquivo
    with open(nome_doc, "r", encoding="utf-8") as arquivo:
        conteudo=arquivo.read()

    resultado= descriptografa_CBC(conteudo,k,Q,P,p,tam_b,vi,y0,s,residuos)

    with open(doc_res, "w", encoding="utf-8") as arquivo:
        arquivo.write(str(resultado))

    print("\n 14 - Descriptografamos ",nome_doc," em modo CBC, resultando no arquivo" ,doc_res,", cujo os primeiros digitos são:")
    print(le_prim_dig(doc_res))
    print("\n")
    return 

def ex15(Tam_Bloco,doc1,doc2):#Calcular dist de hamming
    dist=dif_hamming(doc1,doc2,Tam_Bloco)
    print("\n 15 - Calculando a Distancia de Hamming entre os 10 primeiros blocos X de", doc1," e Y de doc1-cript.txt, encontramos: ",dist)
    print("\n O resultado desejado seria uma distância grande, afinal quando criptografamos uma mensagem esperamos que ela seja diferente da original")
    print(" Para assim, ocultar a informação, senão, de nada valeria criptografar uma informação")
    print("\n")
    return 

def ex16(doc_1,nome_doc):#Cria arquvio
    print("\n 16 - Agora criamos o arquivo ",nome_doc," cujo os primeiros dígitos são: ")
    with open(doc_1, "r", encoding="utf-8") as arquivo:
        tmp = arquivo.read()

    tmp = tmp[1:]

    with open(nome_doc, "w", encoding="utf-8") as arquivo:
        arquivo.write(str(2))
        arquivo.write(tmp)

    print(le_prim_dig(nome_doc))
    print("\n")
    return

def ex17(k,P,Q,p,nome_doc,Tam_b,VI,doc_res):#Criptografa o arquivo
    with open(nome_doc, "r", encoding="utf-8") as arquivo:
        conteudo=arquivo.read()

    (y0,resultado,residuos)=criptografa_CBC(conteudo,k,P,Q,p,Tam_b,VI)

    with open(doc_res, "w", encoding="utf-8") as arquivo:
        arquivo.write(str(resultado))

    print("\n 17 - Criptografamos ",nome_doc," em modo CBC, resultando no arquivo doc1-cript.txt, cujo os primeiros digitos são:")
    print(le_prim_dig(doc_res))
    print("\n")
    return 

def ex18(Tam_Bloco,doc1,doc2):#Calcular dist de hamming
    dist=dif_hamming(doc1,doc2,Tam_Bloco)
    print("\n 18 - Calculando a Distancia de Hamming entre os 10 primeiros blocos Y dos doc1-cript.txt e doc2-cript.txt, encontramos: ",dist)
    print("\n O esperado seria que, por estar em modo CBC, uma pequena diferença no primeiro bloco se propague em todos os blocos seguintes")
    print(" Dessa forma, é esperado uma grande mudança entre os resultados das criptografias")
    print("\n")
    return 

def ex19(p,Data,P):#Gera novo s_b e Q_b
    sb=Data%p
    Qb=sb*P
    print("\n 19 - O valor de s_b é: ",sb)
    print("E o valor de Q_b é: (",Qb[0],",",Qb[1],")")
    print("\n")
    return sb

def ex20(k,P,Q,p,nome_doc,Tam_b,VI,doc_res):#Criptografa arquivo
    
    with open(nome_doc, "r", encoding="utf-8") as arquivo:
        conteudo=arquivo.read()

    
    (y0,resultado,residuos)=criptografa_CBC(conteudo,k,P,Q,p,Tam_b,VI)
    
    with open(doc_res, "w", encoding="utf-8") as arquivo:
        arquivo.write(str(resultado))

    print("\n 20 - Criptografamos ",nome_doc," em modo CBC com o Q_b, resultando no arquivo doc1-cript-beto.txt, cujo os primeiros digitos são:")
    print(le_prim_dig(doc_res))
    print("\n")
    return 

def ex21(Tam_Bloco,doc1,doc2):#Calcular dist de hamming
    dist=dif_hamming(doc1,doc2,Tam_Bloco)
    print("\n 21 - Calculando a Distancia de Hamming entre os 10 primeiros blocos Y dos doc1-cript.txt e doc1-cript-Beto.txt, encontramos: ",dist)
    print("\n O resultado esperado é que seja uma distância grande, afinal são usadas chaves diferentes")
    print(" Pelo Algoritmo de Menezes-Vanstone, mudar o Q muda os valores de c1 e c2 o que também espera-se gerar diferenças no resultado da criptografia")
    print(" E como o CBC tem dependencia do bloco anterior se a chave muda o primeiro bloco será bem diferente, propagando a diferenca nos blocos seguintes ")
    print("\n")
    return 





def main():

    print("\n\n==================================== EP 1 ====================================\n")
    
    p=263 #mod
    NUSP=12542842 #Meu número USP
    Tam_Bloco=2 #Tamanho de um Bloco
    VI=bytes.fromhex("0000")#Valor inicial, 4 chars de hexa -> 2 bytes

    E=ex1(p) #Curva eliptica

    P=ex2(E,200,39) #Ponto P=(200,39)

    pontos=ex3(E) #Pontos da curva

    ex4(pontos) #10 primeiros pontos

    R=ex5(E,175,83) #Ponto R=(200,39)

    soma_p_r=ex6(P,R) #Soma de P+R

    s=ex7(p,NUSP) #Chave Secreta s

    Q=ex8(s,P)
    chave_pub=(Q,P) #Chave Secreta

    k=ex9(s,NUSP,p) 

    Y=ex10(R,k,P,Q,p) #Resultadoda criptografia de R

    ex11(Y,s,p) #Descriptografia

    nome_doc1="documento1.txt"
    if len(sys.argv)>=2: #Vê se outro nome foi colocado na linha de comando
        nome_doc1=sys.argv[1]

    ex12(nome_doc1,NUSP) #Gera o arquivo

    doc1_cript="doc1-cript.txt"
    (y0,residuos)=ex13(k,P,Q,p,nome_doc1,Tam_Bloco,VI,doc1_cript) #Criptografa o arquivo

    doc1_cript_inv="doc1-cript-inverso.txt"
    ex14(k,Q,P,p,Tam_Bloco,VI,doc1_cript,doc1_cript_inv,y0,s,residuos) #Descriptografa o arquivo

    ex15(Tam_Bloco,nome_doc1,doc1_cript) #Faz análise da Diferença de Hamming
    
    nome_doc2="documento2.txt"
    ex16(nome_doc1,nome_doc2) #Cria arquivo

    doc2_cript="doc2-cript.txt"
    ex17(k,P,Q,p,nome_doc2,Tam_Bloco,VI,doc2_cript) #Criptografa arquivo
    
    ex18(Tam_Bloco,doc1_cript,doc2_cript) #Faz análise da Diferença de Hamming

    data_atu=18041250 #18/04 as 12:50, valor escolhido pela data no momento de criação
    sb=ex19(p,data_atu,P) #Gera s_b
    Qb=sb*P

    doc1_cript_Beto="doc1-cript-Beto.txt"
    ex20(k,P,Qb,p,nome_doc1,Tam_Bloco,VI,doc1_cript_Beto) #Criptografa arquivo
    
    ex21(Tam_Bloco,doc1_cript,doc1_cript_Beto) #Faz análise da Diferença de Hamming

    

    

    print("\n==================================== FIM ====================================\n\n")
    
    return 0


main()
