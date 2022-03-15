class node:

    def __init__(self, x=0, y=0, dist=0):
        self.x = x
        self.y = y
        self.dist = dist

def isin(x, y):
    if x >= 0 and x < 8 and y >= 0 and y < 8 :
        return True
    else :
        return False

def minStepToReachTarget(temp, goal):
    movex = [2, 2, -2, -2, 1, 1, -1, -1]
    movey = [1, -1, 1, -1, 2, -2, 2, -2]

    queue = []

    queue.append(node(temp[0], temp[1], 0))

    # make all cell unvisited
    visited = [[False for i in range(8)]
               for j in range(8)]


    visited[temp[0]][temp[1]] = True

    while (len(queue) > 0):

        t = queue[0]
        queue.pop(0)


        if (t.x == goal[0] and t.y == goal[1]):
            return t.dist

        for i in range(8):

            x = t.x + movex[i]
            y = t.y + movey[i]
            if ( isin(x, y) and not visited[x][y] ):
                visited[x][y] = True
                queue.append(node(x, y, t.dist + 1))

def toindex( num ) :
    if num == ['0', '0'] :
        return [0,0]
    ans = []
    for i in range( len(num) ):
        ans.append([ord(num[i][0]) - ord('a'), int(num[i][1])-1])
    return ans

num = input()
num = num.split()

index = toindex(num)

while index[0] != 0 and index[1] != 0 :

    ans = minStepToReachTarget(index[0], index[1])
    print("From %s to %s, Knight Moves = %d" % ( num[0], num[1], ans) )

    num = input()
    num = num.split()
    index = toindex(num)
    print(index)

print('quit')