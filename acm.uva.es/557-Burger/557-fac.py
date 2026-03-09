POW_2 = [1] # 2^0, 2^1, 2^2, 2^3, ...

def init(max_n):
    p2  = 1

    for i in range(1,max_n+1):
        p2  = p2 + p2
        POW_2.append(p2)

def solve(n):
    # probability that Ben and Bill get the same type of burger
    prob = 1 / POW_2[n-1]
    bino = 1

    for i in range(1,n-1):
        bino  *= (n+i-1)
        bino //= i
        prob += bino / POW_2[n+i-1]
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
