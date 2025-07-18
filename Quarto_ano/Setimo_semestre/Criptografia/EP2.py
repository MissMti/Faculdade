import random
import time



def extended_euclid (a , b ) :
    """
    Implementa algoritmo de Euclides estendido .
    Entrada: (int)a, (int)b.
    Saida : (s , t , gcd ) , inteiros tais que:
    s * a + t * b = gcd
    """

    if (a==0):#1 * b = gcd
        return(0,1,b)
    
    s,t,gcd=extended_euclid(b%a,a)

    tmp=t-int(b/a)*s
    
    return (tmp,s,gcd)



def inverse_mod (a , m ) :
    """
    Calcula a inversa de a modulo m usando o algoritmo de Euclides Estendido .
    Entrada: (int)a, (int)m.
    Saída: a^(-1)mod m , caso exista, ou seja, mdc(a,m)==1
    Caso não exista, levante Value Error
    """

    s , t , gcd = extended_euclid (a,m)
    
    if (gcd!=1):
        raise ValueError (f"mdc (a,m) deve ser 1 para haver inversa de a mod m")

    assert (a*s%m== 1)

    s=s%m
    return s



"""
Funçoes da Curva Eliptica
"""



EC_POINT_AT_INFINITY=None

MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS = (
    0,  
    7,  
    0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f 
)

MV_POINT_P = (
    0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798,
    0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8
)

MV_POINT_P_ORDER = 0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141



def EC_is_in_curve(ec_coefficients: tuple, point: tuple):
    """
    Verifica se um ponto pertence à curva elíptica.
    Entrada :
        ec_coefficients : (A , B , M ) coeficientes da curva eliptica C
            y ^2 = x ^3 + A * x + B modulo M
        point : (x , y ) onde x e y sao inteiros
    Saida :
        True se (x , y ) pertence a curva C, False caso contrario .
    """

    if point == EC_POINT_AT_INFINITY:
        return True
    
    #   Eu troquei de ordem aqui em relaçao ao codigo do enunciado, porque estava dando problema 
    #quando o point era ponto para o infinito
    A,B,M=ec_coefficients
    x,y=point

    if((y*y)%M==(x*x*x+A*x+B)%M):
        return True
    else:
        return False



def EC_invert_point(ec_coefficients: tuple, point: tuple):
    """
    Inverte um ponto da curva elíptica (calcula o oposto).
    Entrada :
        ec_coefficients : (A , B , M ) coeficientes da curva eliptica C
            y ^2 = x ^3 + A * x + B modulo M
        point : (x , y )
    Saida :
        (x , -y ) : o ponto oposto a point .
    """

    if (point == EC_POINT_AT_INFINITY):
        return point
    
    A, B, M = ec_coefficients
    x, y = point

    y=(-y)%M
    point=(x,y)
    
    return point



def EC_add_points(ec_coefficients: tuple, point1: tuple, point2: tuple):
    """
    Soma dois pontos em uma curva elíptica.
    Entrada :
        ec_coefficients : (A , B , M ) coeficientes da curva eliptica C
            y ^2 = x ^3 + A * x + B modulo M
        point1 : ( x1 , y1 ) onde x1 e y1 sao inteiros , um ponto de C
        point2 : ( x2 , y2 ) onde x2 e y2 sao inteiros , outro ponto de C

    Saida : Ponto (x , y ) pertence a curva C , dado pela soma dos pontos point1
    e point2 segundo a expressao de soma de pontos .
    """

    #Codigo no enunciado
    if not EC_is_in_curve(ec_coefficients, point1) or not EC_is_in_curve(ec_coefficients, point2):
        raise ValueError('Os dois pontos devem fazer parte da curva elíptica definida por ec_coefficients')
    
    if point1 == EC_POINT_AT_INFINITY:
        return point2

    if point2 == EC_POINT_AT_INFINITY:
        return point1
    
    x1, y1 = point1
    x2, y2 = point2
    A, B, M = ec_coefficients

    
    # Caso os pontos sejam opostos
    if (EC_invert_point(ec_coefficients,point1)==point2): 
        return EC_POINT_AT_INFINITY 
    
    
    # Caso os pontos sejam iguais
    if (point1 == point2): 
        if (y1 == 0):
            return EC_POINT_AT_INFINITY
        # Calcula lambda 
        l = (3*x1*x1 +A)*inverse_mod(2*y1,M)%M

    # Caso não sejam nem opostos nem iguais
    else:
        #Calcula labmda também
        l = (y2 - y1)*inverse_mod((x2 - x1)%M, M) % M
        
    
    # Calcula o novo ponto
    x3 = (l*l - x1 - x2)%M
    y3 = (l*(x1 - x3) - y1)%M
    
    return (x3, y3)



