#------------------------------------------------------------------------------
# # MAT0122 ÁLGEBRA LINEAR I
# Aluno: <NOME COMPLETO>
# Numero USP: <NUSP>
# Tarefa: <Identificação do exercício>
# Data: <DATA>
# 
# Baseado em ... (breve e se aplicável)
# 
# DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESTE PROGRAMA.  TODAS AS 
# PARTES DO PROGRAMA, EXCETO AS QUE SÃO BASEADAS EM MATERIAL FORNECIDO  
# PELO PROFESSOR OU COPIADAS DO LIVRO OU DO MATERIAL DIDÁTICO DE MAT0122, 
# FORAM DESENVOLVIDAS POR MIM.  DECLARO TAMBÉM QUE SOU RESPONSÁVEL POR TODAS 
# AS CÓPIAS DESTE PROGRAMA E QUE NÃO DISTRIBUÍ NEM FACILITEI A DISTRIBUIÇÃO
# DE CÓPIAS DESTA PROGRAMA.
#------------------------------------------------------------------------------

#Funcao do EX2#
def tumple_sum(A,B):
    return[(a+b,c+d) for ((a,c),(b,d)) in list(zip(A,B))]
    
#Teste para a funcao, exemplo dado apelo ex#    
tumple_sum([(1, 2),(10, 20)],[(3, 4),(30, 40)])
