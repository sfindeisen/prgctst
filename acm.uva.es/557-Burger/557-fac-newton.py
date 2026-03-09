POW_2 = [1] # 2^0, 2^1, 2^2, 2^3, ...
FAC   = [1] # 0! 1! 2! ...

def init(max_n):
    fac = 1
    p2  = 1

    for i in range(1,max_n+1):
        fac = fac * i
        FAC.append(fac)

        p2  = p2 + p2
        POW_2.append(p2)

def newton_binomial(n,k):
    return FAC[n] // FAC[k] // FAC[n-k]

def solve(n):
    # probability that Ben and Bill get the same type of burger
    prob = 0
    for i in range(n-1):
        prob += newton_binomial(n+i-1, i) / POW_2[n+i-1]
        if prob >= 0.9999:
            break
    print("{:.4f}".format(prob))

testcases = int(input())
queries2  = []

for i in range(testcases):
    n = int(input())
    queries2.append(n//2)

if 1 <= testcases:
    max_n = max(queries2)
    init(2*max_n)

    for n in queries2:
        if 1 == n:
            print("0.0000")
        else:
            solve(n)
