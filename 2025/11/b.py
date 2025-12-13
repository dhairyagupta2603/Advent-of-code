type Graph = dict[str, set[str]]

def read_input(file_name: str):
    graph: Graph = dict()
    with open(file_name, "r") as f:
        for line in f.readlines():
            source, sinks = line.strip("\n").split(": ")
            graph[source] = set()
            for sink in sinks.split(" "):
                graph[source].add(sink.strip())

    return graph


def dfs(graph: Graph, dp: dict[str, int], node: str, end: str) -> int:
    if node == end: return 1
    if node == 'out': return 0
    
    if node in dp: return dp[node]
    
    result = 0
    for sink in graph[node]:
        result += dfs(graph, dp, sink, end)

    dp[node] = result
    return dp[node]


def main() -> None:
    graph = read_input("11/input.txt")

    x = dfs(graph, {}, 'svr', 'dac'); print(x, end=" ")
    y = dfs(graph, {}, 'dac', 'fft'); print(y, end=" ")
    z = dfs(graph, {}, 'fft', 'out'); print(z)

    a = dfs(graph, {}, 'svr', 'fft'); print(a, end=" ")
    b = dfs(graph, {}, 'fft', 'dac'); print(b, end=" ")
    c = dfs(graph, {}, 'dac', 'out'); print(c)

    print(f"\nPaths: {a*b*c + x*y*z}")

if __name__ == '__main__':
    main()