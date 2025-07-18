#------------------------------------------------------------------------------
# # MAT0122 ÁLGEBRA LINEAR I
# Aluno: Mariana Tiemi Silva Misu
# Numero USP: 12542842
# Tarefa: EP01 - Alinhamento na ONU
# Data: 29/08/2024
# 
# DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESTE PROGRAMA.  TODAS AS 
# PARTES DO PROGRAMA, EXCETO AS QUE SÃO BASEADAS EM MATERIAL FORNECIDO  
# PELO PROFESSOR OU COPIADAS DO LIVRO OU DO MATERIAL DIDÁTICO DE MAT0122, 
# FORAM DESENVOLVIDAS POR MIM.  DECLARO TAMBÉM QUE SOU RESPONSÁVEL POR TODAS 
# AS CÓPIAS DESTE PROGRAMA E QUE NÃO DISTRIBUÍ NEM FACILITEI A DISTRIBUIÇÃO
# DE CÓPIAS DESTA PROGRAMA.
#------------------------------------------------------------------------------
    

import sys

def list_dot(u,v):
    #codigo disponibilizado em aula, calcula o dot_product de u e v
    return sum([u[i]*v[i] for i in range(len(u))])

def align(dicionario,pais):
    rsp=[]
    for pais_tmp in dicionario:
        rsp.append((list_dot(dicionario[pais_tmp],dicionario[pais]),pais_tmp))

    rsp.sort(reverse=True)
    return rsp

def main():
    pais = sys.argv[1]

    dicionario = dict()

    f = open(sys.argv[2])

    linha=f.readline()
    while (linha!=''):
        conteudo=linha.split()
        pais_tmp=conteudo[0]
        conteudo.remove(pais_tmp)
        dicionario[pais_tmp]=[]
        for j in conteudo:
            dicionario[pais_tmp].append(int(j))
        linha=f.readline()

    resposta=align(dicionario,pais)

    for tmp in resposta:
        print(tmp[1])


main()
