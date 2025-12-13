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

def dfs(graph: Graph, dp: dict[str, int], node: str) -> int:
    if node == 'out':
        return 1

    if node in dp:
        return dp[node]
    
    result = 0
    for sink in graph[node]:
        result += dfs(graph, dp, sink)

    dp[node] = result
    return dp[node]

def main() -> None:
    graph = read_input("11/input.txt")

    print(f"Paths: {dfs(graph, {}, 'you')}")


if __name__ == "__main__":
    main()
