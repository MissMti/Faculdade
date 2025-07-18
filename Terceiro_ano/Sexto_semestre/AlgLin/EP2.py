#------------------------------------------------------------------------------
# # MAT0122 ÁLGEBRA LINEAR I
# Aluno: Mariana Tiemi Silva Misu
# Numero USP: 12542842
# Tarefa: EP02 - Código de Hamming
# Data: 26/10/2024
# 
# DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESTE PROGRAMA.  TODAS AS 
# PARTES DO PROGRAMA, EXCETO AS QUE SÃO BASEADAS EM MATERIAL FORNECIDO  
# PELO PROFESSOR OU COPIADAS DO LIVRO OU DO MATERIAL DIDÁTICO DE MAT0122, 
# FORAM DESENVOLVIDAS POR MIM.  DECLARO TAMBÉM QUE SOU RESPONSÁVEL POR TODAS 
# AS CÓPIAS DESTE PROGRAMA E QUE NÃO DISTRIBUÍ NEM FACILITEI A DISTRIBUIÇÃO
# DE CÓPIAS DESTA PROGRAMA.
#------------------------------------------------------------------------------
    


from vec import Vec
from mat import Mat
from bitutil import bits2mat, noise, mat2bits, bits2str, str2bits
from GF2 import one
from matutil import listlist2mat, mat2coldict, coldict2mat
from vecutil import list2vec



G = listlist2mat([[one, 0, one, one], 
                  [one, one, 0, one], 
                  [0, 0, 0, one],
                  [one, one, one, 0], 
                  [0, 0, one, 0], 
                  [0, one, 0, 0], 
                  [one, 0, 0, 0]])





H = listlist2mat([[0, 0, 0, one, one, one, one], 
                  [0, one, one, 0, 0, one, one],
                  [one, 0, one, 0, one, 0, one]])





#   Os valores de R foram encontrados com a ajuda do meu encontra.py, programa 
# que fiz para o E45, com apenas algumas modficações, além desses valores foram encontrado outros:
#   Primeira linha pode ser:
#        0 0 0 0 0 0 1 
#        0 0 0 1 1 1 0 
#        0 1 1 0 0 1 0 
#        0 1 1 1 1 0 1 
#        1 0 1 0 1 0 0 
#        1 0 1 1 0 1 1 
#        1 1 0 0 1 1 1 
#        1 1 0 1 0 0 0
#   Segunda linhas pode ser:
#        0 0 0 0 0 1 0 
#        0 0 0 1 1 0 1 
#        0 1 1 0 0 0 1 
#        0 1 1 1 1 1 0 
#        1 0 1 0 1 1 1 
#        1 0 1 1 0 0 0 
#        1 1 0 0 1 0 0 
#        1 1 0 1 0 1 1
#   Terceira linha pode ser:
#        0 0 0 0 1 0 0 
#        0 0 0 1 0 1 1 
#        0 1 1 0 1 1 1 
#        0 1 1 1 0 0 0 
#        1 0 1 0 0 0 1 
#        1 0 1 1 1 1 0 
#        1 1 0 0 0 1 0 
#        1 1 0 1 1 0 1 
#   Quarta linhas pode ser:
#        0 0 1 0 0 0 0 
#        0 0 1 1 1 1 1
#        0 1 0 0 0 1 1 
#        0 1 0 1 1 0 0 
#        1 0 0 0 1 0 1 
#        1 0 0 1 0 1 0 
#        1 1 1 0 1 1 0 
#        1 1 1 1 0 0 1 

R = listlist2mat([[0, 0, 0, 0, 0, 0, one], 
                  [0, 0, 0, 0, 0, one, 0],
                  [0, 0, 0, 0, one, 0, 0], 
                  [0, 0, one, 0, 0, 0, 0]])










def find_error(s:Vec):
    """
        Recebe: uma síndrome de erro s (em 3-vetor)
        Devolve: o vetor de erro correspondente (um 7-vetor)

        Fazendo o Lab 4.14 foi possível perceber que ao fazer H*e o resultado, que é
    uma sindrome de erro nada mais é do que o binário da posição do "erro" do e:
    
        111->0000001(one na sétima posição)
        110->0000010(one na sexta posição)
        101->0000100(one na quinta posição)
        100->0001000(one na quarta posição)
        011->0010000(one na terceira posição)
        010->0100000(one na segunda posição)
        001->1000000(one na primeira posição)

        Sendo assim, basta calcular do binário para o decimal de s e encontrar 
    a posição do erro, se tiver erro, afinal 000->0000000.
    """
    
    tmp=[m for m in s.D if s[m]==one] #encontra quais posicoes tem one
    
    soma=sum(pow(2,2-m) for m in tmp) #faz a conversao do binario pro decimal

    rsp=[0]*7 #lista de zeros
    
    rsp=list2vec(rsp) #lista para vetor
    
    if soma==0: #sem erros
        
        return rsp
    
    else: #se a soma é diferente de zero, coloca one na posicao do erro
    
        rsp[soma-1]=one
    
        return rsp
    




def find_error_matrix(S: Mat):
    """
        Recebe: um matriz de síndromes (cada coluna de S é uma síndrome)
        Devolve: uma matriz cujas colunas são os vetores-erro correspondentes.

        Com a função find_error(s) foi simples gerar essa função visto que 
    basta aplicar essa função em cada coluna de S.
    
    """
    
    m=mat2coldict(S) #matriz para dicionario de colunas

    tmp={n: find_error(m[n]) for n in m} #cria um dicionario de colunas de vetores de erro com o dicionario de colunas da matriz de sindrome de erros
    
    return coldict2mat(tmp) # diionario de colunas para matriz





def correct(C_TILDE: Mat):
    """
        Recebe: uma matriz que codifica um string s, possivelmente com erros
        Devolve: a matriz corrigida (dentro do possível)
    """

    matrix_sindrome_de_erros=H*C_TILDE # gera matriz de sindrome de erros

    E=find_error_matrix(matrix_sindrome_de_erros) # encontra a matriz de erros

    C=E+C_TILDE # corrige, dentro do possível, a C_TILDE

    return C





def encode_with_errors(s, f):
    """
    Recebe: um string s e uma frequência de erro f
    Devolve: a matriz C_TILDE = C + E 
    """

    tmp=str2bits(s) #string para bits

    P=bits2mat(tmp,4) #bits para uma matriz com 4 linhas

    C=G*P #codifica
    
    E=noise(C,f) #cria matriz erros
    
    C_TILDE=C+E #gera C_TILDE
    
    return C_TILDE





def decode_plain(C_TILDE): 
    """
    Recebe: uma matriz que codifica um string s, possivelmente com erros
    Devolve: um string s obtido decodificando C_TILDE sem corrigir os possíveis erros.
    """
    
    m=R*C_TILDE #decodifica a matriz
    
    tmp=mat2bits(m) #passa para bits
    
    return bits2str(tmp) #passa para string





def decode(C_TILDE:Mat):
    """
    Recebe: uma matriz que codifica um string s, possivelmente com erros
    Devolve: s
    """
    
    corrigida=correct(C_TILDE) #corrige possíveis erros C_TILDE
    
    m=R*corrigida #decodifica a matriz
     
    tmp=mat2bits(m) #passa para bits
    
    return bits2str(tmp) #passa para string
