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
) -> list[tuple[float, Coordinate, Coordinate]]:
    junction_pairs: list[tuple[float, Coordinate, Coordinate]] = []
    for i in range(len(coordinates) - 1):
        for j in range(i + 1, len(coordinates)):
            dist = euclidean_distance(coordinates[i], coordinates[j])
            junction_pairs.append((dist, coordinates[i], coordinates[j]))

    # sort to get closest pairs
    return sorted(junction_pairs, key=lambda x: x[0])

def kruskal(coordinates: list[Coordinate], junction_pairs: list[tuple[float, Coordinate, Coordinate]]):
    parent = {coord: coord for coord in coordinates}
    curcuit_size = {coord: 1 for coord in coordinates}

    def _find(coord: Coordinate) -> Coordinate:
        if parent[coord] != coord:
            parent[coord] = _find(parent[coord])

        return parent[coord]
    
    def _union(a: Coordinate, b: Coordinate):
        root_a = _find(a)
        root_b = _find(b)
        if root_a != root_b:
            if curcuit_size[root_a] > curcuit_size[root_b]:
                parent[root_b] = root_a
                curcuit_size[root_a] += curcuit_size[root_b]
            else:
                parent[root_a] = root_b
                curcuit_size[root_b] += curcuit_size[root_a]

    for _, a, b in junction_pairs:
        if _find(b) != _find(a):
            _union(a, b)

            # last connection to make
            if len(set(_find(coord) for coord in coordinates)) == 1:
                return a[0] * b[0]

    return -1


def main() -> None:
    coordinates = read_input("8/input.txt")
    junction_pairs = create_junction_pairs(coordinates)

    print("Result:", kruskal(coordinates, junction_pairs))

if __name__ == "__main__":
    main()
