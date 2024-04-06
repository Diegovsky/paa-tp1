def eppstein(graph, s, t, k):
    q = Heap()
    visitados = [0] * len(graph)
    q.push(s, 0)
    while visitados[t] < k:
        (v, w) = q.pop()

        if visitados[v] == k:
            continue 

        visitados[v] += 1

        if v == t:
            continue

        for (u, l) in g.sucessors(v):
            push(q, u, w + l)
