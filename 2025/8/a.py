from pprint import pprint
import math
from collections import deque
from queue import Empty

type Coordinate = tuple[int, int, int]


def read_input(file_name: str) -> list[Coordinate]:
    coordinates: list[Coordinate] = []
    with open(file_name, "r") as f:
        for line in f.readlines():
            coordinate: Coordinate = tuple(
                map(int, line.strip().split(","))
            )  # pyright: ignore[reportAssignmentType]
            coordinates.append(coordinate)

    return coordinates


def euclidean_distance(p1: Coordinate, p2: Coordinate) -> float: # O(1)
    a, b, c = p1
    x, y, z = p2
    return math.sqrt((x - a) ** 2 + (y - b) ** 2 + (z - c) ** 2)


def create_junction_pairs(
    coordinates: list[Coordinate],
) -> list[tuple[float, Coordinate, Coordinate]]: # O(n**2)
    junction_pairs: list[tuple[float, Coordinate, Coordinate]] = []
    for i in range(len(coordinates) - 1):
        for j in range(i + 1, len(coordinates)):
            dist = euclidean_distance(coordinates[i], coordinates[j])
            junction_pairs.append((dist, coordinates[i], coordinates[j]))

    # sort to get closest pairs
    return sorted(junction_pairs, key=lambda x: x[0])


def create_graph(
    coordinates: list[Coordinate],
    junction_pairs: list[tuple[float, Coordinate, Coordinate]],
) -> dict[Coordinate, set[Coordinate]]:
    graph: dict[Coordinate, set[Coordinate]] = {coord: set() for coord in coordinates}
    for _, p1, p2 in junction_pairs:
        graph[p1].add(p2)
        graph[p2].add(p1)

    return graph


def traverse_graph(graph: dict[Coordinate, set[Coordinate]]):
    visited: set[Coordinate] = set()
    curcuits: list[list[Coordinate]] = []

    q: deque[Coordinate] = deque()
    for coord in graph:
        if coord in visited:
            continue

        stack = [coord]
        current_circuit: list[Coordinate] = []
        while stack:
            current = stack.pop()
            if current in visited:
                continue

            visited.add(current)
            current_circuit.append(current)
            stack.extend(graph[current] - visited)
        
        curcuits.append(current_circuit)
    
    return curcuits

def main() -> None:
    coordinates = read_input("8/input.txt")
    CONNECTIONS = 1000
    junction_pairs = create_junction_pairs(coordinates)[:CONNECTIONS]

    junction_graph = create_graph(coordinates, junction_pairs)
    curcuits = traverse_graph(junction_graph)
    curcuits = sorted(curcuits, key=lambda x: len(x), reverse=True)
    
    result = 1
    for curcuit in curcuits[:3]:
        result *= len(curcuit)

    print("Result:", result)

if __name__ == "__main__":
    main()
