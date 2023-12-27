import random
N=10000000
acertos=0
for i in range(N):
    x=random.uniform(0,1)
    y=random.uniform(0,1)
    if x*x + y*y <= 1.0:
        acertos=acertos+1

print(4*acertos/N)