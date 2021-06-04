#10727219 許志仲
def Isend( list ) :
    if list[0] + list[1] == 0 :
        return True
    else :
        return False

def Readsize() :
    lines = input()
    towersize = lines.split()
    while len( towersize ) != 2 :
        lines = input('Must be 2 numbers :')
        towersize = lines.split()

    for i in range(0, len(towersize)):
        towersize[i] = int(towersize[i])
    return towersize


def LCS(X, Y):
    UPPERLEFT = 1
    UP = 2
    LEFT = 3

    m = len(X)
    n = len(Y)

    c = [[0 for j in range(0, n + 1)] for i in range(0, m + 1)]
    b = [[0 for j in range(0, n + 1)] for i in range(0, m + 1)]

    for i in range(1, m + 1):
        c[i][0] = 0

    for j in range(1, n + 1):
        c[0][j] = 0

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if X[i - 1] == Y[j - 1]:
                c[i][j] = c[i - 1][j - 1] + 1
                b[i][j] = UPPERLEFT
            else:
                if c[i - 1][j] >= c[i][j - 1]:
                    c[i][j] = c[i - 1][j]
                    b[i][j] = UP
                else:
                    c[i][j] = c[i][j - 1]
                    b[i][j] = LEFT

    i = m
    j = n
    result = 0
    while i != 0 and j != 0:
        if b[i][j] == UPPERLEFT:
            result += 1
            i -= 1
            j -= 1
        elif b[i][j] == UP:
            i -= 1
        else:
            j -= 1

    return result

size = Readsize()
num = 0
while not Isend( size ) :
    num += 1

    tower1 = input()
    tower1 = tower1.split()
    if len( tower1 ) != size[0] :
        print('SizeError')
        break
    tower2 = input()
    tower2 = tower2.split()
    if len(tower2) != size[1]:
        print('SizeError')
        break

    for i in range(0, len(tower1)):
        tower1[i] = int(tower1[i])
    for i in range(0, len(tower2)):
        tower2[i] = int(tower2[i])

    print('Twin Towers #', num)
    ans = LCS(tower1, tower2)
    print('Number of Tiles : ', ans )
    size = Readsize()

print('End')