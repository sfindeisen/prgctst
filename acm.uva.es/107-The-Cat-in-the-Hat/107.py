from itertools import count
from math import log2

def compute(h0,wc):
    # h0 == (n+1) ** h
    # wc == n ** h
    for h in count(2):
        nap = int(wc ** (1/h))     # n approximate
        n0h = 1
        n1h = nap ** h

        for n in count(nap):
            n0h = n1h              # this trick is actually important; the ** operator is so expensive!
            n1h = (n+1) ** h

            if h0 == n1h and wc == n0h:
                # print (f"h={h} n={n}")
                return (h,n)
            if n1h > h0:
                break
            if n0h > wc:
                break

def print_res(h,n):
    if 1==n:
        cidle = h
    else:
        cidle = (n**h -1) // (n-1)
    htotal = sum(n**k * (n+1)**(h-k) for k in range(h+1))
    print("{} {}".format(cidle,htotal))

while True:
    parts = input().split()
    h0 = int(parts[0])
    wc = int(parts[1])

    if h0 == 0 and wc == 0:
        break

    if h0 == wc+1:
        # special case h=1 (just two levels of cats)
        print_res(1,wc)
    elif 1 == wc:
        # just one working cat: n=1
        h = int(log2(h0))
        print_res(h,1)
    else:
        (h,n) = compute(h0,wc)
        print_res(h,n)

