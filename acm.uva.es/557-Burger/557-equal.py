def solve_all(queries):
    max_n = max(queries)
    qset  = set(queries)
    qres  = dict()
    bino  = 1                          # binomial coefficient (2k,k) = (2k)! / (k!*k!)
    k2    = 0                          # 2*k
    pow2  = 1                          # 2 ** k2

    for k in range(1, max_n):
        k2    += 2                     # 2*k
        pow2  += pow2 + pow2 + pow2    # 2 ** k2

        bino  *= (k2-1)
        bino  += bino
        bino //=  k

        if (k+1) in qset:
            # 1 - probability that Ben and Bill get equal types of burger
            qres[k+1] = 1 - bino / pow2

    qres[0] = 0
    qres[1] = 0
    return qres

testcases = int(input())
queries   = []

for i in range(testcases):
    n = int(input())
    queries.append(n//2)

if 1 <= testcases:
    results = solve_all(queries)

    for n in queries:
        print("{:.4f}".format(results[n]))
