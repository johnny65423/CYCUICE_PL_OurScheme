INFINITY = 1e30

def Dijkstra(v, source, graph):
    d = [1e30 for i in range(v + 1)]
    d[source] = 0
    set = [True] * (v + 1)

    n = v
    while (n != 0):
        # Extract Minimum
        u = 0
        min = INFINITY
        for i in range(1, v + 1):
            if (set[i] and d[i] < min):
                u = i
                min = d[i]
        set[u] = False
        n -= 1

        # Relax
        for v in range(1, v + 1):
            if (graph[u][v] != 0):
                if (d[v] > d[u] + graph[u][v]):
                    d[v] = d[u] + graph[u][v]

    for i in range(v):
        if source != i and d[i] != INFINITY:
            print(source + 1, 'to', i + 1, '=', d[i])


num = int(input())

while num != 0 :
    lines = int(input())
    source = int(input())

    g = [[0 for i in range(num+1)]
            for j in range(num+1)]
    for i in range(lines) :
        info = input()
        info = info.split()

        for j in range(len(info)) :
            info[j] = int(info[j])
        g[info[0]-1][info[1]-1] = info[2]

    Dijkstra(num, source-1, g)

    num = int(input())

print('quit')