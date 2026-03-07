MAX_N  = 200
MAX_N3 = MAX_N ** 3

cubes     = dict((k,k**3) for k in range(1,MAX_N+1))
cubes_rev = dict((k**3,k) for k in range(1,MAX_N+1))

# print(cubes)
# print(cubes_rev)

for a in range(2,MAX_N+1):
    a3 = cubes[a]

    for b in range(2,a):
        b3 = cubes[b]

        for c in range(b,a):
            c3 = cubes[c]
            d3 = a3 - b3 - c3

            if d3 < c3:
                break
            if d3 in cubes_rev:
                d = cubes_rev[d3]
                print(f"Cube = {a}, Triple = ({b},{c},{d})")
