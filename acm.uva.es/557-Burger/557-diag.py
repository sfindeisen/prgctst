POW_2 = [1] # 2^0, 2^1, 2^2, 2^3, ...

def init(max_n):
    p2  = 1

    for i in range(1,max_n+1):
        p2  = p2 + p2
        POW_2.append(p2)

def solve_all(queries):
    max_n = max(queries)
    qset  = set(queries)
    qres  = dict()
    diag  = [1] * max_n # Pascal Triangle's diagonal

    for n in range(2, max_n + 1):
        # print(f"n = {n}")

        # compute new diag of length n-1
        if 2==n:
            diag[0] = 1
        elif 3==n:
            diag[0] = 1
            diag[1] = 3
        else:
            lastold = 0
            for i in range(1,n-2):
                lastold  = diag[i]
                diag[i] += diag[i-1]
            diag[n-2] = diag[n-3] + lastold + lastold

        # if n <= 12:
        #     print(f"n = {n}; diag = {diag[:n-1]}")

        if n in qset:
            # probability that Ben and Bill get the same type of burger
            prob = 0
            for i in range(n-1):
                prob += diag[i] / POW_2[n+i-1]
                if prob >= 0.9999:
                    break
            qres[n] = prob

    qres[0] = 0
    qres[1] = 0
    return qres

testcases = int(input())
queries   = []

for i in range(testcases):
    n = int(input())
    queries.append(n//2)

if 1 <= testcases:
    init(2 * max(queries))
    results = solve_all(queries)

    for n in queries:
        print("{:.4f}".format(results[n]))
