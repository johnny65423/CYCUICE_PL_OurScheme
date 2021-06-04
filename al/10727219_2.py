import numpy
def Knapsack(value, weight, n, W):
    c = [[0 for w in range(W + 1)] for i in range(n + 1)]

    for w in range(W + 1):
        c[0][w] = 0

    for i in range(n + 1):
        c[i][0] = 0
        for w in range(1, W + 1):
            if (weight[i - 1] <= w):
                if (value[i - 1] + c[i - 1][w - weight[i - 1]] > c[i - 1][w]):
                    c[i][w] = value[i - 1] + c[i - 1][w - weight[i - 1]]

                else:
                    c[i][w] = c[i - 1][w]
            else:
                c[i][w] = c[i - 1][w]

    return c[n][W]



# end of function knapSack


# Driver Code
itemv = []
itemw = []
Weight = input()
Weight = int( Weight )

num = input()
num = int( num )

for i in range(num):
    item = input()
    item = item.split()
    if len(item) != 2 :
        print('error')
        break
    itemw.append(int(item[0]))
    itemv.append(int(item[1]))

ans = Knapsack(itemv, itemw, num, Weight)

find = False
for i in range( 2**len(itemv) ) :
    check = [int(d) for d in bin(i)[2:].zfill(len(itemv))]

    if numpy.dot( itemv, check ) == ans and numpy.dot( itemw, check ) <= Weight :
        find = True

        break

if not find :
    print('error')
else :
    print('Total Value = ', ans)
    ans = [idx for idx, val in enumerate(check) if val != 0]
    print('Items ',end=' ')
    print(", ".join( str(i+1) for i in ans))