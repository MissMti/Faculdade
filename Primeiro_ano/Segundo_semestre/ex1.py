def tumple_sum(A,B):
    return[(a+b,c+d) for ((a,c),(b,d)) in list(zip(A,B))]
    
tumple_sum([(1, 2),(10, 20)],[(3, 4),(30, 40)])
