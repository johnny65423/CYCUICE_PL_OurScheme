
def minDistance(num, dist, sptSet):
    min = 99999999
    min_index = 0
    for i in range(num):
        if dist[i] < min and sptSet[i] == False:
            min = dist[i]
            min_index = i

    return min_index

def dijkstra(num, src, graph):

    dist = [99999999] * num
    dist[src] = 0
    sptSet = [False] * num

    for i in range(num):

        u = minDistance(num, dist, sptSet)

        sptSet[u] = True

        for v in range(num):
            if graph[u][v] > 0 and sptSet[v] == False and dist[v] > dist[u] + graph[u][v]:
                dist[v] = dist[u] + graph[u][v]

    return dist



num = int(input())

while num != 0 :
    lines = int(input())
    v = int(input())

    g = [[0 for i in range(num)]
            for j in range(num)]
    for i in range(lines) :
        info = input()
        info = info.split()

        for j in range(len(info)) :
            info[j] = int(info[j])
        g[info[0]-1][info[1]-1] = info[2]



    ans = []
    ans = dijkstra(num, v - 1, g)

    for i in range(1, len(ans)) :
        print('1 to', i+1, '=', ans[i])

    num = int(input())

print('quit')