def EC_scalar_multiplication(ec_coefficients: tuple, scalar: int, point: tuple):
    """
    Multiplica um ponto por um escalar.
    Entrada:
        ec_coefficients : (A , B , M ) coeficientes da curva eliptica C
            y ^2 = x ^3 + A * x + B modulo M
        point : ( x , y ) onde x e y sao inteiros , um ponto de C
        scalar: um valor inteiro escalar
    Saída: 
        O produto entre o ponto e o escalar.   
    """

    #Caso de escalar 0 ou ponto pro infinito, vai dar ponto pro infinito,
    if (scalar == 0 or point == EC_POINT_AT_INFINITY):
        return EC_POINT_AT_INFINITY
    
    # Se o escalar era negativo
    if (scalar < 0):
        point = EC_invert_point(ec_coefficients, point)
    

    #Pseudocodigo
    # bits = representacao binaria de scalar ( bits mais significativos primeiro )
    scalar_pos = abs(scalar)

    bits=bin(scalar_pos)

    result_point=EC_POINT_AT_INFINITY

    for bit in bits:
        # Double
        result_point = EC_add_points(ec_coefficients, result_point, result_point)
        
        # Add
        if( bit == '1'):
            result_point = EC_add_points(ec_coefficients, result_point, point)

    return result_point



"""
Funçoes de assinatura digital
"""



def MV_keygen():
    """
    Gera um par de chaves pública e privada.
    Saida :
        s : um inteiro correspondente a chave secreta
        Q : um ponto na curva eliptica correspondente a chave publica Q = sP .
    """

    #Codigo do enunciado
    A, B, q = MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS
    n = MV_POINT_P_ORDER
    s = random.randrange(1, n) 

    Q = EC_scalar_multiplication(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, s, MV_POINT_P)  # Chave pública
    
    return s, Q



def MV_sign(secret_key: int, hash_x: int):
    """
    Gera uma assinatura para o hash_x usando a chave secreta.
    Entrada :
        secret_key : a chave secreta s
        hash_x : int , um inteiro entre 1 e n correspondente ao hash de uma mensagem
    Saida :
        (r , z ) : a assinatura sobre o hash_x
    """
    n = MV_POINT_P_ORDER
    z=0
    r=0
    
    while (z==0 or r==0):
        k = random.randrange(1, n) 
        r, t = EC_scalar_multiplication(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, k, MV_POINT_P)
        
        if( r!= 0):# r não pode ser zero, 1<=r,z<=n
            z = (inverse_mod(k, n) * (hash_x + secret_key * r)) % n
    
    return (r, z)



def MV_verify(public_key: tuple, signature: tuple, hash_x: int):
    """
    Verifica assinatura segundo algoritmo de Menezes - Vanstone

    Entrada :
        public_key : Q chave publica de quem assinou a mensagem x
        signature : (r , z ) a ( potencial ) assinatura de uma mensagem x
        hash_x : o hash da mensagem que foi assinada
    Saida :
        True se assinatura for valida , False caso contrario
    """
    n = MV_POINT_P_ORDER
    r, z = signature
    
    # Verifica se r e z 
    if ((not (1 <= r <= n-1)) or (not (1 <= z <= n-1))):
        return False
    
    try:
        z_inv = inverse_mod(z, n)
    except ValueError:
        return False  
    
    u1 = (z_inv*hash_x) % n
    u2 = (z_inv*r) % n
    
    # Calcula u1*P + u2*Q
    u1P = EC_scalar_multiplication(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, u1, MV_POINT_P)
    u2Q = EC_scalar_multiplication(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, u2, public_key)
    x0, y0 = EC_add_points(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, u1P, u2Q)

    if(x0%n == r%n):
        return True
    
    return False


def hamming_distance(a, b):
    xor=a^b #para encontrar os diferentes

    binario=bin(xor)

    resposta=0
    for bit in binario:
        if(bit=='1'):
            resposta+=1


    return resposta

"""
def EC_run_tests():
    
    #Testa as funções básicas de curva elíptica.
    
    # Testa se o ponto no infinito está na curva
    assert EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, EC_POINT_AT_INFINITY)
    
    # Testa se o ponto base P está na curva
    P = MV_POINT_P
    assert EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, P)
    
    # Testa adição e multiplicação escalar
    cummP = EC_POINT_AT_INFINITY
    for i in range(1, 300):
        cummP = EC_add_points(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, P, cummP)
        scalar_multP = EC_scalar_multiplication(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, i, P)
        assert cummP == scalar_multP
        assert EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, cummP)
    
    print('Completed EC_run_tests()')

def MV_run_tests():
    
    #Testa as funções do esquema Menezes-Vanstone.
    
    # Verifica parâmetros públicos
    assert MV_POINT_P != EC_POINT_AT_INFINITY
    assert EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, MV_POINT_P)
    assert EC_scalar_multiplication(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, 
                                  MV_POINT_P_ORDER, MV_POINT_P) == EC_POINT_AT_INFINITY
    
    # Testa assinatura e verificação
    s, Q = MV_keygen()
    n = MV_POINT_P_ORDER
    x = random.randrange(n)
    signature = MV_sign(s, x)
    
    assert MV_verify(Q, signature, x) == True
    assert MV_verify(Q, signature, x + 1) == False
    assert MV_verify(Q, signature, x - 1) == False
    assert MV_verify(Q, signature, x * n) == False
    
    print('Completed MV_run_tests()')
"""

def main():

    print("\n\n==================================== EP 2 ====================================\n")
    

    # Pontos para verificação
    Z=(0x714f956d8365148f9372ff1e69cf550b279381d6a837e87e5dcd38cfa1c56727,
       0xc403a250a26946f672517c118ed4f61b6c407d3407b0175437d1fba7a945d3e1)
    
    Y=(0x76e64113f677cf0e10a2570d599968d31544e179b760432952c02a4417bdde39,
       0xc90ddf8dee4e95cf577066d70681f0d35e2a33d2b56d2032b4b1752d1901ac01)
    
    X=(0x6cbc4847a842d45d0d031f0b933cfcb8488884685da384bc6c7ea175bd5dd1f0,
       0xc3d779d0048e8d4192864ae7226e9e6007a18bd8643e3d4f6f75b8a32ba6bc4c)
    
    """
    EC_run_tests()
    MV_run_tests()
    """

    EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS,EC_POINT_AT_INFINITY)

    # 1. O ponto Z pertence a curva?
    Z_is_in_curve = EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, Z)
    print(f'Q1: {"SIM" if Z_is_in_curve else "NAO"}\n')

    # 2. O ponto Y pertence a curva?
    Y_is_in_curve = EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, Y)
    print(f'Q2: {"SIM" if Y_is_in_curve else "NAO"}\n')
    
    # 3. O ponto X pertence a curva?
    X_is_in_curve = EC_is_in_curve(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, X)
    print(f'Q3: {"SIM" if X_is_in_curve else "NAO"}\n')

    # 4. Calcule o ponto Z+Y
    somaZY = EC_add_points(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, Z, Y)
    print(f'Q4: Z + Y = (0x{hex(somaZY[0])[2:]}, 0x{hex(somaZY[1])[2:]})\n')

    # 5. Calcule o ponto 2^(100)Z
    elevadoZ=EC_scalar_multiplication(MV_ELLIPTIC_CURVE_A_B_COEFFICIENTS, 2**100, Z)
    print(f'Q5: 2^100*Z = (0x{hex(elevadoZ[0])[2:]}, 0x{hex(elevadoZ[1])[2:]})\n')
    
    # 6. Gere um par de chaves privada e pública (s, Q) e mostre na saída padrão.
    s, Q = MV_keygen()
    print(f'Q6: s=0x{hex(s)[2:]},\n Q=(0x{hex(Q[0])[2:]}, 0x{hex(Q[1])[2:]})\n')

    # 7. Calcule a assinatura (r, z) de x usando a chave secreta s.
    x= 0xd221c4371788e41c91a4be95c9cac7a7ea7a593f405b4213a5d903a457dbfa8
    r,z = MV_sign(s, x)
    print(f'Q7: (r, z) = (0x{hex(r)[2:]}, 0x{hex(z)[2:]})\n')

    # 8. Verifique se assinatura (r, z) é válida para x usando a chave pública Q.
    print(f'Q8: {"SIM" if MV_verify(Q, (r,z), x) else "NAO"}\n')

    # 9. Construa x' = x ⊕ 1
    x_linha=x^1
    print(f'Q9: x\' = 0x{hex(x_linha)[2:]}\n')

    # 10. Verifique se a assinatura (r, z) de x vale como assinatura de x' 
    print(f'Q10: {"SIM" if MV_verify(Q, (r,z), x_linha) else "NAO"}\n')
    
    # 11. Calcule a assinatura (r',z') de x' 
    r_linha,z_linha=MV_sign(s, x_linha)
    print(f'Q11: (r\', z\') = (0x{hex(r_linha)[2:]}, 0x{hex(z_linha)[2:]})\n')

    # 12. Verifique se a assinatura (r',z') de x' vale como assinatura de x
    print(f'Q12: {"SIM" if MV_verify(Q, (r_linha,z_linha), x) else "NAO"}\n')

    # 13. Qual a distância de Hamming entre (r, z) e (r',z')?
    dist_r = hamming_distance(r, r_linha)
    dist_z = hamming_distance(z, z_linha)
    print(f'Q13: {dist_r+dist_z}\n')
    
    print("===============================================================================\n")
    

    return 0


main()
